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
		
		ShaderManager::get("default").setUniformV("u_directional_light.direction", scene.directional_light().direction);
		ShaderManager::get("default").setUniformV("u_directional_light.ambient",   scene.directional_light().ambient);
		ShaderManager::get("default").setUniformV("u_directional_light.diffuse",   scene.directional_light().diffuse);
		ShaderManager::get("default").setUniformV("u_directional_light.specular",  scene.directional_light().specular);

		for (const auto& l : scene.point_lights()) {
			ShaderManager::get("default").setUniformV("u_point_light.position",  l.position);
			ShaderManager::get("default").setUniformV("u_point_light.color",     l.color);
			ShaderManager::get("default").setUniformV("u_point_light.ambient",   l.ambient);
			ShaderManager::get("default").setUniformV("u_point_light.diffuse",   l.diffuse);
			ShaderManager::get("default").setUniformV("u_point_light.specular",  l.specular);
			ShaderManager::get("default").setUniformV("u_point_light.constant",  l.constant);
			ShaderManager::get("default").setUniformV("u_point_light.linear",    l.linear);
			ShaderManager::get("default").setUniformV("u_point_light.quadratic", l.quadratic);
		}

		for (auto& o : scene.objects()) {
			o.draw(ShaderManager::get("default"));
		}

		ui.draw(scene);

		ShaderManager::get("default").disuse(); // explicit disuse, remember draw calls disuse! 
	}
};