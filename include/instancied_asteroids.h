#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "mesh2.h"
#include "material.h"
#include "shader.h"
#include "model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <cassert>
#include <chrono>


namespace gl {

	class InstanciedAsteroid
	{
	private:
		std::unique_ptr<Model> model_ = nullptr;

		float rotationSpeedFactor_ = 1.0f;
		glm::vec3 rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
		
		glm::mat4 modelMatrix_ = glm::mat4(1.0f);
		glm::mat4 invModelMatrix_ = glm::mat4(1.0f);
		unsigned int amount = 100000;
		float radius = 150.0;
		float offset = 25.0f;
		//Model rock(FileSystem::getPath("resources/objects/rock/rock.obj"));
		
	public:
		InstanciedAsteroid() {}

		InstanciedAsteroid()
		{
			glm::mat4* modelMatrices;
			modelMatrices = new glm::mat4[amount];
			//srand(glfwGetTime()); // initialize random seed	
			for (unsigned int i = 0; i < amount; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				// 1. translation: displace along circle with 'radius' in range [-offset, offset]
				float angle = (float)i / (float)amount * 360.0f;
				float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float x = sin(angle) * radius + displacement;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float z = cos(angle) * radius + displacement;
				model = glm::translate(model, glm::vec3(x, y, z));

				// 2. scale: Scale between 0.05 and 0.25f
				float scale = (rand() % 20) / 100.0f + 0.05;
				model = glm::scale(model, glm::vec3(scale));

				// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
				float rotAngle = (rand() % 360);
				model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

				// 4. now add to list of matrices
				modelMatrices[i] = model;
			}

			unsigned int buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

			//for (unsigned int i = 0; i < rock.meshes.size(); i++)
			{
				//unsigned int VAO = rock.meshes[i].VAO;
				//glBindVertexArray(VAO);
				// set attribute pointers for matrix (4 times vec4)
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
				glEnableVertexAttribArray(6);
				glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

				glVertexAttribDivisor(3, 1);
				glVertexAttribDivisor(4, 1);
				glVertexAttribDivisor(5, 1);
				glVertexAttribDivisor(6, 1);

				glBindVertexArray(0);
			}

		}
		//InstanciedAsteroid() = default;
		/*InstanciedAsteroid() {}

		InstanciedAsteroid(std::string filepath, float rotationSpeedFactor, glm::vec3 rotationAxis) :
			rotationSpeedFactor_(rotationSpeedFactor),
			rotationAxis_(rotationAxis)
		{
			model_ = std::make_unique<Model>(filepath);
		}

		void Update(std::chrono::duration<float, std::ratio<1, 1>> dt, Shader& shader)
		{
			SetModelMatrix(dt);

			shader.SetMat4("inv_model", invModelMatrix_);
			shader.SetMat4("model", modelMatrix_);

			for (const auto& mesh : model_->meshes)
			{
				mesh.Bind();
				const auto& material = model_->materials[mesh.material_index];
				material.color_tex.Bind(0);
				material.specular_tex.Bind(1);
				shader.SetFloat("specular_pow", material.specular_pow);
				shader.SetVec3("specular_vec", material.specular_vec);
				glDrawElements(GL_TRIANGLES, mesh.nb_vertices, GL_UNSIGNED_INT, 0);
			}
		}*/

		//struct AsteroidTransfom
		//{
		//	glm::quat rotation;
		//	glm::vec3 translation;
		//	glm::vec3 scale;
		//};

		//AsteroidTransfom GetTransform()
		//{
		//	glm::vec3 scale;
		//	glm::quat rotation;
		//	glm::vec3 translation;
		//	glm::vec3 skew;
		//	glm::vec4 perspective;
		//	glm::decompose(modelMatrix_, scale, rotation, translation, skew, perspective);

		//	AsteroidTransfom result;
		//	result.translation = translation;
		//	result.scale = scale;
		//	result.rotation = rotation;

		//	return result;
		//}

		//void SetModelMatrix(std::chrono::duration<float, std::ratio<1, 1>> dt)
		//{
		//	modelMatrix_ = glm::rotate(modelMatrix_, dt.count() * rotationSpeedFactor_, rotationAxis_); //rotate around themself

		//	//TODO rotate around an object/position
		//	//float asteroidRotation = glm::rotate(GetTransform().translation * GetTransform().translation);

		//	invModelMatrix_ = glm::transpose(glm::inverse(modelMatrix_));
		//}
		
		void Draw()
		{
			
		}

		void Bind()
		{
			
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
	
}