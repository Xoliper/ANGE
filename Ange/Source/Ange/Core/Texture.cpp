#include "Apch.h"
#include "Texture.h"

namespace Ange {

	void ReadArrayPNGHelper(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		MemoryReaderState *f = (MemoryReaderState*)png_get_io_ptr(png_ptr);
		if (length > (f->m_Bufsize - f->m_CurrentPos)) {
			ANGE_WARNING("Read error while iterating through array. [%p]", data);
			return;
		}
		memcpy(data, f->m_Buffer + f->m_CurrentPos, (size_t)length);
		f->m_CurrentPos += (png_uint_32)length;
	}


	struct jpegErrorManager {
		/* "public" fields */
		struct jpeg_error_mgr pub;
		/* for return to caller */
		jmp_buf setjmp_buffer;
	};

	char jpegLastErrorMsg[JMSG_LENGTH_MAX];

	void jpegErrorExit(j_common_ptr cinfo)
	{
		/* cinfo->err actually points to a jpegErrorManager struct */
		jpegErrorManager* myerr = (jpegErrorManager*)cinfo->err;
		/* note : *(cinfo->err) is now equivalent to myerr->pub */

		/* output_message is a method to print an error message */
		/*(* (cinfo->err->output_message) ) (cinfo);*/

		/* Create the message */
		(*(cinfo->err->format_message)) (cinfo, jpegLastErrorMsg);

		/* Jump to the setjmp point */
		longjmp(myerr->setjmp_buffer, 1);

	}


	Texture::Texture()
	{
		m_Dimension.Set(0, 0);
		m_iTextureId = -1;
		m_iChannels = 4;
	}
	
	Texture::Texture(std::string path) : Texture()
	{
		Load(path);
	}
	
	Texture::~Texture()
	{
		Cleanup();
	}

	void Texture::Cleanup()
	{
		if (m_iTextureId != -1)
		{
			glDeleteTextures(1, &m_iTextureId);
			m_iTextureId = -1;
			m_sTexturePath = "";
			m_iChannels = 4;
		}
	}

	bool Texture::IsLoaded() const
	{
		if (m_iTextureId != -1) {
			return true;
		}
		return false;
	}

	GLuint Texture::GetTexId() const
	{
		ANGE_M1_TEST(m_iTextureId, "Texture is not loaded!");
		return m_iTextureId;
	}

	Dimension<size_t> Texture::GetDimension() const
	{
		return m_Dimension;
	}

	std::string Texture::GetSourcePath() const
	{
		return m_sTexturePath;
	}

	bool Texture::Load(std::string path)
	{
		Cleanup();

		FILE *fp = fopen(path.c_str(), "rb");
		if (!fp) {
			ANGE_WARNING("Cannot find texture file to open. [%s]", path.c_str());
			return false;
		}

		//CHECK FOR PNG
		png_byte header[8];
		fread(header, 1, 8, fp);
		int is_png = !png_sig_cmp(header, 0, 8);
		if (!is_png) {
			//CHECK FOR JPEG,BMP,GIF
			if (!fseek(fp, 0, SEEK_SET)) {
				struct jpeg_decompress_struct info; //for our jpeg info
				//struct jpeg_error_mgr err;			//the error handler
				//info.err = jpeg_std_error(&err);
			
				jpegErrorManager jerr;
				info.err = jpeg_std_error(&jerr.pub);
				jerr.pub.error_exit = jpegErrorExit;
				
				/* Establish the setjmp return context for my_error_exit to use. */
				if (setjmp(jerr.setjmp_buffer)) {
					/* If we get here, the JPEG code has signaled an error. */
					//cerr << jpegLastErrorMsg << endl;
					jpeg_destroy_decompress(&info);
					fclose(fp);
					ANGE_WARNING("Unsupported texture format. [%s]", path.c_str());
					return false;
				}

				jpeg_create_decompress(&info);		//fills info structure
				jpeg_stdio_src(&info, fp);
				if (jpeg_read_header(&info, false) == JPEG_HEADER_OK)
				{
					ANGE_INFO("libJPEG [%s]", path.c_str());

					jpeg_start_decompress(&info);

					int w = info.output_width;
					int h = info.output_height;
					int numChannels = info.num_components; // 3 = RGB, 4 = RGBA
					unsigned long dataSize = w * h * numChannels;

					// read RGB(A) scanlines one at a time into jdata[]
					unsigned char *data = new unsigned char[dataSize];
					unsigned char* rowptr;

					//Read data & flip it vertically for OpenGL
					while ((int)info.output_scanline < h) {
						rowptr = (JSAMPROW)&data[(info.image_height-1-info.output_scanline)*w*numChannels];
						jpeg_read_scanlines(&info, &rowptr, 1);
					}

					jpeg_finish_decompress(&info);

					m_Dimension.Set(w, h);
					m_iChannels = numChannels;

					GLuint format = GL_RGB;
					if (m_iChannels == 1) {
						format = GL_RED;
						m_iChannels = 1;
					} else if (m_iChannels == 4) {
						m_iChannels = GL_RGBA;
					}

					glGenTextures(1, &m_iTextureId);
					glBindTexture(GL_TEXTURE_2D, m_iTextureId);

					glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

					fclose(fp);
					delete[] data;

				}	else {
					//NON SUPPORTED FILE
					ANGE_WARNING("Unsupported texture format. [%s]", path.c_str());
					return false;
				}
			}
		} else {
			//PNG PROCESSING
			ANGE_INFO("libPNG [%s]", path.c_str());

			//create png struct
			png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png_ptr) {
				fclose(fp);
				ANGE_WARNING("Error while loading texture file. (1) [%s]", path.c_str());
				return false;
			}

			//create png info struct
			png_infop info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr) {
				png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
				fclose(fp);
				ANGE_WARNING("Error while loading texture file. (2) [%s]", path.c_str());
				return false;
			}

			//create png info struct
			png_infop end_info = png_create_info_struct(png_ptr);
			if (!end_info) {
				png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
				fclose(fp);
				ANGE_WARNING("Error while loading texture file. (3) [%s]", path.c_str());
				return false;
			}

			//png error stuff, not sure libpng man suggests this.
			if (setjmp(png_jmpbuf(png_ptr))) {
				png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
				fclose(fp);
				ANGE_WARNING("Error while loading texture file. (4) [%s]", path.c_str());
				return false;
			}

			//init png reading
			png_init_io(png_ptr, fp);

			//let libpng know you already read the first 8 bytes
			png_set_sig_bytes(png_ptr, 8);

			// read all the info up to the image data
			png_read_info(png_ptr, info_ptr);

			//variables to pass to get info
			int bit_depth, color_type;
			png_uint_32 twidth, theight;

			if (color_type == PNG_COLOR_TYPE_GRAY)  png_set_palette_to_rgb(png_ptr);
			if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) png_set_add_alpha(png_ptr, 255, PNG_FILLER_AFTER);

			// get info about png
			png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);
		

			//update width and height based on png info
			m_Dimension.Set(twidth, theight);

			// Update the png info struct.
			png_read_update_info(png_ptr, info_ptr);

			// Row size in bytes.
			int rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);

			// Allocate the image_data as a big block, to be given to opengl
			png_byte *image_data = new png_byte[rowbytes * theight];
			if (!image_data) {
				//clean up memory and close stuff
				png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
				fclose(fp);
				ANGE_WARNING("Cannot allocate memory for texture. (1) [%s]", path.c_str());
				return false;
			}

			//row_pointers is for pointing to image_data for reading the png with libpng
			png_bytep *row_pointers = new png_bytep[theight];
			if (!row_pointers) {
				//clean up memory and close stuff
				png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
				delete[] image_data;
				fclose(fp);
				ANGE_WARNING("Cannot allocate memory for texture. (2) [%s]", path.c_str());
				return false;
			}
			// set the individual row_pointers to point at the correct offsets of image_data
			for (int i = 0; i < (int)theight; ++i)
				row_pointers[theight - 1 - i] = image_data + i * rowbytes;

			//read the png into image_data through row_pointers
			png_read_image(png_ptr, row_pointers);

			//Fix grayscale
			GLuint format = GL_RGBA;
			if (color_type == PNG_COLOR_TYPE_GRAY){
				format = GL_RED;
				m_iChannels = 1;
			}

			//Now generate the OpenGL texture object
			glGenTextures(1, &m_iTextureId);
			glBindTexture(GL_TEXTURE_2D, m_iTextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, twidth, theight, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			delete[] image_data;
			delete[] row_pointers;
			fclose(fp);
		}

		return true;
	}


	bool Texture::LoadFromMemory(unsigned char * data, size_t size)
	{
		Cleanup(); 

		if (data == nullptr) {
			ANGE_WARNING("Data cannot be nullptr. [%p]", data);
			return false;
		}

		//CHECK FOR PNG
		png_byte header[8];
		memcpy(header, data, 8);
		int is_png = !png_sig_cmp(header, 0, 8);
		if (!is_png) {
			//CHECK FOR JPEG,BMP,GIF
			struct jpeg_decompress_struct info; //for our jpeg info
			struct jpeg_error_mgr err;			//the error handler
			info.err = jpeg_std_error(&err);
			jpeg_create_decompress(&info);		//fills info structure
			jpeg_mem_src(&info, data, (unsigned long)size);
			//jpeg_stdio_src(&info, fp);
			if (jpeg_read_header(&info, false) == JPEG_HEADER_OK)
			{
				ANGE_INFO("libJPEG [%p]", data);

				jpeg_start_decompress(&info);

				int w = info.output_width;
				int h = info.output_height;
				int numChannels = info.num_components; // 3 = RGB, 4 = RGBA
				unsigned long dataSize = w * h * numChannels;

				// read RGB(A) scanlines one at a time into jdata[]
				unsigned char *data_out = new unsigned char[dataSize];
				unsigned char* rowptr;

				/*while (info.output_scanline < h) {
					rowptr = data + info.output_scanline * w * numChannels;
					jpeg_read_scanlines(&info, &rowptr, 1);
				}*/

				//Read data & flip it vertically for OpenGL
				while ((int)info.output_scanline < h) {
					rowptr = (JSAMPROW)&data_out[(info.image_height - 1 - info.output_scanline)*w*numChannels];
					jpeg_read_scanlines(&info, &rowptr, 1);
				}

				jpeg_finish_decompress(&info);

				m_Dimension.Set(w, h);
				m_iChannels = numChannels;

				GLuint format = GL_RGB;
				if (m_iChannels == 1) {
					format = GL_RED;
					m_iChannels = 1;
				} else if (m_iChannels == 4) {
					m_iChannels = GL_RGBA;
				}

				glGenTextures(1, &m_iTextureId);
				glBindTexture(GL_TEXTURE_2D, m_iTextureId);
	
				glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data_out);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				delete[] data_out;

				} else {
					//NON SUPPORTED FILE
					ANGE_WARNING("Unsupported texture format. [%p]", data);
					return false;
				}
			
		} else {
			//PNG PROCESSING
			ANGE_INFO("libPNG [%p]", data);

			//create png struct
			png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png_ptr) {
				ANGE_WARNING("Error while loading texture file. (1)");
				return false;
			}

			//create png info struct
			png_infop info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr) {
				png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
				ANGE_WARNING("Error while loading texture file. (2)");
				return false;
			}

			//create png info struct
			png_infop end_info = png_create_info_struct(png_ptr);
			if (!end_info) {
				png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
				ANGE_WARNING("Error while loading texture file. (3)");
				return false;
			}

			//png error stuff, not sure libpng man suggests this.
			if (setjmp(png_jmpbuf(png_ptr))) {
				png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
				ANGE_WARNING("Error while loading texture file. (4)");
				return false;
			}

			MemoryReaderState memory_reader_state;
			// png_source is array which have png data
			memory_reader_state.m_Buffer = (png_bytep)data;
			memory_reader_state.m_Bufsize = (png_uint_32)size;
			memory_reader_state.m_CurrentPos = 8;
			// set our own read_function
			png_set_read_fn(png_ptr, &memory_reader_state, ReadArrayPNGHelper);
			png_set_sig_bytes(png_ptr, 8);

			// read all the info up to the image data
			png_read_info(png_ptr, info_ptr);

			//variables to pass to get info
			int bit_depth, color_type;
			png_uint_32 twidth, theight;

			if (color_type == PNG_COLOR_TYPE_GRAY)  png_set_palette_to_rgb(png_ptr);
			if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) png_set_add_alpha(png_ptr, 255, PNG_FILLER_AFTER);

			// get info about png
			png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);


			//update width and height based on png info
			m_Dimension.Set(twidth, theight);

			// Update the png info struct.
			png_read_update_info(png_ptr, info_ptr);

			// Row size in bytes.
			int rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);

			// Allocate the image_data as a big block, to be given to opengl
			png_byte *image_data = new png_byte[rowbytes * theight];
			if (!image_data) {
				//clean up memory and close stuff
				png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
				ANGE_WARNING("Cannot allocate memory for texture. (1)");
				return false;
			}

			//row_pointers is for pointing to image_data for reading the png with libpng
			png_bytep *row_pointers = new png_bytep[theight];
			if (!row_pointers) {
				//clean up memory and close stuff
				png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
				delete[] image_data;
				ANGE_WARNING("Cannot allocate memory for texture. (2)");
				return false;
			}
			// set the individual row_pointers to point at the correct offsets of image_data
			for (int i = 0; i < (int)theight; ++i)
				row_pointers[theight - 1 - i] = image_data + i * rowbytes;

			//read the png into image_data through row_pointers
			png_read_image(png_ptr, row_pointers);

			//Fix grayscale
			GLuint format = GL_RGBA;
			if (color_type == PNG_COLOR_TYPE_GRAY) {
				format = GL_RED;
				m_iChannels = 1;
			}

			//Now generate the OpenGL texture object
			glGenTextures(1, &m_iTextureId);
			glBindTexture(GL_TEXTURE_2D, m_iTextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, twidth, theight, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			delete[] image_data;
			delete[] row_pointers;
		}

		return true;
	}

	void Texture::Load32BitRaw(unsigned char * bytes, int width, int height)
	{
		ANGE_WARNING("Using unsafe 'Load32BitRaw' function.");
		Cleanup();

		glGenTextures(1, &m_iTextureId);
		glBindTexture(GL_TEXTURE_2D, m_iTextureId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		m_Dimension.Set(width, height);
		ANGE_WARNING("Texture PROBABLY loaded. Data address: [%p]", bytes);
	}


	bool Texture::Save(std::string path)
	{
		if (m_iTextureId <= 0 || path.size() == 0) return false;

		int i = 0;
		int pitch = m_iChannels * m_Dimension.tWidth;
		
		FILE* fp = nullptr;
		png_structp png_ptr = nullptr;
		png_infop info_ptr = nullptr;
		png_bytep* row_pointers = nullptr;

		fp = fopen(path.c_str(), "wb");
		if (nullptr == fp) {
			ANGE_WARNING("Cannot open output file for Save() funciton. [%s]", path.c_str());
			return false;
		}

		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (nullptr == png_ptr) {
			ANGE_WARNING("Unexpected error [1] occured in Save() function. [%s]", path.c_str());
			return false;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (nullptr == info_ptr) {
			ANGE_WARNING("Unexpected error [2] occured in Save() function. [%s]", path.c_str());
			return false;
		}

		int colorType = 0;
		int glColorType = 0;
		if (m_iChannels == 3) {
			colorType = PNG_COLOR_TYPE_RGB;
			glColorType = GL_RGB;
		} else if(m_iChannels == 4) {
			colorType = PNG_COLOR_TYPE_RGBA;
			glColorType = GL_RGBA;
		}

		png_set_IHDR(png_ptr,
			info_ptr,
			m_Dimension.tWidth,
			m_Dimension.tHeight,
			8,                 /* e.g. 8 */
			colorType,                /* PNG_COLOR_TYPE_{GRAY, PALETTE, RGB, RGB_ALPHA, GRAY_ALPHA, RGBA, GA} */
			PNG_INTERLACE_NONE,       /* PNG_INTERLACE_{NONE, ADAM7 } */
			PNG_COMPRESSION_TYPE_BASE,
			PNG_FILTER_TYPE_BASE
		);


		//Read data from texture
		unsigned char *data = new unsigned char[m_iChannels*m_Dimension.tHeight*m_Dimension.tWidth];
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_iTextureId);
		glGetTexImage(GL_TEXTURE_2D,
			0,
			glColorType,
			GL_UNSIGNED_BYTE,
			data
		);

		row_pointers = new png_bytep[sizeof(png_bytep) * (int)m_Dimension.tHeight];
		for (i = 0; i < (int)m_Dimension.tHeight; ++i) {
			row_pointers[(int)m_Dimension.tHeight-i-1] = data + i * pitch;
		}

		png_init_io(png_ptr, fp);
		png_set_rows(png_ptr, info_ptr, row_pointers);
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);


		if (nullptr != fp) {
			fclose(fp);
			fp = nullptr;
		}

		if (nullptr != png_ptr) {

			if (nullptr == info_ptr) {
				ANGE_WARNING("Unexpected error [3] occured in Save() function. [%s]", path.c_str());
				return false;
			}

			png_destroy_write_struct(&png_ptr, &info_ptr);
			png_ptr = nullptr;
			info_ptr = nullptr;
		}

		if (nullptr != row_pointers) {
			delete [] row_pointers;
			row_pointers = nullptr;
		}

		if (nullptr != data) {
			delete[] data;
			data = nullptr;
		}

		return true;
	}


}
