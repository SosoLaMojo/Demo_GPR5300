#include <glad/glad.h>
#include <iostream>
#include "model.h"
#include "skybox.h"

namespace gl {

	MeshSkybox::MeshSkybox()
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

			-1.0f, -1.0f, 1.0f,
			-1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			 1.0f, 1.0f, -1.0f,
			 1.0f, 1.0f,  1.0f,
			 1.0f, 1.0f,  1.0f,
			-1.0f, 1.0f,  1.0f,
			-1.0f, 1.0f, -1.0f,

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		// transfert les données dans le GPU
		glEnableVertexAttribArray(0); // for location 0 in skybox.vert aPos ACTIVER aPos dans le shader, quoi utiliser
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// lie les données de skyboxverticies déja contenu dans le GPU, comment utiliser
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

		glGenTextures(1, &skyboxTextureID_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID_);

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

	void MeshSkybox::Bind(unsigned i) const
	{
		glBindVertexArray(skyboxVAO_);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID_);
	}
	
	ModelSkybox::ModelSkybox(const std::string& path, const Camera& camera) :
		camera_(camera)
	{
		skybox_mesh_ = std::make_unique<MeshSkybox>();
		skyboxShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/skybox.vert",
			path + "data/shaders/hello_scene/skybox.frag");
	}
	
	void ModelSkybox::Update() const
	{
		glDepthFunc(GL_LEQUAL);
		skybox_mesh_->Bind();
		skyboxShader_->Use();
		skyboxShader_->SetInt("skybox", 0);
		skyboxShader_->SetMat4("projection", glm::perspective(glm::radians(45.0f),
			4.0f / 3.0f, 0.1f, 100.f)); // TODO 4.0f / 3.0f take result 1.33f
		glm::mat4 view = glm::mat4(glm::mat3(camera_.GetViewMatrix()));
		skyboxShader_->SetMat4("view", view);
	}
	
	void ModelSkybox::Draw() const
	{
		glDrawArrays(GL_TRIANGLES, 0, nbTriangles_);
	}
	
	
} // namespace gl.