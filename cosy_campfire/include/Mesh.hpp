#pragma once

#include <glad/glad.h>

#include <vector> // std::vector
#include <string> // std::string
#include <cstddef> // offsetof

#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "NamedObjects.hpp"
#include "Shader.hpp"

class Mesh {
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<std::string>& textures, float shininess)
		: _vertices{std::move(vertices)}
		, _indices{std::move(indices)}
		, _textures{std::move(textures)}
		, _vbo{_vertices}
		, _ebo{_indices}
		, _shininess{shininess}
	{
		_vao.bind();
		_vbo.bind();
		_ebo.bind();

		_vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		_vao.enableVertexAttribArray(0);
		_vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
		_vao.enableVertexAttribArray(1);
		_vao.vertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tex_coord)));
		_vao.enableVertexAttribArray(2);
	}

	void draw(const ShaderProgram& shader) const {
		shader.use();

		shader.setUniformV("u_material.shininess", _shininess);
		for (std::size_t i{}, d_num{1}, s_num{1}, e_num{1}; i < _textures.size(); ++i) {
			auto& t{TextureManager::get(_textures[i])};
			t.bind(i);
			const auto type{(t.type() == aiTextureType_DIFFUSE) ? "diffuse" : (t.type() == aiTextureType_SPECULAR) ? "specular" : "emission"};
			const auto n{((t.type() == aiTextureType_DIFFUSE) ?  d_num : (t.type() == aiTextureType_SPECULAR) ? s_num : e_num)++};
			shader.setUniformV(std::format("u_material.{}{}", type, n), i);
		}
		_vao.bind();
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
		_vao.unbind();

		shader.disuse();
	}

private:
	std::vector<Vertex> _vertices{};
	std::vector<GLuint> _indices{};
	std::vector<std::string> _textures{};
	float _shininess{};

	Vao _vao{};
	Vbo _vbo{};
	Ebo _ebo{};
};