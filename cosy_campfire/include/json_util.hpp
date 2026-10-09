#include <string_view>

#include <nlohmann/json.hpp>

#include "GameObject.hpp"

namespace bb {
template<int N>
requires (N == 2 || N == 3 || N == 4)
inline auto to_vec(const nlohmann::json& j, std::string_view k) {
	if constexpr (N == 2)
		return glm::vec2{j[k][0], j[k][1]};
	else if constexpr (N == 3)
		return glm::vec3{j[k][0], j[k][1], j[k][2]};
	else
		return glm::vec4{j[k][0], j[k][1], j[k][2], j[k][3]};
}

template<typename T>
requires std::same_as<T, glm::vec2> || std::same_as<T, glm::vec3> || std::same_as<T, glm::vec4>
inline nlohmann::json to_json(T v) {
	if constexpr (std::same_as<T, glm::vec2>)
		return {v.x, v.y};
	else if constexpr (std::same_as<T, glm::vec3>)
		return {v.x, v.y, v.z};
	else if constexpr (std::same_as<T, glm::vec4>)
		return {v.x, v.y, v.z, v.w};
}
} // namespace bb

// https://json.nlohmann.me/api/basic_json/json_serializer/#examples
namespace nlohmann {
template<> 
struct adl_serializer<GameObject> {
    static GameObject from_json(const nlohmann::json& j) {
		GameObject obj{};
		obj.model(std::move(Model{j["model"]}));
		obj.position(bb::to_vec<3>(j, "position"));
		obj.rotation(bb::to_vec<3>(j, "rotation"));
		obj.scale(bb::to_vec<3>(j, "scale"));
        return obj;
    }

    static void to_json(nlohmann::json& j, const GameObject& o) {
		j["model"]    = o.model().path();
		j["position"] = bb::to_json(o.position());
		j["rotation"] = bb::to_json(o.rotation());
		j["scale"]    = bb::to_json(o.scale());
    }
};

template<> 
struct adl_serializer<PointLight> {
    static PointLight from_json(const nlohmann::json& j) {
		return {
			.position  = bb::to_vec<3>(j, "position"),
			.color     = bb::to_vec<3>(j, "color"),
			.ambient   = bb::to_vec<3>(j, "ambient"),
			.diffuse   = bb::to_vec<3>(j, "diffuse"),
			.specular  = bb::to_vec<3>(j, "specular"),
			.constant  = j["constant"],
			.linear    = j["linear"],
			.quadratic = j["quadratic"],
		};
    }

    static void to_json(nlohmann::json& j, const PointLight& p) {
		j["position"]  = bb::to_json(p.position);
		j["color"]     = bb::to_json(p.color);
		j["ambient"]   = bb::to_json(p.ambient);
		j["diffuse"]   = bb::to_json(p.diffuse);
		j["specular"]  = bb::to_json(p.specular);
		j["constant"]  = p.constant;
		j["linear"]    = p.linear;
		j["quadratic"] = p.quadratic;
    }
};
} // namespace nlohmann