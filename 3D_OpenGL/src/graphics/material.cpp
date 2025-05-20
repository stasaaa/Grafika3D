#include "material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLuint diffuseTex, GLuint specularTex)
	: ambient(ambient), diffuse(diffuse), specular(specular), diffuseTex(diffuseTex), specularTex(specularTex) { }

void Material::SendToShader(Shader& shader) {
	shader.Use();
	shader.SetVector3f("material.ambient", this->ambient);
	shader.SetVector3f("material.diffuse", this->diffuse);
	shader.SetVector3f("material.specular", this->specular);
	shader.SetInteger("material.diffuseTex", this->diffuseTex);
	shader.SetInteger("material.specularTex", this->specularTex);
}