#pragma once

#include "Texture.h"
#include "Primitives.h"
#include "BasicWidgets.h"

namespace Ange {

	enum Components
	{
		RGB = 3,
		RGBA = 4
	};

	class Framebuffer
	{
	public:
		Framebuffer(Components components, Dimension<size_t> dim);
		~Framebuffer();

		void Bind();
		void Unbind();
		Texture* GetTexture();
		Components GetComponents();
		void SetDimension(Dimension<size_t> dim);

	private:

		int GetGLComponents();

		Components m_Components;

		GLuint m_giFBO;
		GLuint m_giDRO;

		bool m_bStatus;

		AreaWidget* m_ClipArea;
		Texture* m_Tex;
	};

}
