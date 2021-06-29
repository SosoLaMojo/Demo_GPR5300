#pragma once
#include <string>
#include <glm/vec2.hpp>

namespace gl {
	
	class FrameBuffer
	{
	private:
		unsigned int FBO_ = 0;
		unsigned int RBO_ = 0;
		unsigned int texColorBuffer_ = 0;
		unsigned int quadVAO_ = 0;
		unsigned int quadVBO_ = 0;

	public:
		FrameBuffer(glm::vec2 windowSize);
		
		unsigned int GetColorBuffer();
		
		void Draw() const;
		void Bind() const;
		void UnBind() const;

	};
	
} // namespace gl