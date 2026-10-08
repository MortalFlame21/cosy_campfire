#include <stdexcept> // std::runtime_error

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Input.hpp"

Window::Window(std::string_view title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, title.data(), nullptr, nullptr);
	if (!_window) 
		throw std::runtime_error{"Error: (ctor) Window: Failed to open window!\n"};
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);

	glfwSetFramebufferSizeCallback(_window, Window::callback);
	glfwSetKeyCallback(_window, Keyboard::callback);
	glfwSetCursorPosCallback(_window, MouseCursor::callback);
	glfwSetScrollCallback(_window, MouseScroll::callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error{"Error: (ctor) Window: Failed to load GLAD!\n"};

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
	
	if (auto* self{static_cast<Window*>(glfwGetWindowUserPointer(window))}) {
		self->_width = w;
		self->_height = h;
	}
}
