#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include <map>


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader& shader, bool isCircle = false);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D& texture, glm::vec3 position, glm::vec3 size = glm::vec3(10.0f)
        , glm::vec3 rotate = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f));
private:
    // Render state
    Shader                          shader;
    unsigned int                    quadVAO;
    bool                            isCircle;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif