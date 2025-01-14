#include "sprite_renderer.h"
#include "../entities/text.h"

#define CRES 30

SpriteRenderer::SpriteRenderer(Shader& shader, bool isCircle)
{
    this->shader = shader;
    this->isCircle = isCircle;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, glm::vec3 rotate,  glm::vec3 color)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    ////                                                          x     y     z   - osa po kojoj se okrece
    //model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f)); // then rotate
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
    //model = glm::scale(model, glm::vec3(size, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f)); // last scale

    this->shader.SetMatrix4("model", model);

    this->shader.SetVector3f("spriteColor", color);
    this->shader.SetBool("isCircle", this->isCircle);

    if (!this->isCircle) {
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();

        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    else {
        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);
        glBindVertexArray(0);
    }
}

void SpriteRenderer::initRenderData()
{
    if(!this->isCircle) {
        // configure VAO/VBO
        unsigned int VBO;
        float vertices[] = {
            // pos           // tex
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f
        };
        //float vertices[] = {
        //    // pos      // tex
        //    0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        //    1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        //    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        //    0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        //    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        //    1.0f, 0.0f, 0.0f, 1.0f, 0.0f
        //};

        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    else {
        unsigned int VBO;

        float circle[2 * CRES + 4];
        circle[0] = 0.0;
        circle[1] = 0.0;
        for (int i = 0; i <= CRES; i++)
        {
            circle[2 + 2 * i] = cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
            circle[2 + 2 * i + 1] = sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
        }

        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVAO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
}