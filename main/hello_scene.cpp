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
		std::unique_ptr<Shader> shader_ = nullptr;
		std::unique_ptr<Shader> skyboxShader_ = nullptr;
		std::unique_ptr<Shader> frameBufferShader_ = nullptr;
		std::unique_ptr<Model> model_obj_ = nullptr;
		std::unique_ptr<MeshSkybox> skybox_mesh_ = nullptr;
		std::unique_ptr<FrameBuffer> frameBuffer_ = nullptr;
		std::vector<Planet> planets_;

		//glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 view_ = glm::mat4(1.0f);
		//glm::mat4 inv_model_ = glm::mat4(1.0f);
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
		camera_ = std::make_unique<Camera>(glm::vec3(.0f, 0.0f, 30.0f)); // Camera position
		skybox_mesh_ = std::make_unique<MeshSkybox>();
		frameBuffer_ = std::make_unique<FrameBuffer>();
		std::string path = "../";

		//Planets
		//index 0	
		planets_.push_back(Planet(path + "data/models/MoonEarthAndMars.obj",
			0.5f,	//rotate speed
			glm::vec3(0.0f, 1.0f, 0.0f))); // Axis rotation
//index 1
		planets_.push_back(Planet(path + "data/models/MoonEarthAndMars.obj",
			1.5f, //rotate speed
			glm::vec3(0.0f, 0.1f, 0.0f))); // Axis rotation
//index 2
		planets_.push_back(Planet(path + "data/models/MoonEarthAndMars.obj",
			3.5f, //rotate speed
			glm::vec3(1.0f, 0.0f, 0.0f))); // Axis rotation

		planets_[0].SetPosition(glm::vec3(10, 10, 10));
		planets_[1].SetPosition(glm::vec3(-10, -10, -10));

		//model_obj_ = std::make_unique<Model>(
		//	path + "data/models/MoonEarthAndMars.obj");

		// Shaders
		shader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/scene.vert",
			path + "data/shaders/hello_scene/scene.frag");

		skyboxShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/skybox.vert",
			path + "data/shaders/hello_scene/skybox.frag");

		frameBufferShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/frame_buffer.vert",
			path + "data/shaders/hello_scene/frame_buffer.frag");

		//glClearColor(0.7f, 0.8f, 0.9f, 1.0f);
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
		shader_->Use();
		shader_->SetMat4("view", view_);
		shader_->SetMat4("projection", projection_);
		shader_->SetVec3("camera_position", camera_->position);
	}

	void HelloScene::Update(seconds dt)
	{
		// FrameBuffer
		IsError(__FILE__, __LINE__);
		frameBuffer_->Bind();
		IsError(__FILE__, __LINE__);
		// Planets
		delta_time_ = dt.count();
		time_ += delta_time_;
		SetViewMatrix(dt);
		/*SetModelMatrix(dt);*/
		SetProjectionMatrix();
		SetUniformMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader_->Use();
		shader_->SetInt("Diffuse", 0);
		shader_->SetInt("Specular", 1);
		//shader_->SetMat4("view", view_);
		std::cout << std::to_string(planets_[0].GetPosition().x) << ", " << std::to_string(planets_[0].GetPosition().y) << ", " << std::to_string(planets_[0].GetPosition().z) << "\n";
		for (Planet& planet : planets_)
		{
			planet.Update(dt, *shader_);
		}
		/*for(const auto& mesh : model_obj_->meshes)
		{
			mesh.Bind();
			const auto& material = model_obj_->materials[mesh.material_index];
			material.color_tex.Bind(0);
			material.specular_tex.Bind(1);
			shader_->SetFloat("specular_pow", material.specular_pow);
			shader_->SetVec3("specular_vec", material.specular_vec);
			glDrawElements(GL_TRIANGLES, mesh.nb_vertices, GL_UNSIGNED_INT, 0);
		}*/

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

		// framebuffer
		IsError(__FILE__, __LINE__);
		frameBuffer_->UnBind();
		IsError(__FILE__, __LINE__);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		IsError(__FILE__, __LINE__);
		frameBufferShader_->Use();
		IsError(__FILE__, __LINE__);
		frameBufferShader_->SetInt("screenTexture", 0);
		IsError(__FILE__, __LINE__);
		glActiveTexture(GL_TEXTURE0);
		IsError(__FILE__, __LINE__);
		glBindTexture(GL_TEXTURE_2D, frameBuffer_->GetColorBuffer());
		IsError(__FILE__, __LINE__);

		frameBuffer_->Draw();
		IsError(__FILE__, __LINE__);
	}

	void HelloScene::Destroy()
	{
	}

	void IsError(const std::string& file, int line)
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

	void HelloScene::OnEvent(SDL_Event& event)
	{
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			glViewport(0, 0, event.window.data1, event.window.data2);
		}
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