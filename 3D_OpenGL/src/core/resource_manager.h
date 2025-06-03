#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <GL/glew.h>
#include <glm/vec3.hpp>

#include <stb/stb_image.h>

#include "../graphics/texture.h"
#include "../graphics/shader.h"
#include "../graphics/material.h"
#include "../entities/mesh.h"
#include "light.h"


// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader*>       Shaders;
    static std::map<std::string, Texture2D*>    Textures;
    static std::map<std::string, Material*>     Materials;
    static std::map<std::string, Mesh*>         Meshes;
    static std::map<std::string, PointLight*>   PointLights;
    static std::map<std::string, glm::vec3*>    Lights;

    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader*    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader*    GetShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D* LoadTexture(const char* file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D* GetTexture(std::string name);

    static Material* LoadMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLuint diffuseTex, GLuint specularTex, std::string name);
    static Material* GetMaterial(std::string name);

    static Mesh* LoadMesh(Vertex* vertexArray, const unsigned& nrOfVertices,
        GLuint* indexArray, const unsigned& nrOfIndices,
        glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string name);
    static Mesh* LoadMesh(Primitive* primitive, glm::vec3 position, 
        glm::vec3 rotation, glm::vec3 scale, std::string name);
    static Mesh* GetMesh(std::string name);

    static glm::vec3* LoadLight(glm::vec3 lightPos, std::string name);
    static glm::vec3* GetLight(std::string name);

    //static PointLight* LoadPointLight() { return NULL; }
    //static PointLight* GetPointLight() { return NULL; }

    // properly de-allocates all loaded resources
    static void      Clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() {}
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

#endif