#pragma once
#include <string>
#include <glm/vec2.hpp>
#include "shader.h"

namespace gl {
	
	class FrameBuffer
	{
	private:
		unsigned int FBO_ = 0;
		unsigned int RBO_ = 0;
		unsigned int texColorBuffer_ = 0;
		unsigned int texColorBufferBloom_ = 0;
		unsigned int quadVAO_ = 0;
		unsigned int quadVBO_ = 0;

		std::unique_ptr<FrameBuffer> frameBuffer_ = nullptr;
		std::unique_ptr<Shader> frameBufferShader_ = nullptr;
	
	public:
		FrameBuffer(glm::vec2 windowSize);
		
		unsigned int GetColorBuffer();
		
		/*void Update() const;*/
		void Draw() const;
		void Bind() const;
		void UnBind() const;

	protected:
	};
	
} // namespace gl