#pragma once
#include <string>
#include <fstream>
#include "stb_image.h"

namespace gl {
	
	class Texture {
	public:
		unsigned int id;
		Texture();

		Texture(const std::string& file_name);

		void Bind(unsigned int i = 0) const;

		void UnBind();
	protected:
		// TODO move this function somewhere else, STATIC function, for testing class and not instance
		void IsError(const char* file, int line) const;
	};

} // End namespace gl.
