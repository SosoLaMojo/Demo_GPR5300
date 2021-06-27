#pragma once

namespace gl {
	class MeshSkybox
	{
	private:
		unsigned int skyboxVAO_ = 0;
		unsigned int skyboxVBO_ = 0;
		unsigned int skyboxTextureID;

	public:
		MeshSkybox();

		void Bind(unsigned int i = 0) const;
	};

} // End namespace gl.
