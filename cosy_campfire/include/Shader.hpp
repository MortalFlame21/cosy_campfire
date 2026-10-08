#pragma once

#include <string> // std::string
#include <string_view> // std::string_view
#include <sstream> // std::stringstream
#include <stdexcept> // std::runtime_error
#include <fstream> // std::ifstream
#include <type_traits> // std::same_as
#include <format> // std::format

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(std::string_view source_path, GLenum type)
		: _id{glCreateShader(type)}
	{
		std::ifstream fs{source_path.data()};
		if (!fs)
			throw std::runtime_error{std::format("Error: path: {} does not exist!", source_path)};
		std::stringstream ss{};
		ss << fs.rdbuf();

		const auto src_str{ss.str()};
		const auto src_cstr{src_str.c_str()};

		glShaderSource(_id, 1, &src_cstr, nullptr);
		compile();
	}

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) = delete;

	~Shader() { glDeleteShader(_id); }

	GLuint id() const { return _id; }
private:
	GLuint _id{};

	void compile() const {
		glCompileShader(_id);

		GLint success{};
		glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLint length{};
			glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);

			auto log{std::format("Error: Compile: Shader ({}): {}", _id, std::string(length, '\0'))};
			glGetShaderInfoLog(_id, length, nullptr, log.data() + log.size() - length);

			throw std::runtime_error{log};
		}
	}
};

class ShaderProgram {
public:
	ShaderProgram() : _id{glCreateProgram()} {
		if (!_id) throw std::runtime_error{"Failed to create program."};
	};

	template <typename... Shaders>
	ShaderProgram(const Shaders&... shaders) 
		: ShaderProgram{} 
	{
		(attach(shaders), ...);
		link();
	}

	ShaderProgram(ShaderProgram&& o) noexcept : _id{o._id} { o._id = 0; }
	ShaderProgram& operator=(ShaderProgram&& o) noexcept {
		if (this != &o) {
			if (_id) glDeleteProgram(_id);
			_id = o._id;
			o._id = 0;
		}
		return *this;
	}

	~ShaderProgram() { glDeleteProgram(_id); }

	void use() const { glUseProgram(_id); }
	void disuse() const { glUseProgram(0); }
	void attach(const Shader& shader) const { glAttachShader(_id, shader.id()); }
	void link() const {
		glLinkProgram(_id);

		GLint success{};
		glGetProgramiv(_id, GL_LINK_STATUS, &success);
		if (!success) {
			GLint length{};
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

			std::string log(length, '\0');
			glGetProgramInfoLog(_id, length, nullptr, log.data());

			throw std::runtime_error{log};
		}
	}

	template <typename... T>
	requires std::is_arithmetic_v<std::tuple_element_t<0, std::tuple<T...>>>
		&& (sizeof...(T) <= 4)
	void setUniformV(std::string_view name, T... args) const {
		using FirstT = std::tuple_element_t<0, std::tuple<T...>>;
		const auto uniformLocation{glGetUniformLocation(_id, name.data())};
		constexpr auto sz{sizeof...(args)};

		if constexpr (std::is_integral_v<FirstT>) {
			if constexpr (sz == 1)
				glUniform1i(uniformLocation, args...);
			else if constexpr (sz == 2)
				glUniform2i(uniformLocation, args...);
			else if constexpr (sz == 3)
				glUniform3i(uniformLocation, args...);
			else if constexpr (sz == 4)
				glUniform4i(uniformLocation, args...);
		}
		else if constexpr (std::is_floating_point_v<FirstT>) {
			if constexpr (sz == 1)
				glUniform1f(uniformLocation, args...);
			else if constexpr (sz == 2)
				glUniform2f(uniformLocation, args...);
			else if constexpr (sz == 3)
				glUniform3f(uniformLocation, args...);
			else if constexpr (sz == 4)
				glUniform4f(uniformLocation, args...);
		}
	}

	template <typename T>
	requires std::same_as<T, glm::vec2>
		|| std::same_as<T, glm::vec3>
		|| std::same_as<T, glm::vec4>
	void setUniformV(std::string_view name, T v) const {
		const auto uniformLocation{glGetUniformLocation(_id, name.data())};

		if constexpr (std::same_as<T, glm::vec2>)
			glUniform2f(uniformLocation, v.x, v.y);
		else if constexpr (std::same_as<T, glm::vec3>)
			glUniform3f(uniformLocation, v.x, v.y, v.z);
		else if constexpr (std::same_as<T, glm::vec4>)
			glUniform4f(uniformLocation, v.x, v.y, v.z, v.w);
	}


	template <typename T>
	requires std::same_as<T, glm::mat2>
		|| std::same_as<T, glm::mat3>
		|| std::same_as<T, glm::mat4>
	void setUniformM(std::string_view name, GLsizei count, GLboolean transpose, const T& matrix) const {
		const auto uniformLocation{glGetUniformLocation(_id, name.data())};

		if constexpr (std::same_as<T, glm::mat2>)
			glUniformMatrix2fv(uniformLocation, count, transpose, glm::value_ptr(matrix));
		else if constexpr (std::same_as<T, glm::mat3>)
			glUniformMatrix3fv(uniformLocation, count, transpose, glm::value_ptr(matrix));
		else if constexpr (std::same_as<T, glm::mat4>)
			glUniformMatrix4fv(uniformLocation, count, transpose, glm::value_ptr(matrix));
	}
private:
	GLuint _id{};
};
