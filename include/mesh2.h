#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		//std::vector<Vertex> vertice;


		Mesh(const std::vector<Vertex>& vertices,
			const std::vector<std::uint32_t>& indices,
			const unsigned int material_id) :
			//vertice(vertices),
			material_index(material_id),
			nb_vertices(static_cast<unsigned int>(indices.size()))
		{

			// VAO binding should be before VAO.
			glGenVertexArrays(1, &VAO_);
			glBindVertexArray(VAO_); // pour mirroir mettre a la fin

			// EBO.
			glGenBuffers(1, &EBO_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				indices.size() * sizeof(float),
				indices.data(),
				GL_STATIC_DRAW);

			// VBO.
			glGenBuffers(1, &VBO_);
			glBindBuffer(GL_ARRAY_BUFFER, VBO_);
			glBufferData(
				GL_ARRAY_BUFFER,
				vertices.size() * sizeof(Vertex),
				vertices.data(),
				GL_STATIC_DRAW);

			GLintptr vertex_color_offset = 3 * sizeof(float);
			GLintptr vertex_tex_offset = 6 * sizeof(float);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				8 * sizeof(float),
				0);
			glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				8 * sizeof(float),
				(GLvoid*)vertex_color_offset);
			glVertexAttribPointer(
				2,
				2,
				GL_FLOAT,
				GL_FALSE,
				8 * sizeof(float),
				(GLvoid*)vertex_tex_offset);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		unsigned int GetVao() const
		{
			return VAO_;
		}
		
		void Bind() const
		{
			glBindVertexArray(VAO_);
		}

		void UnBind() const
		{
			glBindVertexArray(0);
		}
	};

} // End namespace gl.



