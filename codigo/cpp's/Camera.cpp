#include "Camera.h"
#include <cmath>

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	moveSpeed = 3.0f;
	turnSpeed = 0.3f;

	update();
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::keyControlXZ(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 flatRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));

	if (keys[GLFW_KEY_W])
	{
		position += flatFront * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= flatFront * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= flatRight * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += flatRight * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::setPosition(glm::vec3 pos)
{
	position = pos;
}

void Camera::setDirection(GLfloat yaw, GLfloat pitch)
{
	this->yaw = yaw;
	this->pitch = pitch;

	if (this->pitch > 89.0f)
	{
		this->pitch = 89.0f;
	}

	if (this->pitch < -89.0f)
	{
		this->pitch = -89.0f;
	}

	update();
}

void Camera::lookAt(glm::vec3 target)
{
	front = glm::normalize(target - position);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}