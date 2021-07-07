#include <glad/glad.h>
#include "mesh.h"

namespace gl {
	
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices,
		const unsigned material_id) :
		material_index(material_id),
		nb_vertices(static_cast<unsigned int>(indices.size()))
	{
		// VAO binding should be before VAO.
		glGenVertexArrays(1, &VAO_);
		glBindVertexArray(VAO_);

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

		GLintptr vertex_normal_offset = 3 * sizeof(float);
		GLintptr vertex_tex_offset = 6 * sizeof(float);
		GLintptr aTangent_offset = 8 * sizeof(float);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			11 * sizeof(float),
			0);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			11 * sizeof(float),
			(GLvoid*)vertex_normal_offset);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			11 * sizeof(float),
			(GLvoid*)vertex_tex_offset);
		glVertexAttribPointer(
			3,
			3,
			GL_FLOAT,
			GL_FALSE,
			11 * sizeof(float),
			(GLvoid*)aTangent_offset);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	unsigned Mesh::GetVao() const
	{
		return VAO_;
	}

	void Mesh::Bind() const
	{
		glBindVertexArray(VAO_);
	}

	void Mesh::UnBind() const
	{
		glBindVertexArray(0);
	}
	
} // namespace gl