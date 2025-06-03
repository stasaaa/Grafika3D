#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "../graphics/shader.h"

class Light
{
protected:
	float intensity;
	glm::vec3 color;

public:
	Light() {}
	Light(float intensity, glm::vec3 color)
		: intensity(intensity), color(color) {}
	~Light() {}

	virtual void SendToShader(Shader& shader) = 0;
};

class PointLight : public Light
{
protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

public:
	PointLight() : Light() {}
	PointLight(glm::vec3 position, 
		float intensity = 1.f, glm::vec3 color = glm::vec3(1.f),
		float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f) 
		: position(position), constant(constant), linear(linear), 
		quadratic(quadratic), Light(intensity, color) {}
	~PointLight() {}

	void SendToShader(Shader& shader)
	{
		shader.Use();
		shader.SetVector3f("pointLight.position", this->position);
		shader.SetFloat("pointLight.intensity", this->intensity);
		shader.SetVector3f("pointLight.color", this->color);
		shader.SetFloat("pointLight.constant", this->constant);
		shader.SetFloat("pointLight.linear", this->linear);
		shader.SetFloat("pointLight.quadratic", this->quadratic);
	}
};

#endif // !LIGHT_H
