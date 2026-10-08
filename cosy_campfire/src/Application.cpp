#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Application.hpp"
#include "ResourceManager.hpp"
#include "Renderer.hpp"

Application::Application() {
	glEnable(GL_DEPTH_TEST);

	TextureManager::init();
	ShaderManager::init();
}

void Application::run() {
	float delta_time{};
	float prev_time{};

	while (_window.running()) {
		auto curr_time{static_cast<float>(glfwGetTime())};
		delta_time = curr_time - prev_time;
		prev_time = curr_time;

		update(delta_time);
		render(delta_time);
		poll();
	}
}

void Application::update(float dt) {
	Camera::instance().onKeyAction(_window.data(), dt);
}

void Application::render(float dt) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer::render(_scene, Camera::instance(), _ui);
}

void Application::poll() {
	glfwSwapBuffers(_window.data());
	glfwPollEvents();
}
