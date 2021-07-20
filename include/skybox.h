#pragma once
#include "camera.h"
#include "shader.h"

namespace gl {
	
	class MeshSkybox
	{
	private:
		unsigned int skyboxVAO_ = 0;
		unsigned int skyboxVBO_ = 0;
		unsigned int skyboxTextureID_;

	public:
		MeshSkybox();

		void Bind(unsigned int i = 0) const;
	};

	class ModelSkybox
	{
	private:
		std::unique_ptr<MeshSkybox> skybox_mesh_ = nullptr;
		std::unique_ptr<Shader> skyboxShader_ = nullptr;

		const Camera& camera_;
		unsigned int nbTriangles_ = 36;
	public:
		ModelSkybox(const std::string& path, const Camera& camera);
		
		void Update() const;
		void Draw() const;
		
	};
	
} // namespace gl
