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
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),
		static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);

	// set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
	// load textures
	ResourceManager::loadTexture("container.png", true, "box");
	//ResourceManager::loadTexture("awesomeface.png", true, "face");

	//Probably gonna need this
	// deltaTime variables
	// -------------------
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool Running = true;
	SDL_Event Event;
	while (Running)
	{
		SDL_PumpEvents();
		// calculate delta time
		// --------------------
		float currentFrame = SDL_GetTicks();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		while (SDL_PollEvent(&Event) != 0)
		{
			if (Event.type == SDL_QUIT)
			{
				Running = false;
			}

		}
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer->drawSprite(ResourceManager::getTexture("box"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		//Renderer->drawSprite(ResourceManager::getTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		SDL_GL_SwapWindow(window);
	}
	delete Renderer;
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