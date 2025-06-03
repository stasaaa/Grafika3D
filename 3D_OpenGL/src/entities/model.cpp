#include "model.h"
#include "../core/OBJLoader.h"

Model::Model(glm::vec3 position, Material* material, 
	Texture2D* orTexDIf, Texture2D* orTexSpec, std::vector<Mesh*> meshes) :
	position(position), material(material), overrideTextureDiffuse(orTexDIf), overrideTextureSpecular(orTexSpec)
{
	for (auto* i : meshes)
	{
		this->meshes.push_back(new Mesh(*i));
	}

	for (auto& i : this->meshes)
	{
		i->Move(position);
	}
}

Model::Model(glm::vec3 position, Material* material,
	Texture2D* orTexDIf, Texture2D* orTexSpec, const char* objFile, glm::vec3 offSet) :
	position(position), material(material), overrideTextureDiffuse(orTexDIf), overrideTextureSpecular(orTexSpec)
{
	std::vector<Vertex> mesh = LoadOBJ(objFile);

	this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0,
		offSet, glm::vec3(0.f), glm::vec3(1.f)));
}

Model::~Model()
{
	for (auto*& i : this->meshes)
	{
		delete i;
	}
}

void Model::UpdateUniforms()
{
}

void Model::Rotate(glm::vec3 rotation)
{
	for (auto& i : this->meshes)
	{
		i->Rotate(rotation);
	}
}

float Model::CalculateToBottom()
{
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;

	for (auto& mesh : this->meshes)
	{
		Vertex* vertices = mesh->GetVertices();
		unsigned count = mesh->GetVertexCount();

		for (unsigned i = 0; i < count; ++i)
		{
			float y = vertices[i].position.y;
			if (y < minY) minY = y;
			if (y > maxY) maxY = y;
		}
	}

	float modelY = this->position.y;
	return modelY - minY;
}

void Model::Update()
{

}

void Model::Render(Shader* shader)
{
	this->UpdateUniforms();

	this->material->SendToShader(*shader);

	for (auto& i : this->meshes)
	{
		this->overrideTextureDiffuse->Bind();
		this->overrideTextureSpecular->Bind();
		i->Render(shader, this->position);
	}
}

void Model::UpdatePosition (glm::vec3 position)
{
	this->position = position;

}