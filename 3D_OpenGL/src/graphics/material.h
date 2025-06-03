#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "texture.h"

class Material 
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLuint diffuseTex;
    GLuint specularTex;

public:
    Material() {}
    //Material(glm::vec3 ambient, glm::vec3 diffuse, 
    //    glm::vec3 specular, Texture2D* texture);
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        GLuint diffuseTex, GLuint specularTex);
    ~Material() {}

    void SendToShader(Shader& shader);
};

#endif // !MATERIAL_H
