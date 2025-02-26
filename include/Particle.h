#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <vector>
#include <SDL/SDL.h>

#include "Shader.h"
#include "Texture.h"

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Weight;
	float Life;
	float FrameW;
	float FrameH;

	Particle()
		: Position(0.0f), Velocity(1.0f), Color(1.0f), Weight(0.0f), Life(0.0f), FrameW(0.0f), FrameH(0.0f) {}
};

class ParticleGenerator{
	public:
		ParticleGenerator(Shader s, Texture2D texture, unsigned int amount, int width, int height);
		void Update(float dt, glm::vec2 center, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
		void Draw(Uint32 currentTime);
	private:
		std::vector<Particle> particles;
		unsigned int amount;
		Shader shader;
		Texture2D texture;
		int spriteWidth;
		int spriteHeight;
		float lastUpdate;
		float animatedFPS = 12.0f;

		unsigned int VAO;
		void init();
		unsigned int firstUnusedParticle();
		// stores the index of the last particle used (for quick access to next dead particle)
		unsigned int lastUsedParticle = 0;
		void respawnParticle(Particle& particle, glm::vec2 center, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif
