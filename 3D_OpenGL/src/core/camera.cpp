#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
	: worldUp(worldUp), position(position), up(worldUp), right(glm::vec3(0.f)), 
	pitch(0.f), yaw(-90.f), roll(0.f)
{
	this->ViewMatrix = glm::mat4(1.f);

	this->movementSpeed = 10.f;
	this->sensitivity = 30.f;

	// Set the front vector to the normalized input direction
	this->front = glm::normalize(direction);

	// Calculate yaw and pitch from the direction vector
	this->yaw = glm::degrees(atan2(front.z, front.x)) - 90.f;  // because yaw=0 faces +X
	this->pitch = glm::degrees(asin(front.y));

	this->UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front.y = sin(glm::radians(this->pitch));
	this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	this->front = glm::normalize(this->front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

const glm::mat4 Camera::GetViewMatrix()
{
	this->UpdateCameraVectors();
	this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);

	return this->ViewMatrix;
}

const glm::vec3 Camera::GetPosition() const
{
	return this->position;
}

void Camera::UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
	this->UpdateMouseInput(dt, offsetX, offsetY);
}

void Camera::Move(const float& dt, const int direction, float terrainY) {
	//glm::vec3 forwardXZ = glm::normalize(glm::vec3(this->front.x, 0.f, this->front.z));
	//glm::vec3 rightXZ = glm::normalize(glm::vec3(this->right.x, 0.f, this->right.z));

	switch (direction) {
	case FORWARD:
		this->position += this->front * this->movementSpeed * dt;
		break;
	case BACK:
		this->position -= this->front * this->movementSpeed * dt;
		break;
	case LEFT:
		this->position -= this->right * this->movementSpeed * dt;
		break;
	case RIGHT:
		this->position += this->right * this->movementSpeed * dt;
		break;
	default:
		break;
	}

	//this->position.y = 4.f + terrainY;
}

void Camera::UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
	// Update pitch, yaw and roll
	this->pitch -= static_cast<GLfloat>(offsetY) * this->sensitivity * dt;
	this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * dt;

	if (this->pitch > 80.f) {
		this->pitch = 80.f;
	}
	else if (this->pitch < -80.f) {
		this->pitch = -80.f;
	}

	if (this->yaw > 360.f || this->yaw < -360.f) {
		this->yaw = 0.f;
	}
}

void Camera::CameraChanged(glm::vec3 position, glm::vec3 direction)
{
	this->position = position;
	this->ViewMatrix = glm::mat4(1.f);

	this->movementSpeed = 10.f;
	this->sensitivity = 30.f;

	// Set the front vector to the normalized input direction
	this->front = glm::normalize(direction);

	// Calculate yaw and pitch from the direction vector
	this->yaw = glm::degrees(atan2(front.z, front.x)) - 90.f;  // because yaw=0 faces +X
	this->pitch = glm::degrees(asin(front.y));

	this->UpdateCameraVectors();
}
