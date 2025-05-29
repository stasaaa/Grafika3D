#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/material.h"
#include "../graphics/vertex.h"
#include "../core/primitives.h"

class Mesh
{
private:
	unsigned nrOfVertices;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	glm::mat4 ModelMatrix;

	void InitVAO(Vertex* vertexArray, const unsigned& nrOfVerices,
				 GLuint* indexArray, const unsigned& nrOfIndices);
	void InitVAO(Primitive* primitive);
	void UpdateUniforms(Shader* shader);
	void UpdateModelMatrix();

public:
	Mesh() {}
	Mesh(Primitive* primitive, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	Mesh(Vertex* vertexArray, const unsigned& nrOfVerices, 
		 GLuint* indexArray, const unsigned& nrOfIndices,
		 glm::vec3 position = glm::vec3(0.f),
		 glm::vec3 rotation = glm::vec3(0.f),
		 glm::vec3 scale = glm::vec3(1.f));
	~Mesh();
	void Update();
	void Render(Shader* shader);
	void Move(const glm::vec3 position);
	void Rotate(const glm::vec3 rotation);
	void ScaleChange(const glm::vec3 scale);
};

#endif // !MESH_H
