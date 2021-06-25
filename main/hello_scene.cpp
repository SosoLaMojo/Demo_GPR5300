#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include <SDL_main.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "mesh2.h"
#include "model.h"
#include "instancied_asteroids.h"

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
		//void SetModelMatrix(seconds dt);
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
		InstanciedAsteroid asteroids_;
		std::unique_ptr<Shader> instanciedAsteroidShader_ = nullptr;

		glm::mat4 view_ = glm::mat4(1.0f);
		glm::mat4 projection_ = glm::mat4(1.0f);
		glm::vec2 windowSize_ = glm::vec2(1024,720);
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
		camera_ = std::make_unique<Camera>(glm::vec3(.0f, 30.0f, 50.0f)); // Camera position
		skybox_mesh_ = std::make_unique<MeshSkybox>();
		frameBuffer_ = std::make_unique<FrameBuffer>(windowSize_);
		std::string path = "../";

		// TODO calculate rotate speed and position for each planets + axis rotation sun
		//Planets
		//index 0
		planets_.push_back(Planet(path + "data/models/sun.obj",
			0.0f,	//rotate speed around point
			glm::vec3(0.0f, 0.01f, 0.0f),// Axis rotation spin
			glm::vec3(0.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			0.1f));// speed spin
		//index 1
		planets_.push_back(Planet(path + "data/models/mercure.obj",
			0.17f, //rotate speed around point
			glm::vec3(0.03f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(10.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			0.1f));// speed spin
		//index 2
		planets_.push_back(Planet(path + "data/models/venus.obj",
			0.12f, //rotate speed around point
			glm::vec3(0.052f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(14.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			0.1f));// speed spin
		//index 3
		planets_.push_back(Planet(path + "data/models/earth_and_moon.obj",
			0.1f, //rotate speed around point
			glm::vec3(0.43f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(18.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			0.1f));// speed spin
		//index 4
		planets_.push_back(Planet(path + "data/models/mars.obj",
			0.08f, //rotate speed around point
			glm::vec3(0.44f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(22.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			1.5f));// speed spin
		//index 5
		planets_.push_back(Planet(path + "data/models/jupiter.obj",
			0.04f, //rotate speed around point
			glm::vec3(0.052f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(35.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			1.5f));// speed spin
		//index 6
		planets_.push_back(Planet(path + "data/models/saturn.obj",
			0.03f, //rotate speed around point
			glm::vec3(0.5f, -1.0f, 0.0f),// Axis rotation spin
			glm::vec3(40.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			1.5f));// speed spin
		//index 7
		planets_.push_back(Planet(path + "data/models/uranus.obj",
			0.02f, //rotate speed around point
			glm::vec3(7.11f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(45.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			1.5f));// speed spin
		//index 8
		planets_.push_back(Planet(path + "data/models/neptune.obj",
			0.01f, //rotate speed around point
			glm::vec3(0.54f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(50.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			1.5f));// speed spin

		asteroids_= InstanciedAsteroid(path + "data/models/rock.obj",
			0.06f, //rotate speed around point
			glm::vec3(0.54f, 1.0f, 0.0f),// Axis rotation spin
			glm::vec3(55.0f, 0.0f, 0.0f),// transVec movement planet World, position of planet in world
			glm::vec3(27.0f, 0.0f, 0.0f),
			0.5f, // min speed spin
			7000, // nb asteroids sur les 2 ceintures / 2
			4.0f, // largeur de chaque ceinture en random
			2.0f, //hauteur de chaque ceinture en random
			2.0f, // taille max des asteroids en random
			3.0f); // vitesse max de rotation par asteroid en random

		//planets_[0].SetPosition(glm::vec3(0, 0, 0)); // sun
		//planets_[1].SetPosition(glm::vec3(10, 0, 0)); // mercure
		//planets_[2].SetPosition(glm::vec3(14, 0, 0)); // venus
		//planets_[3].SetPosition(glm::vec3(18, 0, 0)); // earth and moon
		//planets_[4].SetPosition(glm::vec3(22, 0, 0)); // mars
		//planets_[5].SetPosition(glm::vec3(30, 0, 0)); // jupiter
		//planets_[6].SetPosition(glm::vec3(35, 0, 0)); // saturn
		//planets_[7].SetPosition(glm::vec3(40, 0, 0)); // uranus
		//planets_[8].SetPosition(glm::vec3(45, 0, 0)); // neptune

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

		instanciedAsteroidShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/instancied_asteroid.vert",
			path + "data/shaders/hello_scene/instancied_asteroid.frag");
	}

	void HelloScene::SetViewMatrix(seconds dt)
	{
		view_ = camera_->GetViewMatrix();
	}

	void HelloScene::SetProjectionMatrix()
	{
		projection_ = glm::perspective(glm::radians(45.0f),
			windowSize_.x / windowSize_.y, // aspect ratio
			0.1f,
			200.f); // ligne d'horizon (jusqu'ou on voit au loin)
	}

	void HelloScene::SetUniformMatrix() const
	{
		shader_->Use();
		shader_->SetMat4("view", view_);
		shader_->SetMat4("projection", projection_);
		shader_->SetVec3("camera_position", camera_->position);

		instanciedAsteroidShader_->Use();
		instanciedAsteroidShader_->SetMat4("view", view_);
		instanciedAsteroidShader_->SetMat4("projection", projection_);
		instanciedAsteroidShader_->SetVec3("camera_position", camera_->position);
	}

	void HelloScene::Update(seconds dt)
	{
		// FrameBuffer
		frameBuffer_->Bind();
		// Planets
		delta_time_ = dt.count();
		time_ += delta_time_;
		SetViewMatrix(dt);
		SetProjectionMatrix();
		SetUniformMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader_->Use();
		shader_->SetInt("Diffuse", 0);
		shader_->SetInt("Specular", 1);
		//std::cout << std::to_string(planets_[0].GetPosition().x) << ", " << std::to_string(planets_[0].GetPosition().y) << ", " << std::to_string(planets_[0].GetPosition().z) << "\n";
		for (Planet& planet : planets_)
		{
			planet.Update(dt, *shader_);
		}

		// Asteroids
		instanciedAsteroidShader_->Use();
		instanciedAsteroidShader_->SetInt("Diffuse", 0);
		instanciedAsteroidShader_->SetInt("Specular", 1);
		asteroids_.Update(dt, *instanciedAsteroidShader_);

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
		frameBuffer_->UnBind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		frameBufferShader_->Use();
		frameBufferShader_->SetInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameBuffer_->GetColorBuffer());
		frameBuffer_->Draw();
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
			windowSize_ = glm::vec2(event.window.data1, event.window.data2);
			frameBuffer_ = std::make_unique<FrameBuffer>(windowSize_);
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