#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum direction
{
	FORWARD = 0,
	BACK,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::mat4 ViewMatrix;

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	void UpdateCameraVectors();
public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp);
	~Camera() {}

	const glm::mat4 GetViewMatrix();
	const glm::vec3 GetPosition() const;

	void UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY);
	void Move(const float& dt, const int direction, float terrainY);
	void UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY);
};

#endif // !CAMERA_H