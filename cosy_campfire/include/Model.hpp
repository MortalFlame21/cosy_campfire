#pragma once

#include <format> // std::format
#include <vector> // std::vector
#include <string> // std::string_view
#include <stdexcept> // std::runtime_error
#include <span> // std::span

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"

class Model {
public:
	Model() = default;

	explicit Model(std::string path) : _directory{path.substr(0, path.find_last_of('/'))}, _path{path} {
		Assimp::Importer importer{};
		constexpr auto flags{aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices};
		const auto* scene{importer.ReadFile(path.data(), flags)};
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			throw std::runtime_error{std::format("Error: (ctor) Model: Failed to load model ({})!", _path)};
		process(scene->mRootNode, scene);
	}

	void draw(const ShaderProgram& shader) const {
		shader.use();
		for (const auto& mesh : _meshes) {
			mesh.draw(shader);
		}
		shader.disuse();
	}

	auto path() const { return _path; }

private:
	void process(const aiNode* node, const aiScene* scene) {
		// process node meshes
		std::span node_meshes{node->mMeshes, node->mNumMeshes};
		for (const auto& mesh_node : node_meshes) {
			const auto mesh_scene{scene->mMeshes[mesh_node]};
			_meshes.push_back(process(mesh_scene, scene));
		}
		// process children nodes
		std::span node_children{node->mChildren, node->mNumChildren};
		for (const auto& child : node_children) {
			process(child, scene);
		}
	}

	Mesh process(const aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices(mesh->mNumVertices, {});
		std::vector<GLuint> indices{};
		std::vector<std::string> textures{};
		float shininess{};

		// process vertices
		for (std::size_t i{}; i < mesh->mNumVertices; ++i) {
			const auto v{mesh->mVertices[i]};
			const auto n{mesh->mNormals[i]};
			const auto t{[&](){
				const auto tc{mesh->mTextureCoords[0]};
				return (tc) ? glm::vec2{tc[i].x, tc[i].y} : glm::vec2{};
			}()};

			Vertex vertex {
				.position  = {v.x, v.y, v.z},
				.normal    = {n.x, n.y, n.z},
				.tex_coord = t
			};
			vertices[i] = std::move(vertex);
		}

		// process indices for each face
		for (std::size_t i{}; i < mesh->mNumFaces; ++i) {
			const auto face{mesh->mFaces[i]};
			std::span face_indices{face.mIndices, face.mNumIndices};
			indices.insert(indices.end(), face_indices.begin(), face_indices.end());
		}

		// load da textures
		if (mesh->mMaterialIndex >= 0) {
			auto* material{scene->mMaterials[mesh->mMaterialIndex]};
			auto d_maps{loadTextures(material, aiTextureType_DIFFUSE)};
			auto s_maps{loadTextures(material, aiTextureType_SPECULAR)};
			auto e_maps{loadTextures(material, aiTextureType_EMISSIVE)};
			textures.insert(textures.end(), std::make_move_iterator(d_maps.begin()), std::make_move_iterator(d_maps.end()));
			textures.insert(textures.end(), std::make_move_iterator(s_maps.begin()), std::make_move_iterator(s_maps.end()));
			textures.insert(textures.end(), std::make_move_iterator(e_maps.begin()), std::make_move_iterator(e_maps.end()));
			shininess = material->Get(AI_MATKEY_SHININESS, shininess); // is this even right ??
		}

		return {vertices, indices, textures, shininess};
	}

	std::vector<std::string> loadTextures(aiMaterial* material, aiTextureType type) {
		std::vector<std::string> textures{};
		for (std::size_t i{}; i < material->GetTextureCount(type); ++i) {
			aiString tex_path{}; // path of texture will be used as name/id
			material->GetTexture(type, i, &tex_path);

			// load new texture
			if (!TextureManager::contains(tex_path.C_Str())) {
				auto t{makeTexture(std::format("{}/{}", _directory, tex_path.C_Str()), type)};
				TextureManager::load(tex_path.C_Str(), std::move(t));
			}
			textures.push_back(tex_path.C_Str());
		}
		// dumbass hack
		if ((type == aiTextureType_SPECULAR || type == aiTextureType_EMISSIVE) && material->GetTextureCount(type) == 0) {
			textures.push_back((type == aiTextureType_SPECULAR) ? "specular_none" : "emission_none");
		}
		return textures;
	}

	std::vector<Mesh> _meshes{};
	std::string _directory{};
	std::string _path{};
};