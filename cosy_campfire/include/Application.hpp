#pragma once

#include "Window.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "UIManager.hpp"

class Application {
public:
	Application();
	void run();
private:
	void update(float dt);
	void render(float dt);
	void poll();

	Window _window{"Cosy Campfire"};
	Scene _scene{};
	UIManager _ui{_window};
};

