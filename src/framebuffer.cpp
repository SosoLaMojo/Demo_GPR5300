#include <iostream>
#include <glad/glad.h>
#include "framebuffer.h"

namespace gl
{
	FrameBuffer::FrameBuffer(glm::vec2 windowSize)
	{
		float quadVertices[24] = {
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f
		};
		// FBO
		glGenFramebuffers(1, &FBO_);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
		// Generate texture
		glGenTextures(1, &texColorBuffer_);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer_, 0);
		// RBO
		glGenRenderbuffers(1, &RBO_);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO_);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer is not complete!\n";
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// quad VAO
		glGenVertexArrays(1, &quadVAO_);
		glBindVertexArray(quadVAO_);
		// quad VBO
		glGenBuffers(1, &quadVBO_);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0,
			2,
			GL_FLOAT,
			GL_FALSE,
			4 * sizeof(float),
			(void*)0);
		glVertexAttribPointer(1,
			2,
			GL_FLOAT,
			GL_FALSE,
			4 * sizeof(float),
			(void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	unsigned FrameBuffer::GetColorBuffer()
	{
		return texColorBuffer_;
	}

	void FrameBuffer::Draw() const
	{
		glBindVertexArray(quadVAO_);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void FrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
	}

	void FrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::IsError(const std::string& file, int line) const
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
	
} // namespace gl