#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/material.h"
#include "../core/resource_manager.h"

class Model
{
private:
	Material* material;
	Texture2D* overrideTextureDiffuse;
	Texture2D* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;

	void UpdateUniforms();

public:
	Model() {}
	Model(glm::vec3 position, Material* material, Texture2D* orTexDIf, 
		Texture2D* orTexSpec, const char* objFile, glm::vec3 offSet = glm::vec3(0.f));
	Model(glm::vec3 position, Material* material,
		Texture2D* orTexDIf, Texture2D* ortexSpec, std::vector<Mesh*> meshes);
	~Model();

	void Rotate(glm::vec3 rotation);
	void SetRotation(glm::vec3 rotation);
	void UpdatePosition(glm::vec3 position);
	void UpdateScale(glm::vec3 scale);
	float CalculateToBottom();
	void Update();
	void Render(Shader* shader);
};

#endif // !MODEL_H