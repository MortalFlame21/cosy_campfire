#pragma once

#include <glm/glm.hpp>

#include "Model.hpp" 
#include "Shader.hpp"

struct DirectionalLight {
	glm::vec3 color{};
	glm::vec3 direction{};
	glm::vec3 ambient{};
	glm::vec3 diffuse{};
	glm::vec3 specular{};
};

struct PointLight {
	glm::vec3 color{};
	glm::vec3 position{};
	glm::vec3 ambient{};
	glm::vec3 diffuse{};
	glm::vec3 specular{};
	float constant{};
	float linear{};
	float quadratic{};
};

class GameObject {
public:
	void draw(const ShaderProgram& shader) const {
		shader.use();
		shader.setUniformM("u_model", 1, GL_FALSE, modelMatrix());
		_model.draw(shader);
		shader.disuse();
	}

	auto& model() { return _model; }
	auto& model() const { return _model; }
	void model(Model model) { _model = std::move(model); }

	auto& position() { return _position; }
	auto& position() const { return _position; }
	void position(glm::vec3 position) { _position = position; }

	auto& scale() { return _scale; }
	auto& scale() const { return _scale; }
	void scale(glm::vec3 scale) { _scale = scale; }

	auto& rotation() { return _rotation; }
	auto& rotation() const { return _rotation; }
	void rotation(glm::vec3 rotation) { _rotation = rotation; }

	glm::mat4 modelMatrix() const {
		glm::mat4 m{1.f};
		m = glm::translate(m, _position);
		m = glm::rotate(m, glm::radians(_rotation.x), {1, 0, 0});
		m = glm::rotate(m, glm::radians(_rotation.y), {0, 1, 0});
		m = glm::rotate(m, glm::radians(_rotation.z), {0, 0, 1});
		m = glm::scale(m, _scale);
		return m;
	}
private:
	Model _model{};
	glm::vec3 _position{};
	glm::vec3 _scale{1.f};
	glm::vec3 _rotation{};
};
