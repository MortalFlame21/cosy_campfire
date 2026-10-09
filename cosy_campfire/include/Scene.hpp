#pragma once

#include <iostream>
#include <fstream>

#include "Camera.hpp"
#include "NamedObjects.hpp"
#include "GameObject.hpp"
#include "json_util.hpp"

class Scene {
public:
	Scene() {
		using namespace nlohmann;

		std::ifstream ifs("resources/scene.json");
		json data{json::parse(ifs)};
		
		for (const auto& o : data["objects"]) {
			_objects.push_back(std::move(o.get<GameObject>()));
		}

		for (const auto& o : data["point_lights"]) {
			_point_lights.push_back(std::move(o.get<PointLight>()));
		}

		_directional_light = std::move(data["directional_light"].get<DirectionalLight>());
	}

	auto& objects() { return _objects; }
	auto& directional_light() { return _directional_light; }
	auto& point_lights() { return _point_lights; }

	void save() {
		using namespace nlohmann;
		json data{};
		for (const auto& o : _objects)
			data["objects"].push_back(o);
		for (const auto& pl : _point_lights)
			data["point_lights"].push_back(pl);
		// don't need to save directional light atm
		std::ofstream ofs("resources/scene.json");
		ofs << data.dump(1, ' ');
	}
private:
	std::vector<GameObject> _objects{};
	DirectionalLight _directional_light{};
	std::vector<PointLight> _point_lights{};
};