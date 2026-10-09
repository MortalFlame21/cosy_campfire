#pragma once

#include <unordered_map>
#include <string>

#include <glad/glad.h>

#include "Shader.hpp"
#include "Texture.hpp"

class ShaderManager {
public:
    static void init() {
        const Shader vert{"resources/shaders/shader.vert", GL_VERTEX_SHADER};
        const Shader frag{"resources/shaders/shader.frag", GL_FRAGMENT_SHADER};

		load("default", {vert, frag});
    }

	static void load(const std::string& name, ShaderProgram&& shader) {
		_shaders[name] = std::move(shader);
	}

	static ShaderProgram& get(const std::string& name) {
		return _shaders.at(name);
	}

	static bool contains(const std::string& name) {
		return _shaders.contains(name);
	}
private:
	static inline std::unordered_map<std::string, ShaderProgram> _shaders{};
};

class TextureManager {
public:
    static void init() {
		load("specular_none", makeTexture("resources/textures/black.png", aiTextureType_SPECULAR));
		load("emission_none", makeTexture("resources/textures/black.png", aiTextureType_EMISSIVE));
	}

	static void load(const std::string& name, Texture&& texture) {
		_textures[name] = std::move(texture);
	}

	static Texture& get(const std::string& name) {
		return _textures.at(name);
	}

	static bool contains(const std::string& name) {
		return _textures.contains(name);
	}
private:
	static inline std::unordered_map<std::string, Texture> _textures{};
};

