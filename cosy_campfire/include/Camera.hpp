#pragma once

#include <memory>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Input.hpp"

class Camera {
public:
	static Camera& instance() {
		static Camera singleton{};
		return singleton;
	}

	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(Camera&&) = delete;

	void onKeyAction(GLFWwindow* window, float dt);
	void onCursorAction(GLFWwindow* window, const glm::vec2& curr_pos, const glm::vec2& prev_pos);
	void onScrollAction(GLFWwindow* window, const glm::vec2& offset);

	glm::vec3& position() { return _position; }
	const glm::vec3& position() const { return _position; }

	glm::vec3& front() { return _front; }
	const glm::vec3& front() const { return _front; }

	glm::vec3& up() { return _up; }
	const glm::vec3& up() const { return _up; }

	float near() const { return _near; }
	float far() const { return _far; }

	float fov() const { return _fov; }

	float yaw() const { return _yaw; }
	float pitch() const { return _pitch; }

	glm::mat4 lookAt() const { return glm::lookAt(_position, _position + _front, _up); }
private:
	static inline constexpr auto SPEED{2.5f};

	Camera() = default;

	glm::vec3 _position{};
	glm::vec3 _front{0.f, 0.f, -1.f};
	glm::vec3 _up{0.f, 1.f, 0.f};
	float _near{0.1f};
	float _far{100.f};
	float _fov{90.f};
	float _pitch{};
	float _yaw{-90.f};
};
