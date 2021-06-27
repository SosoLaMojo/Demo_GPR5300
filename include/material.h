#pragma once
#include "texture.h"
#include <glm/glm.hpp>

namespace gl
{
	class Material
	{
	public:
		Texture color_tex;
		Texture specular_tex;
		glm::vec3 specular_vec;
		float specular_pow;
	};
} // namespace gl
