#pragma once

#include <string>
#include <fstream>
#include <glad/glad.h>
#include "stb_image.h"

namespace gl {

	class TextureCubeMaps {
	public:
		
		unsigned int textureID;

		TextureCubeMaps() {}
		TextureCubeMaps(const std::string& file_name)
		{
			// for texture skybox
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		}
	};

} // End namespace gl.
