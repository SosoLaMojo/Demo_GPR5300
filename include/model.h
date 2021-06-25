#pragma once
#include <tiny_obj_loader.h>
#include <string>
#include <vector>
#include "mesh2.h"
#include "material.h"
#include <cassert>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace gl
{
	class Model
	{
	public:
		std::vector<Mesh> meshes;
		std::vector<Material> materials;

		Model(const std::string& filename)
		{
			tinyobj::ObjReader reader;
			if (!reader.ParseFromFile(filename))
			{
				throw std::runtime_error("Cannot load file: " + filename);
			}
			auto& attrib = reader.GetAttrib();
			auto& shapes = reader.GetShapes();
			auto& materials = reader.GetMaterials();
			for (const auto& material : materials)
			{
				ParseMaterial(material);
			}
			for (const auto& shape : shapes)
			{
				ParseMesh(shape, attrib);

			}
		}
		Mesh GetMesh(unsigned int i)
		{
			return meshes[i];
		}
	private:
		void ParseMaterial(const tinyobj::material_t& material)
		{
			Material mat{};
			std::string path = "../data/textures/";
			mat.color_tex = Texture(path + material.diffuse_texname);
			mat.specular_tex = Texture(path + material.specular_texname);
			mat.specular_vec = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
			mat.specular_pow = material.shininess;
			materials.push_back(mat);
		}
		void ParseMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib)
		{
			unsigned int material_id = 0;
			std::vector<Vertex> vertices;
			std::vector<std::uint32_t> indices;
			int index_offset = 0;
			for (std::size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
			{
				int fv = shape.mesh.num_face_vertices[f];
				if (fv != 3) throw std::runtime_error("Should be triangles? ");

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					Vertex vertex{};
					// access to vertex
					tinyobj::index_t idx =
						shape.mesh.indices[index_offset + v];
					vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
					vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
					vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];
					vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
					vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
					vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
					vertex.texture.x =
						attrib.texcoords[2 * idx.texcoord_index + 0];
					vertex.texture.y =
						attrib.texcoords[2 * idx.texcoord_index + 1];
					vertices.push_back(vertex);
					indices.push_back(static_cast<int>(indices.size()));
				}
				index_offset += fv;
			}
			material_id = shape.mesh.material_ids[0];
			assert(index_offset == indices.size());
			meshes.emplace_back(vertices, indices, material_id);
		}
	};

	class Planet
	{
	public:
		Planet() {}

		Planet(std::string filepath, float rotationSpeedFactor, glm::vec3 spinRotationAxis, glm::vec3 transVec, float spinSpeedFactor) :
			rotationSpeedFactor_(rotationSpeedFactor),
			spinRotationAxis_(spinRotationAxis),
			transVec_(transVec),
			spinSpeedFactor_(spinSpeedFactor)
		{
			model_ = std::make_unique<Model>(filepath);
		}

		void Update(std::chrono::duration<float, std::ratio<1, 1>> dt, Shader& shader)
		{
			SetModelMatrix(dt);

			shader.SetMat4("inv_model", invModelMatrix_);
			shader.SetMat4("model", modelMatrix_);

			for (const auto& mesh : model_->meshes)
			{
				mesh.Bind();
				const auto& material = model_->materials[mesh.material_index];
				material.color_tex.Bind(0);
				material.specular_tex.Bind(1);
				shader.SetFloat("specular_pow", material.specular_pow);
				shader.SetVec3("specular_vec", material.specular_vec);
				glDrawElements(GL_TRIANGLES, mesh.nb_vertices, GL_UNSIGNED_INT, 0);
			}
		}

		void SetModelMatrix(std::chrono::duration<float, std::ratio<1, 1>> dt)
		{
			delta_time_ = dt.count();
			time_ += delta_time_;
			
			modelMatrix_ = glm::mat4(1.0f);
			modelMatrix_ = glm::rotate(modelMatrix_, time_ * rotationSpeedFactor_, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around specific point
			modelMatrix_ = glm::translate(modelMatrix_, transVec_);
			modelMatrix_ = glm::rotate(modelMatrix_, time_ * spinSpeedFactor_, spinRotationAxis_); //rotate around themself

			invModelMatrix_ = glm::transpose(glm::inverse(modelMatrix_));
		}

		glm::vec3 GetPosition()
		{
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(modelMatrix_, scale, rotation, translation, skew, perspective);

			return translation;
		}

		struct PlanetTransfom
		{
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 scale;
		};

		PlanetTransfom GetTransform()
		{
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(modelMatrix_, scale, rotation, translation, skew, perspective);

			PlanetTransfom result;
			result.translation = translation;
			result.scale = scale;
			result.rotation = rotation;

			return result;
		}

		/*void SetPosition(glm::vec3 position)
		{
			modelMatrix_ = glm::translate(modelMatrix_, glm::vec3(0.0f));
			modelMatrix_ = glm::translate(modelMatrix_, position);
		}*/

	private:

		float time_ = 0.0f;
		float delta_time_ = 0.0f;
		float spinSpeedFactor_ = 1.0f; // speed spin
		float rotationSpeedFactor_ = 1.0f; // speed rotate in world
		std::unique_ptr<Model> model_ = nullptr;

		glm::vec3 transVec_ = glm::vec3(0.0f, 0.0f, 0.0f); // transVec movement planet World
		glm::vec3 spinRotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f); // Axis rotation spin 
		glm::mat4 modelMatrix_ = glm::mat4(1.0f);
		glm::mat4 invModelMatrix_ = glm::mat4(1.0f);
		
	};

} // namespace gl