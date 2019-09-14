#pragma once

#include "Texture.h"
#include "Primitives.h"

namespace Ange {

	class Framebuffer
	{
	public:
		Framebuffer(Dimension<size_t> dim);
		~Framebuffer();

		void Bind();
		void Unbind();
		Texture* GetTexture();
		void SetDimension(Dimension<size_t> dim);

	private:

		GLuint m_giFBO;
		GLuint m_giDRO;

		bool m_bStatus;

		Texture* m_Tex;
	};

}
