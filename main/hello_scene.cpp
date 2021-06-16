#include <SDL_main.h>
#include <glad/glad.h>
#include <array>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "mesh2.h"
#include "model.h"

namespace gl {

	class HelloScene : public Program
	{
	public:
		void Init() override;
		void Update(seconds dt) override;
		void Destroy() override;
		void OnEvent(SDL_Event& event) override;
		void DrawImGui() override;

	protected:
		void SetModelMatrix(seconds dt);
		void SetViewMatrix(seconds dt);
		void SetProjectionMatrix();
		void IsError(const std::string& file, int line) const;
		void SetUniformMatrix() const;

	protected:
		unsigned int vertex_shader_;
		unsigned int fragment_shader_;
		unsigned int program_;

		float time_ = 0.0f;
		float delta_time_ = 0.0f;

		std::unique_ptr<Camera> camera_ = nullptr;
		std::unique_ptr<Shader> shaders_ = nullptr; // mettre au singulier
		std::unique_ptr<Shader> skyboxShader_ = nullptr;
		std::unique_ptr<Model> model_obj_ = nullptr;
		std::unique_ptr<MeshSkybox> skybox_mesh_ = nullptr;

		glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 view_ = glm::mat4(1.0f);
		glm::mat4 inv_model_ = glm::mat4(1.0f);
		glm::mat4 projection_ = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};

	void HelloScene::IsError(const std::string& file, int line) const
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

	void HelloScene::Init()
	{
		glEnable(GL_DEPTH_TEST);
		camera_ = std::make_unique<Camera>(glm::vec3(.0f, 0.0f, 30.0f));
		

		std::string path = "../";
		model_obj_ = std::make_unique<Model>(path + "data/models/MoonEarthAndMars.obj");

		skybox_mesh_ = std::make_unique<MeshSkybox>();
		
		shaders_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/scene.vert",
			path + "data/shaders/hello_scene/scene.frag");

		skyboxShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/skybox.vert",
			path + "data/shaders/hello_scene/skybox.frag");
		
		//glClearColor(0.7f, 0.8f, 0.9f, 1.0f);
	}

	void HelloScene::SetModelMatrix(seconds dt)
	{
		model_ = glm::rotate(glm::mat4(1.0f), time_, glm::vec3(-1.f, -1.f, 0.f));
		inv_model_ = glm::transpose(glm::inverse(model_));
	}

	void HelloScene::SetViewMatrix(seconds dt)
	{
		view_ = camera_->GetViewMatrix();
	}

	void HelloScene::SetProjectionMatrix()
	{
		projection_ = glm::perspective(glm::radians(45.0f),
			4.0f / 3.0f,
			0.1f,
			100.f);
	}

	void HelloScene::SetUniformMatrix() const
	{
		shaders_->Use();
		shaders_->SetMat4("model", model_);
		shaders_->SetMat4("view", view_);
		shaders_->SetMat4("projection", projection_);
		shaders_->SetMat4("inv_model", inv_model_);
		shaders_->SetVec3("camera_position", camera_->position);
	}

	void HelloScene::Update(seconds dt)
	{
		delta_time_ = dt.count();
		time_ += delta_time_;
		SetViewMatrix(dt);
		SetModelMatrix(dt);
		SetProjectionMatrix();
		SetUniformMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaders_->Use();
		shaders_->SetInt("Diffuse", 0);
		shaders_->SetInt("Specular", 1);
		shaders_->SetMat4("view", view_);
		for(const auto& mesh : model_obj_->meshes)
		{
			mesh.Bind();
			const auto& material = model_obj_->materials[mesh.material_index];
			material.color_tex.Bind(0);
			material.specular_tex.Bind(1);
			shaders_->SetFloat("specular_pow", material.specular_pow);
			shaders_->SetVec3("specular_vec", material.specular_vec);
			glDrawElements(GL_TRIANGLES, mesh.nb_vertices, GL_UNSIGNED_INT, 0);
		}
		// Skybox
		glDepthFunc(GL_LEQUAL);
		skybox_mesh_->Bind();
		skyboxShader_->Use();
		skyboxShader_->SetInt("skybox", 0);
		skyboxShader_->SetMat4("projection", glm::perspective(glm::radians(45.0f),
			4.0f / 3.0f, 0.1f, 100.f));
		glm::mat4 view = glm::mat4(glm::mat3(camera_->GetViewMatrix()));
		skyboxShader_->SetMat4("view", view);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void HelloScene::Destroy()
	{
	}

	void HelloScene::OnEvent(SDL_Event& event)
	{
		if (event.type == SDL_MOUSEMOTION)
		{
			const auto mouseState = SDL_GetMouseState(nullptr, nullptr);
			if (mouseState & SDL_BUTTON(3))
			{
				camera_->ProcessMouseMovement(event.motion.xrel, event.motion.yrel, true);
			}
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				exit(0);
			if (event.key.keysym.sym == SDLK_w)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::FORWARD, delta_time_);
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::BACKWARD, delta_time_);
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::LEFT, delta_time_);
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::RIGHT, delta_time_);
			}
		}
	}

	void HelloScene::DrawImGui()
	{
	}

} // End namespace gl.

int main(int argc, char** argv)
{
	gl::HelloScene program;
	gl::Engine engine(program);
	engine.Run();
	return EXIT_SUCCESS;
}