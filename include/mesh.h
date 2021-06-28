#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace gl {

	class Vertex
	{
	public:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture;
	};
	
	class Mesh
	{
	private:

		unsigned int VAO_ = 0;
		unsigned int VBO_ = 0;
		unsigned int EBO_ = 0;

	public:
		unsigned int material_index;
		unsigned int nb_vertices;

		Mesh(const std::vector<Vertex>& vertices,
		     const std::vector<std::uint32_t>& indices,
		     const unsigned int material_id);

		unsigned int GetVao() const;

		void Bind() const;

		void UnBind() const;
	};
	
} // End namespace gl.