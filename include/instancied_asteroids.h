#pragma once
#include <string>
//#include <glad/glad.h>
#include <glm/glm.hpp>
//#include <iostream>
#include <vector>
//#include "mesh2.h"
#include "material.h"
#include "shader.h"
#include "model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
//#include <cassert>
#include <chrono>


namespace gl {

	class InstanciedAsteroid
	{
	public:
		InstanciedAsteroid();

		InstanciedAsteroid(std::string filepath, float rotationSpeedFactor, glm::vec3 spinRotationAxis,
		                   glm::vec3 transVec, glm::vec3 transVec2, float spinSpeedFactor, unsigned int nbAsteroids,
		                   float thicknessAsteroidsX, float thicknessAsteroidsY, float maxSizeAsteroid,
		                   float maxSpeedSpinAsteroid);

		void Update(std::chrono::duration<float, std::ratio<1, 1>> dt, Shader& shader);

		void SetModelMatrix(std::chrono::duration<float, std::ratio<1, 1>> dt, unsigned int i);

		glm::vec3 GetPosition(unsigned int i);

		struct AsteroidTransfom
		{
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 scale;
		};

		AsteroidTransfom GetTransform(unsigned int i);

		/*void SetPosition(glm::vec3 position)
		{
			modelMatrix_ = glm::translate(modelMatrix_, glm::vec3(0.0f));
			modelMatrix_ = glm::translate(modelMatrix_, position);
		}*/

	private:

		float time_ = 0.0f;
		float delta_time_ = 0.0f;
		float spinSpeedFactor_ = 1.0f; // speed spin
		float rotationSpeedFactor_ = 1.0f; // speed rotate in world
		float thicknessAsteroidsX_;
		float thicknessAsteroidsY_;
		float maxSizeAsteroid_;
		float maxSpeedSpinAsteroid_;
		int densityAsteroids_ = 1000;
		unsigned int instanceVBO_;
		std::unique_ptr<Model> model_ = nullptr;

		glm::vec3 transVec_ = glm::vec3(0.0f, 0.0f, 0.0f); // transVec movement planet World
		glm::vec3 transVec2_ = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 spinRotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f); // Axis rotation spin 
		std::vector<glm::mat4> modelMatrix_; // tableau de modelmatrix (taille, rotation et position) pour chaque asteroid
		unsigned int nbAsteroids_ = 0;
		// Randoms
		std::vector<float> initTransDistanceX_;
		std::vector<float> initTransDistanceY_;
		std::vector<float> initTransScale_;
		std::vector<float> initTransSpeedSpinRotate_;
	};

	
}
