#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL2/SOIL2.h>

// Instantiate static variables
std::map<std::string, Texture2D*>    ResourceManager::Textures;
std::map<std::string, Shader*>       ResourceManager::Shaders;
std::map<std::string, Material*>     ResourceManager::Materials;
std::map<std::string, Mesh*>         ResourceManager::Meshes;
std::map<std::string, glm::vec3*>    ResourceManager::Lights;


Shader* ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = new Shader(loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile));
    return Shaders[name];
}

Shader* ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D* ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    Textures[name] = new Texture2D(loadTextureFromFile(file, alpha));
    return Textures[name];
}

Texture2D* ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto& iter : Shaders)
        glDeleteProgram(iter.second->ID);
    // (properly) delete all textures
    for (auto& iter : Textures)
        glDeleteTextures(1, &iter.second->ID);
    Materials.clear();
    Meshes.clear();
    Lights.clear();
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vShaderFile;
    const char* fShaderCode = fShaderFile;
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = SOIL_load_image(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    SOIL_free_image_data(data);
    return texture;
}

Material* ResourceManager::LoadMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLuint diffuseTex, GLuint specularTex, std::string name)
{
    Materials[name] = new Material(ambient, diffuse, specular, diffuseTex, specularTex);
    return Materials[name];
}

Material* ResourceManager::GetMaterial(std::string name)
{
    return Materials[name];
}

Mesh* ResourceManager::LoadMesh(Primitive* primitive, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string name)
{
    Meshes[name] = new Mesh(primitive, position, rotation, scale);
    return Meshes[name];
}

Mesh* ResourceManager::GetMesh(std::string name)
{
    return Meshes[name];
}

glm::vec3* ResourceManager::LoadLight(glm::vec3 lightPos, std::string name)
{
    Lights[name] = new glm::vec3(lightPos);
    return Lights[name];
}

glm::vec3* ResourceManager::GetLight(std::string name)
{
    return Lights[name];
}