#include "camera.h"

namespace gl {
	
	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM)
	{
		this->position = position;
		this->world_up = up;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM)
	{
		position = glm::vec3(posX, posY, posZ);
		world_up = glm::vec3(upX, upY, upZ);
		yaw = yaw;
		pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(position, position + front, up);
	}

	void Camera::ProcessKeyboard(CameraMovementEnum direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == CameraMovementEnum::FORWARD)
			position += front * velocity;
		if (direction == CameraMovementEnum::BACKWARD)
			position -= front * velocity;
		if (direction == CameraMovementEnum::LEFT)
			position -= right * velocity;
		if (direction == CameraMovementEnum::RIGHT)
			position += right * velocity;
	}

	void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get
		// flipped
		if (constrainpitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		// update front, right and up Vectors using the updated Euler
		// angles
		updateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		Zoom -= yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void Camera::updateCameraVectors()
	{
		// calculate the new front vector
		glm::vec3 front_;
		front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front_.y = sin(glm::radians(pitch));
		front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front_);
		// also re-calculate the right and up vector
		right = glm::normalize(glm::cross(front, world_up));
		// normalize the vectors, because their length gets closer to 0 the
		// more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, front));
	}
	
} // namespace gl