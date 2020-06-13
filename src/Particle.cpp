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
			p.Color.a -= dt * 0.5f;
		}
	}
}


void ParticleGenerator::Draw(Uint32 currentTime)
{
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE); //additive blending
	this->shader.use();
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			float dT = (currentTime - this->lastUpdate) / 1000.0f;
			int framesToUpdate = floor(dT / (1.0f / animatedFPS));
			if (framesToUpdate > 0) {
				particle.Frame += framesToUpdate;
				particle.Frame = (int)particle.Frame % (texture.Width / spriteWidth);
				this->lastUpdate = currentTime;

			}		

			this->shader.setVector2f("offset", particle.Position);
			this->shader.setVector4f("color", particle.Color);

			this->shader.setFloat("frame", 0);
			this->shader.setFloat("numRow", (float)(texture.Height / spriteHeight));
			this->shader.setFloat("numCol", (float)(texture.Width / spriteWidth));

			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
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
	float randx = ((rand() % 100) + 10);
	float randy = ((rand() % 100) + 10);
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = center;
	//particle.Position = center + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 10.0f;
	particle.Velocity = glm::vec2(0.0f, randy);
}