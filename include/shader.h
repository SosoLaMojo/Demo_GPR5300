#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>

namespace gl {

	class Shader
	{
	public:
		unsigned int id;
		// constructor generates the shader on the fly
		Shader(
			const std::string& vertexPath,
			const std::string& fragmentPath,
			const std::string& geometryPath = "");
		// activate the shader
		void Use();
		// utility uniform functions
		void SetBool(const std::string& name, bool value) const;

		void SetInt(const std::string& name, int value) const;

		void SetFloat(const std::string& name, float value) const;

		void SetVec2(const std::string& name, const glm::vec2& value) const;

		void SetVec2(const std::string& name, float x, float y) const;

		void SetVec3(const std::string& name, const glm::vec3& value) const;

		void SetVec3(const std::string& name, float x, float y, float z) const;

		void SetVec4(const std::string& name, const glm::vec4& value) const;

		void SetVec4(
			const std::string& name,
			float x, float y, float z, float w);

		void SetMat2(const std::string& name, const glm::mat2& mat) const;

		void SetMat3(const std::string& name, const glm::mat3& mat) const;

		void SetMat4(const std::string& name, const glm::mat4& mat) const;

	private:
		// utility function for checking shader compilation/linking errors.
		void CheckCompileErrors(GLuint shader, std::string type);
		
		// TODO move this function somewhere else, STATIC function, for testing class and not instance
		void IsError(const char* file, int line) const;
	};

} // End namespace gl.
