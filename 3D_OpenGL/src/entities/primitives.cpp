#include "primitives.h"

void Primitive::Set(const Vertex* vertices, const unsigned nrOfVertices,
					const GLuint* indices, const unsigned nrOfIndices)
{
	for (size_t i = 0; i < nrOfVertices; i++)
	{
		this->vertices.push_back(vertices[i]);
	}
	for (size_t i = 0; i < nrOfIndices; i++)
	{
		this->indices.push_back(indices[i]);
	}
}

Vertex* Primitive::GetVertices()
{
	return this->vertices.data();
}

GLuint* Primitive::GetIndices()
{
	return this->indices.data();
}

const unsigned Primitive::GetNrOfVertices()
{
	return this->vertices.size();
}

const unsigned Primitive::GetNrOfIndices()
{
	return this->indices.size();
}


Quad::Quad() : Primitive()
{
	Vertex vertices[] = {
		{ {0.0f, 0.0f, 0.0f}, {1.f, 1.f, 1.f}, {0.0f, 1.0f}, {0.f, 0.f, 1.f} },
		{ {1.0f, 0.0f, 0.0f}, {1.f, 1.f, 1.f}, {1.0f, 1.0f}, {0.f, 0.f, 1.f} },
		{ {1.0f, 1.0f, 0.0f}, {1.f, 1.f, 1.f}, {1.0f, 0.0f}, {0.f, 0.f, 1.f} },
		{ {0.0f, 1.0f, 0.0f}, {1.f, 1.f, 1.f}, {0.0f, 0.0f}, {0.f, 0.f, 1.f} }
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	unsigned int indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->Set(vertices, nrOfVertices, indices, nrOfIndices);
}

Triangle::Triangle() : Primitive()
{
	Vertex vertices[] = {
		{ {0.0f, 0.0f, 0.0f}, {1.f, 1.f, 1.f}, {0.0f, 1.0f}, {0.f, 0.f, 1.f} },
		{ {1.0f, 0.0f, 0.0f}, {1.f, 1.f, 1.f}, {1.0f, 1.0f}, {0.f, 0.f, 1.f} },
		{ {1.0f, 1.0f, 0.0f}, {1.f, 1.f, 1.f}, {1.0f, 0.0f}, {0.f, 0.f, 1.f} },
		//{ {0.0f, 1.0f, 0.0f}, {1.f, 1.f, 1.f}, {0.0f, 0.0f}, {0.f, 0.f, 1.f} }
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	unsigned int indices[] =
	{
		0, 1, 2,
		//0, 2, 3
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->Set(vertices, nrOfVertices, indices, nrOfIndices);
}

Pyramid::Pyramid() : Primitive()
{
	Vertex vertices[] = {
		// Position					// Color			//Texture		// Normal

		// Triangle front
		{{0.f, 0.5f, 0.f},			{1.f, 1.f, 1.f},	{0.5, 1.f},		{0.f, 0.f, 1.f}},
		{{-0.5f, -0.5f, 0.5f},		{1.f, 1.f, 1.f},	{0.f, 0.f},		{0.f, 0.f, 1.f}},
		{{0.5f, -0.5f, 0.5f},		{1.f, 1.f, 1.f},	{1.f, 0.f},		{0.f, 0.f, 1.f}},

		// Triangle left
		{{0.0f, 0.5f, 0.f},			{1.f, 1.f, 1.f},	{0.5f, 1.f},	{-1.f, 0.f, 0.f}},
		{{-0.5f, -0.5f, -0.5f},		{1.f, 1.f, 1.f},	{0.f, 0.f},		{-1.f, 0.f, 0.f}},
		{{-0.5f, -0.5f, 0.5f},		{1.f, 1.f, 1.f},	{1.f, 0.f},		{-1.f, 0.f, 0.f}},

		// Triangle back
		{{0.0f, 0.5f, 0.f},			{1.f, 1.f, 1.f},	{0.5f, 1.f},	{0.f, 0.f, -1.f}},
		{{0.5f, -0.5f, -0.5f},		{1.f, 1.f, 1.f},	{0.f, 0.f},		{0.f, 0.f, -1.f}},
		{{-0.5f, -0.5f, -0.5f},		{1.f, 1.f, 1.f},	{1.f, 0.f},		{0.f, 0.f, -1.f}},

		// Triangle right
		{{0.f, 0.5f, 0.f},			{1.f, 1.f, 1.f},	{0.5f, 1.f},	{1.f, 0.f, 0.f}},
		{{0.5f, -0.5f, 0.5f},		{1.f, 1.f, 1.f},	{0.f, 0.f},		{1.f, 0.f, 0.f}},
		{{0.5f, -0.5f, -0.5f},		{1.f, 1.f, 1.f},	{1.f, 0.f},		{1.f, 0.f, 0.f}}

		//// Base
		//{{0.5f, 0.0f, 0.5f},   {0, 0, 1}, {0, 0}, {0.707f, 0.5f, 0.707f}},
		//{{-0.5f, 0.0f, 0.5f},  {1, 1, 0}, {1, 0}, {-0.707f, 0.5f, 0.707f}},
		//{{0.0f, 1.0f, 0.0f},   {1, 0, 1}, {0.5f, 1}, {0, 0.5f, 1}},

		//{{-0.5f, 0.0f, 0.5f},  {1, 1, 0}, {0, 0}, {-0.707f, 0.5f, 0.707f}},
		//{{-0.5f, 0.0f, -0.5f}, {1, 0, 0}, {1, 0}, {-0.707f, 0.5f, -0.707f}},
		//{{0.0f, 1.0f, 0.0f},   {0, 1, 1}, {0.5f, 1}, {-1, 0.5f, 0}}
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	this->Set(vertices, nrOfVertices, nullptr, 0);
}

Cube::Cube() : Primitive()
{
	Vertex vertices[] = {
		// Front face (normal 0,0,1)
		{{-0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}},

		{{-0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}},
		{{-0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 1.f}, {0.f, 0.f, 1.f}},

		// Back face (normal 0,0,-1)
		{{ 0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, 0.f, -1.f}},
		{{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f, -1.f}},
		{{-0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, 0.f, -1.f}},

		{{ 0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, 0.f, -1.f}},
		{{-0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, 0.f, -1.f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 1.f}, {0.f, 0.f, -1.f}},

		// Left face (normal -1,0,0)
		{{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{-0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{-0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}},

		{{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{-0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}},
		{{-0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f}},

		// Right face (normal 1,0,0)
		{{ 0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 0.f}, {1.f, 0.f, 0.f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}},

		{{ 0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 1.f}, {1.f, 0.f, 0.f}},

		// Top face (normal 0,1,0)
		{{-0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 0.f}, {0.f, 1.f, 0.f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, 1.f, 0.f}},

		{{-0.5f,  0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, 1.f, 0.f}},
		{{-0.5f,  0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 1.f}, {0.f, 1.f, 0.f}},

		// Bottom face (normal 0,-1,0)
		{{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 0.f}, {0.f, -1.f, 0.f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},

		{{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},
		{{-0.5f, -0.5f,  0.5f}, {1.f, 1.f, 1.f}, {0.f, 1.f}, {0.f, -1.f, 0.f}},
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	this->Set(vertices, nrOfVertices, nullptr, 0);
}