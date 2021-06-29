#pragma once
#include <tiny_obj_loader.h>
#include <string>
#include <vector>
#include <chrono>
#include "mesh.h"
#include "material.h"

namespace gl {
	
	class Model
	{
	public:
		std::vector<Mesh> meshes;
		std::vector<Material> materials;

		Model(const std::string& filename);

		Mesh GetMesh(unsigned int i);

	private:
		void ParseMaterial(const tinyobj::material_t& material);
		void ParseMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib);
	};

} // namespace gl