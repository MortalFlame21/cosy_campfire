#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "Camera.hpp"

void Keyboard::callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto pressed{[&](auto k) { return key == k && action == GLFW_PRESS; }};

    if (pressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if (int mode[2]{}; pressed(GLFW_KEY_TAB)) {
		glGetIntegerv(GL_POLYGON_MODE, mode);
		glPolygonMode(GL_FRONT_AND_BACK, (mode[0] == GL_LINE) ? GL_FILL : GL_LINE);
	}

	if (auto mode{glfwGetInputMode(window, GLFW_CURSOR)}; pressed(GLFW_KEY_Q)) {
		auto m{(mode == GLFW_CURSOR_DISABLED) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED};
		glfwSetInputMode(window, GLFW_CURSOR, m);
	}
}

void MouseCursor::callback(GLFWwindow* window, double x_pos, double y_pos) {
	const glm::vec2 curr_pos{x_pos, y_pos};

	if (static auto firstRun{true}; firstRun) {
		_prev_pos = curr_pos;
		firstRun = false;
	}

	Camera::instance().onCursorAction(window, curr_pos, _prev_pos);
	_prev_pos = curr_pos;
}

void MouseScroll::callback(GLFWwindow* window, double x_offset, double y_offset) {
	if (auto mode{glfwGetInputMode(window, GLFW_CURSOR)}; mode == GLFW_CURSOR_NORMAL) return;
	Camera::instance().onScrollAction(window, {x_offset, y_offset});
}
