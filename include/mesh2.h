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

		std::unique_ptr<Shader> shaders_ = nullptr;
		glm::mat4 view = glm::mat4(1.0f);
		
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
			if(data)
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
		
		/*shaders_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/skybox.vert",
			path + "data/shaders/hello_scene/skybox.frag");*/

		/*shaders_->Use();
		shaders_->SetInt("skybox", 0);
		shaders_->SetMat4("projection", glm::perspective(glm::radians(45.0f),
			4.0f / 3.0f, 0.1f, 100.f));*/
	}
	/*void UpdateSkybox()
	{
		shaders_->SetMat4("view", view);
	}*/

	void Bind(unsigned int i = 0) const
	{
		glBindVertexArray(skyboxVAO_);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
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



