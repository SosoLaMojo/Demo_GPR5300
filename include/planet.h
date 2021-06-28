#pragma once
#include <chrono>
#include <string>
#include <glm/vec3.hpp>
#include "shader.h"
#include "model.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace gl
{
	class Planet
	{
	public:
		Planet();

		// TODO passer par reference pour les vec3 12bit -> 8bit + string
		Planet(std::string filepath, float rotationSpeedFactor, glm::vec3 spinRotationAxis, glm::vec3 transVec,
			float spinSpeedFactor);

		void Update(std::chrono::duration<float, std::ratio<1, 1>> dt, Shader& shader);

		void SetModelMatrix(std::chrono::duration<float, std::ratio<1, 1>> dt);

		glm::vec3 GetPosition();

		struct PlanetTransfom
		{
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 scale;
		};

		PlanetTransfom GetTransform();

	private:

		float time_ = 0.0f;
		float delta_time_ = 0.0f;
		float spinSpeedFactor_ = 1.0f; // speed spin
		float rotationSpeedFactor_ = 1.0f; // speed rotate in world
		std::unique_ptr<Model> model_ = nullptr;

		glm::vec3 transVec_ = glm::vec3(0.0f, 0.0f, 0.0f); // transVec movement planet World
		glm::vec3 spinRotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f); // Axis rotation spin 
		glm::mat4 modelMatrix_ = glm::mat4(1.0f);
		glm::mat4 invModelMatrix_ = glm::mat4(1.0f);
	};
	
} // namespace gl