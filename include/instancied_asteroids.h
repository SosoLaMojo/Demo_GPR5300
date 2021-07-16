#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <chrono>

#include "camera.h"
#include "shader.h"
#include "model.h"

namespace gl {

	class InstanciedAsteroid
	{
	public:
		InstanciedAsteroid();

		// TODO passer par reference pour les vec3 12bit -> 8bit and string + edit name transvec -> ceinture asteroids1 et 2
		InstanciedAsteroid(std::string filepath, float rotationSpeedFactor, glm::vec3 spinRotationAxis,
		                   glm::vec3 transVec, glm::vec3 transVec2, float spinSpeedFactor, unsigned int nbAsteroids,
		                   float thicknessAsteroidsX, float thicknessAsteroidsY, float maxSizeAsteroid,
		                   float maxSpeedSpinAsteroid);
		
		void Update(std::chrono::duration<float, std::ratio<1, 1>> dt, Shader& shader, Camera* camera);

		void SetModelMatrix(std::chrono::duration<float, std::ratio<1, 1>> dt, unsigned int i);

		glm::vec3 GetPosition(unsigned int i);

		//AsteroidTransfom GetTransform(unsigned int i);

	private:

		// TODO edit names
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
	
} // namespace gl
