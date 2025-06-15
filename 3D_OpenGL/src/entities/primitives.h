#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../graphics/vertex.h"

class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Primitive() {}
	virtual ~Primitive() {}

	void Set(const Vertex* vetices, const unsigned nrOfVertices,
		     const GLuint* indices, const unsigned nrOfIndices);

	Vertex* GetVertices();
	GLuint* GetIndices();
	const unsigned GetNrOfVertices();
	const unsigned GetNrOfIndices();

};


class Quad : public Primitive
{
public:
	Quad();
};

class Triangle : public Primitive
{
public:
	Triangle();
};

class Pyramid : public Primitive
{
public:
	Pyramid();
};

class InvertedPyramid : public Primitive
{
public:
	InvertedPyramid();
};

class Cube : public Primitive
{
public:
	Cube();
};

#endif // !PRIMITIVES_H
