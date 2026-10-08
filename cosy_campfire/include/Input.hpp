#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Keyboard {
public:
	static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

class MouseCursor {
public:
	static void callback(GLFWwindow* window, double x_pos, double y_pos);
private:
	static inline glm::vec2 _prev_pos{};
};

class MouseScroll {
public:
	static void callback(GLFWwindow* window, double x_offset, double y_offset);
};

class Input {
public:
	Keyboard keys{};
	MouseCursor cursor{};
	MouseScroll scroll{};
};