#pragma once

#include "Scene.hpp"
#include "Shader.hpp"
#include "UIManager.hpp"

class Renderer {
public:
	static void render(Scene& scene, Camera& camera, UIManager& ui) {
		ShaderManager::get("default").use();

		const auto projection{glm::perspective(
			glm::radians(camera.fov()), 
			Window::aspectRatio(), 
			camera.near(),
			camera.far()
		)};

		ShaderManager::get("default").setUniformM("u_view", 1, GL_FALSE, camera.lookAt());
		ShaderManager::get("default").setUniformM("u_projection", 1, GL_FALSE, projection);
		ShaderManager::get("default").setUniformV("u_camera_position", camera.position());
		
		ShaderManager::get("default").setUniformV("u_dir_light.direction", scene.light().direction);
		ShaderManager::get("default").setUniformV("u_dir_light.ambient", scene.light().ambient);
		ShaderManager::get("default").setUniformV("u_dir_light.diffuse", scene.light().diffuse);
		ShaderManager::get("default").setUniformV("u_dir_light.specular", scene.light().specular);

		for (auto& o : scene.objects()) {
			o.draw(ShaderManager::get("default"));
		}

		ui.draw(scene);

		ShaderManager::get("default").disuse(); // explicit disuse, remember draw calls disuse! 
	}
};