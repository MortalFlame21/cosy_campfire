#pragma once

#include <string_view> // std::string_view
#include <stdexcept> // std::runtime_error
#include <format> // std::format

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <assimp/material.h>

class Texture {
public:
	Texture() {
		glGenTextures(1, &_id); // generate and bind
		glBindTexture(GL_TEXTURE_2D, _id);
		// default settings
		setParam(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set up texture wrapping for s
		setParam(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // set up texture wrapping for t
		setParam(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // level of filtering for MINimising
		setParam(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // level of filtering for MAXimising
	}

	explicit Texture(std::string_view path) : Texture{} {
		load(path);
		unbind();
	}

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

	Texture(Texture&& o) noexcept
		: _id{o._id}
		, _width{o._width}
		, _height{o._height}
		, _numColorChannels{o._numColorChannels}
		, _type{o._type}
	{
		o._id = 0;
		o._width = 0;
		o._height = 0;
		o._numColorChannels = 0;
		o._type = aiTextureType_NONE;
	}

	Texture& operator=(Texture&& o) noexcept {
		if (this != &o) {
			glDeleteTextures(1, &_id);

			_id = o._id;
			_width = o._width;
			_height = o._height;
			_numColorChannels = o._numColorChannels;
			_type = o._type;

			o._id = 0;
			o._width = 0;
			o._height = 0;
			o._numColorChannels = 0;
			o._type = aiTextureType_NONE;
		}
		return *this;
	}

	~Texture() { glDeleteTextures(1, &_id); }

	void setParam(GLenum target, GLenum name, GLint parameter) const {
		glTexParameteri(target, name, parameter);
	}

	void load(std::string_view path) {
		auto* data{stbi_load(path.data(), &_width, &_height, &_numColorChannels, STBI_rgb_alpha)};
		if (!data) throw std::runtime_error{std::format("Error: Texture ({}): Failed to load texture data", path)};
		// load texture on gpu
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// generate mipmap of current texture
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	void bind(GLuint slot = 0) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	void unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint id() const { return _id; }

	int width() const { return _width; }

	int height() const { return _height; }

	void type(aiTextureType type) { _type = type; }
	aiTextureType type() const { return _type; }

private:
	GLuint _id{};
	int _width{};
	int _height{};
	int _numColorChannels{};
	aiTextureType _type{};
};

inline Texture makeTexture(std::string_view path, aiTextureType type = aiTextureType_NONE) {
	Texture t{path};
	t.type(type);
	return t;
}