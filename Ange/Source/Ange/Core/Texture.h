#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Window.h"
#include "Ange/Core/Logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <png.h>

#include <stdio.h>
#include <setjmp.h>
#include <GL/gl.h>
#include <string>

namespace Ange {

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

#ifdef __clang__
	//using png_size_t = unsigned long;
#endif

	class AreaWidget;

	/*
	Stores the position of the pointer in the array.
	*/
	typedef struct {
		png_bytep m_Buffer;
		png_uint_32 m_Bufsize;
		png_uint_32 m_CurrentPos;
	} MemoryReaderState;

	/*
	Helper function that allows to read PNG files in the convention from the bottom to up.
	*/
	void ReadArrayPNGHelper(png_structp png_ptr, png_bytep data, png_size_t length);


	/*!
	Allows to load textures into the GPU's memory.
	*/
	class Texture
	{
		friend class SubFont;
		friend class Font;
		friend class Framebuffer;
		friend class Widget2D;
	public:

		/*!
		Default constructor. The object created in this way is unusable until any loading function is called.
		Example of use:
		**Texture texture;
		**texture.Load32BitRaw(storedData, width, height);
		*/
		Texture();

		/*!
		Constructor with the file path from which the texture will be created.
		*/
		Texture(std::string path);

		/*!
		No copy constructor for this moment.
		*/
		Texture(const Texture& copy) = delete;

		/*!
		Destructor - frees allocated memory.
		*/
		~Texture();

		/*
		Load the texture into memory. If the object has previously loaded some texture, it will be removed
		and replaced with the new one.
		*/
		bool Load(std::string path);

		/*!
		Save stored texture data into PNG file.
		*/
		bool Save(std::string path);

		/*!
		Load the texture into memory. Version with a compressed binary data input (RGBA). If the object has previously
		loaded some texture, it will be removed and replaced with the new one.
		*/
		bool LoadFromMemory(unsigned char * data, size_t size);

		/*!
		Load the texture into memory. Version with a decompressed binary data input (RGBA). If the object has previously
		loaded some texture, it will be removed and replaced with the new one.
		*/
		void Load32BitRaw(unsigned char * bytes, int width, int height);


		/*!
		Returns the texture loading status. A value of true means that the texture has been correctly loaded.
		*/
		bool IsLoaded() const;

		/*!
		Returns the texture ID associated with the GPU.
		*/
		GLuint GetTexId() const;

		/*!
		Returns the dimensions of the loaded texture.
		*/
		Dimension<size_t> GetDimension() const;

		/*!
		Returns the path of the loaded file.
		*/
		std::string GetSourcePath() const;

		/*!
		Creates new Texture from clipped area. Notice: this function returns new allocated object. You need to destroy it.
		*/
		Texture* GetSubTexture(AreaWidget* clipArea);


	private:

		/*!
		Deallocates memory and resets variables.
		*/
		void Cleanup();


		/* Stores the dimensions of the loaded texture. */
		Dimension<size_t> m_Dimension;

		/* Stores the path of the loaded file. */
		std::string m_sTexturePath;

		/* Stores the texture ID associated with the GPU/OpenGL */
		GLuint m_iTextureId;

		/* Stores information about generated texture format. */
		int m_iChannels;

	};

}
