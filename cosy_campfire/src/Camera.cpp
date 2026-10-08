#include <algorithm> // std::clamp
#include <cmath> // std::sin, std::cos
#include <iostream>

#include "Camera.hpp"

void Camera::onKeyAction(GLFWwindow* window, float dt) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_position += SPEED * dt * _front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_position -= SPEED * dt * glm::normalize(glm::cross(_front, _up));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_position -= SPEED * dt * _front;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_position += SPEED * dt * glm::normalize(glm::cross(_front, _up));
}

void Camera::onCursorAction(GLFWwindow* window, const glm::vec2& curr_pos, const glm::vec2& prev_pos) {
	if (auto mode{glfwGetInputMode(window, GLFW_CURSOR)}; mode == GLFW_CURSOR_NORMAL)
		return;

	constexpr auto sensitivity{.1f};
	const auto x_offset{(curr_pos.x - prev_pos.x) * sensitivity};
	const auto y_offset{(prev_pos.y - curr_pos.y) * sensitivity};

	_yaw += x_offset;
	_pitch = std::clamp(_pitch + y_offset, -89.f, 89.f);

	const glm::vec3 direction {
		std::cosf(glm::radians(_yaw)) * std::cosf(glm::radians(_pitch)),
		std::sinf(glm::radians(_pitch)),
		std::sinf(glm::radians(_yaw)) * std::cosf(glm::radians(_pitch))
	};

	_front = glm::normalize(direction);
}

void Camera::onScrollAction(GLFWwindow* window, const glm::vec2& offset) {
	constexpr auto multiplier{5.f};
	_fov = std::clamp(_fov - (offset.y * multiplier), 1.f, 90.f);
}
