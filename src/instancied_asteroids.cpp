#include "instancied_asteroids.h"
#include "camera.h"

#include "Tracy.hpp"
#include "TracyOpenGL.hpp"

namespace gl {

	InstanciedAsteroid::InstanciedAsteroid() {}

	InstanciedAsteroid::InstanciedAsteroid(std::string filepath, float rotationSpeedFactor,
		glm::vec3 spinRotationAxis, glm::vec3 transVec, glm::vec3 transVec2,
		float spinSpeedFactor,
		unsigned nbAsteroids, float thicknessAsteroidsX,
		float thicknessAsteroidsY, float maxSizeAsteroid,
		float maxSpeedSpinAsteroid) :
		rotationSpeedFactor_(rotationSpeedFactor),
		spinRotationAxis_(spinRotationAxis),
		transVec_(transVec),
		transVec2_(transVec2),
		spinSpeedFactor_(spinSpeedFactor),
		nbAsteroids_(nbAsteroids),
		thicknessAsteroidsX_(thicknessAsteroidsX),
		thicknessAsteroidsY_(thicknessAsteroidsY),
		maxSizeAsteroid_(maxSizeAsteroid),
		maxSpeedSpinAsteroid_(maxSpeedSpinAsteroid)
	{
		initTransDistanceX_.resize(nbAsteroids_);
		std::generate(initTransDistanceX_.begin(), initTransDistanceX_.end(), [&]()
			{
				return (rand() % densityAsteroids_) / (densityAsteroids_ / thicknessAsteroidsX_);
			});

		initTransDistanceY_.resize(nbAsteroids_);
		std::generate(initTransDistanceY_.begin(), initTransDistanceY_.end(), [&]()
			{
				return (rand() % densityAsteroids_) / (densityAsteroids_ / thicknessAsteroidsY_);
			});

		initTransScale_.resize(nbAsteroids_);
		std::generate(initTransScale_.begin(), initTransScale_.end(), [&]()
			{
				return (rand() % densityAsteroids_) / (densityAsteroids_ / maxSizeAsteroid_);
			});

		initTransSpeedSpinRotate_.resize(nbAsteroids_);
		std::generate(initTransSpeedSpinRotate_.begin(), initTransSpeedSpinRotate_.end(), [&]()
			{
				return (rand() % densityAsteroids_) / (densityAsteroids_ / maxSpeedSpinAsteroid_);
			});

		modelMatrix_.resize(nbAsteroids_, glm::mat4(1.0f));
		model_ = std::make_unique<Model>(filepath);
		const auto& asteroidMesh = model_->GetMesh(0);

		glBindVertexArray(asteroidMesh.GetVao());

		// VBO instancing
		glGenBuffers(1, &instanceVBO_);

		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	void InstanciedAsteroid::Update(std::chrono::duration<float, std::ratio<1, 1>> dt, Shader& shader, Camera* camera)
	{
		ZoneScoped;
		TracyGpuZone("Check Update InstanciedAsteroid");
		// Frustum Culling
		std::vector<glm::mat4> asteroidCulled;
		float nearPlane = 0.1f;
		float farPlane = 400.0f;
		const float radius = 0.3f;
		const auto cameraDir = camera->front;
		const auto cameraRightDir = camera->right;
		const auto cameraUp = camera->up;
		const auto fovX = glm::radians(camera->GetFovX()); // / 2 pour réduire le frustum
		const auto fovY = glm::radians(camera->fovY); // / 2 pour réduire le frustum

		const auto leftQuaternion = glm::angleAxis(fovX / 2.0f, cameraUp);
		const auto leftNormal = leftQuaternion * -cameraRightDir;
		const auto rightQuaternion = glm::angleAxis(-fovX / 2.0f, cameraUp);
		const auto rightNormal = rightQuaternion * cameraRightDir;
		const auto topQuaternion = glm::angleAxis(fovY / 2.0f, cameraRightDir);
		const auto topNormal = topQuaternion * cameraUp;
		const auto bottomQuaternion = glm::angleAxis(-fovY / 2.0f, cameraRightDir);
		const auto bottomNormal = bottomQuaternion * -cameraUp;

		for(auto i = 0; i < nbAsteroids_; i++)
		{
			glm::vec3 asteroidWorldPos = modelMatrix_[i][3];
			const auto asteroidCameraPos = asteroidWorldPos - camera->position;

			// Near and Far
			//const auto planePos = camera->position + cameraDir * nearPlane;
			const auto v1 = glm::dot(cameraDir, asteroidCameraPos);
			if(v1 < radius + nearPlane || v1 > farPlane)
			{
				continue;
			}

			// Left
			const auto v2 = glm::dot(leftNormal, asteroidCameraPos);
			if(v2 > radius)
			{
				continue;
			}

			// Right
			const auto v3 = glm::dot(rightNormal, asteroidCameraPos);
			if (v3 > radius)
			{
				continue;
			}

			// Top
			const auto v4 = glm::dot(topNormal, asteroidCameraPos);
			if (v4 > radius)
			{
				continue;
			}

			// Bottom
			const auto v5 = glm::dot(bottomNormal, asteroidCameraPos);
			if (v5 > radius)
			{
				continue;
			}

			asteroidCulled.push_back(modelMatrix_[i]);
		}
		
		shader.Use();
		shader.SetInt("TexDiffuse", 0);
		shader.SetInt("TexNormal", 1);
		
		delta_time_ = dt.count();
		time_ += delta_time_;

		for (unsigned int i = 0; i < modelMatrix_.size(); i++)
		{
			SetModelMatrix(dt, i);
		}
		Mesh mesh = model_->GetMesh(0);
		mesh.Bind();
		const auto& material = model_->materials[mesh.material_index];
		material.color_tex.Bind(0);
		material.normal_tex.Bind(1);
		shader.SetFloat("specular_pow", material.specular_pow);
		shader.SetVec3("specular_vec", material.specular_vec);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
		if(asteroidCulled.size() > 0)
		{
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(glm::mat4) * asteroidCulled.size(),
				&asteroidCulled[0],
				GL_DYNAMIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(mesh.GetVao());
		glDrawElementsInstanced(GL_TRIANGLES,
			mesh.nb_vertices,
			GL_UNSIGNED_INT, 
			0,
			asteroidCulled.size());
		glBindVertexArray(0);

	}

	void InstanciedAsteroid::SetModelMatrix(std::chrono::duration<float, std::ratio<1, 1>> dt, unsigned i)
	{
		modelMatrix_[i] = glm::mat4(1.0f);
		// rotate around specific point
		modelMatrix_[i] = glm::rotate(modelMatrix_[i], (time_ + i) * rotationSpeedFactor_, glm::vec3(0.0f, 1.0f, 0.0f));
		if (i < nbAsteroids_ / 2)
		{
			modelMatrix_[i] = glm::translate(modelMatrix_[i],
				transVec_ + glm::vec3(initTransDistanceX_[i], initTransDistanceY_[i],
					0.0f));
		}
		else
		{
			modelMatrix_[i] = glm::translate(modelMatrix_[i],
				transVec2_ + glm::vec3(initTransDistanceX_[i], initTransDistanceY_[i],
					0.0f));
		}
		modelMatrix_[i] = glm::rotate(modelMatrix_[i], time_ * (spinSpeedFactor_ + initTransSpeedSpinRotate_[i]),
			spinRotationAxis_); //rotate around themself
		modelMatrix_[i] = glm::scale(modelMatrix_[i], glm::vec3(initTransScale_[i])); // random size of each asteroids
	}

	glm::vec3 InstanciedAsteroid::GetPosition(unsigned i)
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(modelMatrix_[i], scale, rotation, translation, skew, perspective);

		return translation;
	}

	/*InstanciedAsteroid::AsteroidTransfom InstanciedAsteroid::GetTransform(unsigned i)
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(modelMatrix_[i], scale, rotation, translation, skew, perspective);

		AsteroidTransfom result;
		result.translation = translation;
		result.scale = scale;
		result.rotation = rotation;

		return result;
	}*/
	
} // namespace gl