#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>


namespace gl {

	class Vertex
	{
	public:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture;
	};

	class MeshSkybox
	{
	private:
		unsigned int skyboxVAO_ = 0;
		unsigned int skyboxVBO_ = 0;
		unsigned int skyboxTextureID;

	public:
		MeshSkybox()
		{
			float skyboxVertices[108] = {
				// positions          
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};

			// VAO Skybox
			glGenVertexArrays(1, &skyboxVAO_);
			glBindVertexArray(skyboxVAO_);

			// VBO Skybox
			glGenBuffers(1, &skyboxVBO_);
			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO_);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW); // transfert les données dans le GPU
			glEnableVertexAttribArray(0); // for location 0 in skybox.vert aPos ACTIVER aPos dans le shader, quoi utiliser
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // lie les données de skyboxverticies déja contenu dans le GPU, comment utiliser
			glBindVertexArray(0); // info n'utilise plus VAO

			std::string path = "../";
			std::vector<std::string> faces
			{
				path + "data/textures/Skybox/right.png",
				path + "data/textures/Skybox/left.png",
				path + "data/textures/Skybox/top.png",
				path + "data/textures/Skybox/bot.png",
				path + "data/textures/Skybox/front.png",
				path + "data/textures/Skybox/back.png",
			};

			glGenTextures(1, &skyboxTextureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

			int width, height, nrChannels;
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // transfert les données sur le GPU
						0,
						GL_RGB, // in en mémoire
						width,
						height,
						0,
						GL_RGB, // out en mémoire
						GL_UNSIGNED_BYTE, // 1 byte par composant de couleur
						data);
					stbi_image_free(data);
				}
				else
				{
					std::cout << " skyboxTexture failed to load\n";
					stbi_image_free(data);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP,
				GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,
				GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,
				GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,
				GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,
				GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		void Bind(unsigned int i = 0) const
		{
			glBindVertexArray(skyboxVAO_);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
		}

	};

	class FrameBuffer
	{
	private:
		unsigned int FBO_ = 0;
		unsigned int RBO_ = 0;
		unsigned int texColorBuffer_ = 0;
		unsigned int quadVAO_ = 0;
		unsigned int quadVBO_ = 0;

	public:
		FrameBuffer()
		{
			float quadVertices[24] = {
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			};
			// FBO
			glGenFramebuffers(1, &FBO_);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
			// Generate texture
			glGenTextures(1, &texColorBuffer_);
			glBindTexture(GL_TEXTURE_2D, texColorBuffer_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer_, 0);
			// RBO
			glGenRenderbuffers(1, &RBO_);
			glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 720);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO_);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cout << "Framebuffer is not complete!\n";
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// quad VAO
			glGenVertexArrays(1, &quadVAO_);
			glBindVertexArray(quadVAO_);
			// quad VBO
			glGenBuffers(1, &quadVBO_);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0,
				2,
				GL_FLOAT,
				GL_FALSE,
				4 * sizeof(float),
				(void*)0);
			glVertexAttribPointer(1,
				2,
				GL_FLOAT,
				GL_FALSE,
				4 * sizeof(float),
				(void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

		}

		unsigned int GetColorBuffer()
		{
			return texColorBuffer_;
		}

		void Draw() const
		{
			glBindVertexArray(quadVAO_);
			IsError(__FILE__, __LINE__);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			IsError(__FILE__, __LINE__);
		}

		void Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
			IsError(__FILE__, __LINE__);
		}

		void UnBind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void IsError(const std::string& file, int line) const
		{
			auto error_code = glGetError();
			if (error_code != GL_NO_ERROR)
			{
				std::cerr
					<< error_code
					<< " in file: " << file
					<< " at line: " << line
					<< "\n";
			}
		}
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



