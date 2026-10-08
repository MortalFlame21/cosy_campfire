#pragma once

#include <string_view> // std::string_view

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
	static constexpr auto WIDTH{1280.f};
	static constexpr auto HEIGHT{1024.f};

	explicit Window(std::string_view title);

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	~Window();

	GLFWwindow* data() const { return _window; }
	int height() const { return _height; }
	int width() const { return _width; }

	static constexpr float aspectRatio() { return WIDTH / HEIGHT; }
	bool running() const { return !glfwWindowShouldClose(_window); }
private:
	static void callback(GLFWwindow* window, int w, int h);

	GLFWwindow* _window{};
	int _width{static_cast<int>(WIDTH)};
	int _height{static_cast<int>(HEIGHT)};
};
