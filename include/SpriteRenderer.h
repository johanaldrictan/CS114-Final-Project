#pragma once
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>

#include "texture.h"
#include "shader.h"

class SpriteRenderer
{
public:
    int spriteWidth, spriteHeight;
    SpriteRenderer(Shader& shader, int width, int height);
    ~SpriteRenderer();

    void drawSprite(Texture2D& texture, glm::vec2 position, Uint32 currentTime,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f)
        );
private:
    Shader       shader;
    unsigned int quadVAO;
    float frame;
    float lastUpdate;
    float animatedFPS = 12.0f;

    void initRenderData();
};
#endif