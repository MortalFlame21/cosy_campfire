#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "Model.hpp" 
#include "Shader.hpp"

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

// https://json.nlohmann.me/api/basic_json/json_serializer/#examples
namespace nlohmann {
template<> 
struct adl_serializer<GameObject> {
    static GameObject from_json(const nlohmann::json& j) {
		GameObject obj{};
		obj.model(std::move(Model{j["m"]}));
		obj.position({j["p"][0], j["p"][1], j["p"][2]});
		obj.rotation({j["r"][0], j["r"][1], j["r"][2]});
		obj.scale({j["s"][0], j["s"][1], j["s"][2]});
        return obj;
    }

    static void to_json(nlohmann::json& j, const GameObject& p) {
		j["m"] = p.model().path();
		j["p"] = {p.position().x, p.position().y, p.position().z};
		j["r"] = {p.rotation().x, p.rotation().y, p.rotation().z};
		j["s"] = {p.scale().x, p.scale().y, p.scale().z};
    }
};
} // namespace nlohmann