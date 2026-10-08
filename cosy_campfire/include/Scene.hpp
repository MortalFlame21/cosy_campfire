#pragma once

#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Camera.hpp"
#include "NamedObjects.hpp"
#include "GameObject.hpp"

class Scene {
public:
	Scene() {
		using namespace nlohmann;

		std::ifstream ifs("resources/scene.json");
		json data{json::parse(ifs)};
		
		for (const auto& o : data["objects"]) {
			_objects.push_back(std::move(o.get<GameObject>()));
		}
	}

	auto& objects() { return _objects; }
	auto& light() { return _dir_light; }

	void save() {
		using namespace nlohmann;
		json data{};
		for (const auto& o : _objects) {
			data["objects"].push_back(o);
		}
		std::ofstream ofs("resources/scene.json");
		ofs << data.dump(1, ' ');
	}
private:
	std::vector<GameObject> _objects{};
	DirectionalLight _dir_light{
		.color     = glm::vec3{1.f},
		.direction = glm::vec3{0.f, -1.f, 0.f},
		.ambient   = glm::vec3{0.2f},
		.diffuse   = glm::vec3{0.5f},
		.specular  = glm::vec3{1.f},
	};
};