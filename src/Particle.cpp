#include "../include/Particle.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount, int width, int height)
	: shader(shader), texture(texture), amount(amount), spriteWidth(width), spriteHeight(height)
{
	this->init();
}

void ParticleGenerator::Update(float dt, glm::vec2 center, unsigned int newParticles, glm::vec2 offset) 
{
	for (unsigned int i = 0; i < newParticles; i++) {
		int unusedParticle = this->firstUnusedParticle();
		if (unusedParticle == -1) {
			break;
		}
		else {
			this->respawnParticle(this->particles[unusedParticle], center, offset);
		}
		
	}

	for (unsigned int i = 0; i < this->amount; i++) {
		Particle& p = this->particles[i];
		
		if (p.Life > 0.0f) {
			p.Life -= dt;
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 0.4f;
		}
	}
}


void ParticleGenerator::Draw(Uint32 currentTime)
{
	float dT = (currentTime - this->lastUpdate) / 1000.0f;
	int framesToUpdate = floor(dT / (1.0f / animatedFPS));
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE); //additive blending
	this->shader.use();
	for (unsigned int i = 0; i < this->amount; i++)
	{
		Particle& particle = this->particles[i];
		if (particle.Life > 0.0f)
		{
			
			if (framesToUpdate > 0) {
				//std::cout << "Rows: " << (float)(texture.Height / spriteHeight) << " Cols: " << (float)(texture.Width / spriteWidth) << std::endl;
				if (particle.FrameW == ((texture.Width / spriteWidth)-1)) {
					particle.FrameH += framesToUpdate;
					particle.FrameH =(int)particle.FrameH % (texture.Height / spriteHeight);
				}
				particle.FrameW += framesToUpdate;
				particle.FrameW = (int)particle.FrameW % (texture.Width / spriteWidth);
			}	

			/*if (particle.FrameW == texture.Width / spriteWidth - 1) {
				particle.FrameW = 0.0;
			}
			particle.FrameW = particle.FrameW+1.0;*/

			this->shader.setVector2f("offset", particle.Position);
			this->shader.setFloat("frameW", particle.FrameW);
			this->shader.setFloat("frameH", particle.FrameH);
			this->shader.setFloat("numCol", (float)(texture.Width / spriteWidth));
			this->shader.setFloat("numRow", (float)(texture.Height / spriteHeight));


			this->shader.setVector4f("color", particle.Color);

			
			//this->shader.setFloat("numCol", (float)(texture.Width / spriteWidth));


			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
		if (framesToUpdate > 0) {
			this->lastUpdate = currentTime;
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() 
{
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// create this->amount default particle instances
	for (unsigned int i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

unsigned int ParticleGenerator::firstUnusedParticle()
{
	// first search from last used particle, this will usually return almost instantly
	for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// otherwise, do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return -1;
}

void ParticleGenerator::respawnParticle(Particle& particle, glm::vec2 center, glm::vec2 offset)
{
	float randx = ((rand() % 40) + -40);
	float randy = ((rand() % 80) + 30);
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	float randA = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	particle.Position = center + glm::vec2(randx, 0.0f);
	particle.Color = glm::vec4(rColor, rColor, rColor, randA);
	particle.Life = 2.0f;
	particle.Velocity = glm::vec2(0.0f, randy);
	particle.FrameH = 0.0f;
	particle.FrameW = 0.0f;
}