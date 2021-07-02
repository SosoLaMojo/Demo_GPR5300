#include "planet.h"

namespace gl {

	Planet::Planet() {}

	Planet::Planet(std::string filepath, float rotationSpeedFactor, glm::vec3 spinRotationAxis,
		glm::vec3 transVec, float spinSpeedFactor) :
		rotationSpeedFactor_(rotationSpeedFactor),
		spinRotationAxis_(spinRotationAxis),
		transVec_(transVec),
		spinSpeedFactor_(spinSpeedFactor)
	{
		model_ = std::make_unique<Model>(filepath);
	}

	void Planet::Update(std::chrono::duration<float, std::ratio<1, 1>> dt, Shader& shader)
	{
		SetModelMatrix(dt);

		shader.SetMat4("inv_model", invModelMatrix_);
		shader.SetMat4("model", modelMatrix_);

		for (const auto& mesh : model_->meshes)
		{
			mesh.Bind();
			const auto& material = model_->materials[mesh.material_index];
			material.color_tex.Bind(0);
			material.normal_tex.Bind(1);
			shader.SetFloat("specular_pow", material.specular_pow);
			shader.SetVec3("specular_vec", material.specular_vec);
			glDrawElements(GL_TRIANGLES, mesh.nb_vertices, GL_UNSIGNED_INT, 0);
		}
	}

	void Planet::SetModelMatrix(std::chrono::duration<float, std::ratio<1, 1>> dt)
	{
		delta_time_ = dt.count();
		time_ += delta_time_;

		modelMatrix_ = glm::mat4(1.0f);
		modelMatrix_ = glm::rotate(modelMatrix_, time_ * rotationSpeedFactor_, glm::vec3(0.0f, 1.0f, 0.0f));
		// rotate around specific point
		modelMatrix_ = glm::translate(modelMatrix_, transVec_);
		modelMatrix_ = glm::rotate(modelMatrix_, time_ * spinSpeedFactor_, spinRotationAxis_); //rotate around themself

		invModelMatrix_ = glm::transpose(glm::inverse(modelMatrix_));
	}

	glm::vec3 Planet::GetPosition()
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(modelMatrix_, scale, rotation, translation, skew, perspective);

		return translation;
	}

	Planet::PlanetTransfom Planet::GetTransform()
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(modelMatrix_, scale, rotation, translation, skew, perspective);

		PlanetTransfom result;
		result.translation = translation;
		result.scale = scale;
		result.rotation = rotation;

		return result;
	}
	
} // namespace gl