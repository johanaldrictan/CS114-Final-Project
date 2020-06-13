#include <glad/glad.h>
#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "../include/Shader.h"
#include "../include/stb_image.h"
#include "../include/Input.h"
#include "../include/ResourceManager.h"
#include "../include/SpriteRenderer.h"
#include "../include/Particle.h"

//#include <direct.h>
//#define GetCurrentDir _getcwd

#define RAND(low, high) (low + rand() / (RAND_MAX / (high - low + 1) + 1))

#undef main

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
/*
std::string get_current_dir() {
	char buff[FILENAME_MAX]; //create string buffer to hold path
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}
*/
SpriteRenderer* Renderer;
ParticleGenerator* Particles;
SDL_Window* InitWindow();
int main()
{
	
	SDL_Window* window = InitWindow();
	if (window == NULL)
		return -1;

	// OpenGL configuration
	// --------------------
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialize resources
	// load shaders
	ResourceManager::loadShader("../shaders/sprite.vs", "../shaders/sprite.fs", nullptr, "sprite");
	ResourceManager::loadShader("../shaders/particle.vs", "../shaders/particle.fs", nullptr, "particle");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),
		static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);

	// set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"), 32, 32);
	// load textures
	ResourceManager::loadTexture("Fire.png", true, "fire");
	//ResourceManager::loadTexture("smoke.png", true, "particle");
	ResourceManager::loadTexture("smoke2.png", true, "particle");
	//ResourceManager::loadTexture("container.png", true, "box");
	//ResourceManager::loadTexture("awesomeface.png", true, "face");
	//Particles = new ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 100000, 256, 256);
	Particles = new ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 10000, 128, 128);

	ResourceManager::getShader("particle").use().setMatrix4("projection", projection);

	// deltaTime variables
	// -------------------
	Uint32 lastUpdate = SDL_GetTicks();

	bool Running = true;
	SDL_Event Event;

	//for calculating mouse position and and light source position
	static float xpos = 0.0f;
	static float ypos = 0.0f;
	float lightWidth = 500.0f;
	float lightHeight = 75.0f;

	while (Running)
	{

		SDL_PumpEvents();
		// calculate delta time for physics
		// --------------------
		Uint32 currentTime = SDL_GetTicks();


		float dT = (currentTime - lastUpdate) / 1000.0f;

		glm::vec2 lightCenter = glm::vec2(xpos, ypos);
		while (SDL_PollEvent(&Event) != 0)
		{
			switch (Event.type)
			{
			case SDL_QUIT:
				Running = false;
				break;
				//Calculate position of light source using mouse position
			case SDL_MOUSEMOTION:
				xpos = Event.motion.x - (lightWidth / 16.0f);
				ypos = Event.motion.y - (lightHeight / 16.0f);
				//std::cout << "X: " << xpos << " Y: " << ypos << "\n";
				lightCenter = glm::vec2(xpos, ypos);
				break;
			}
		}
		//Particles->Update(dT, glm::vec2(190.0f, 510.0f), 4);
		Particles->Update(dT, lightCenter + glm::vec2(45.0f,0.0f), 4);


		lastUpdate = currentTime;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		//Renderer->drawSprite(ResourceManager::getTexture("fire"), glm::vec2(150.0f, 500.0f),currentTime, glm::vec2(500.0f, 75.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		Renderer->drawSprite(ResourceManager::getTexture("fire"), lightCenter, currentTime, glm::vec2(lightWidth, lightHeight), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		Particles->Draw(currentTime);
		SDL_GL_SwapWindow(window);
	}
	delete Renderer;
	delete Particles;
	ResourceManager::clear();

	return 0;
}

SDL_Window* InitWindow() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* window = SDL_CreateWindow("SDL OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Failed to create SDL Window Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_GL_CreateContext(window);
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Failed to load GLAD!");
	}
	return window;
}