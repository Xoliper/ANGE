#include "Apch.h"
#include "Framebuffer.h"

namespace Ange {

	Framebuffer::Framebuffer(Components components, Dimension<size_t> dim)
	{
		m_Components = components;

		//Create framebuffer
		glGenFramebuffers(1, &m_giFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_giFBO);

		// The texture we're going to render to
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GetGLComponents(), (int)dim.tWidth, (int)dim.tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		m_Tex = new Texture();
		m_Tex->m_iChannels = (int)m_Components;
		m_Tex->m_Dimension = dim;
		m_Tex->m_iTextureId = texture;
		m_Tex->m_sTexturePath = "[Framebuffer]";

		//Configure stuff
		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			m_bStatus = false;
		} else {
			m_bStatus = true;
		}

	}


	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_giFBO);
		delete m_Tex;
	}

	int Framebuffer::GetGLComponents()
	{
		if (m_Components == Components::RGB) {
			return GL_RGB;
		} else if (m_Components == Components::RGBA) {
			return GL_RGBA;
		}
		return 0;
	}

	void Framebuffer::SetDimension(Dimension<size_t> dim)
	{
		glBindTexture(GL_TEXTURE_2D, m_Tex->m_iTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GetGLComponents(), (int)dim.tWidth, (int)dim.tHeight, 0, GetGLComponents(), GL_UNSIGNED_BYTE, 0);
		m_Tex->m_Dimension = dim;
	}


	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_giFBO);
		//glBindRenderbuffer(GL_RENDERBUFFER, m_giDRO);
		//glBindTexture(GL_TEXTURE_2D, m_Tex->m_iTextureId);
	}

	void Framebuffer::Unbind()
	{
		//Unbind everything
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture* Framebuffer::GetTexture()
	{
		return m_Tex;
	}

	Components Framebuffer::GetComponents()
	{
		return m_Components;
	}

}
