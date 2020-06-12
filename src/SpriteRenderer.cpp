#include "../include/SpriteRenderer.h"


SpriteRenderer::SpriteRenderer(Shader& shader, int width, int height)
{
    this->shader = shader;
    this->spriteWidth = width;
    this->spriteHeight = height;
    this->initRenderData();
    this->frame = 0.0;
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::drawSprite(Texture2D& texture, glm::vec2 position, Uint32 currentTime, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    this->shader.use();
    glm::mat4 model = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
        };
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    float dT = (currentTime - this->lastUpdate) / 1000.0f;
    int framesToUpdate = floor(dT / (1.0f / animatedFPS));
    if (framesToUpdate > 0) {
        frame += framesToUpdate;
        frame = (int)frame%(texture.Width / spriteWidth);
        this->lastUpdate = currentTime;
        
    }
    this->shader.setMatrix4("model", model);
    this->shader.setFloat("frame", frame);
    /*if (frame == texture.Width / spriteWidth - 1) {
        frame = 0.0;
    }
    frame++;*/

    // render textured quad
    this->shader.setVector3f("spriteColor", color);
    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}