#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Modules/Logger.h"
#include "Ange/Core/Modules/Graphics/Texture.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <string>
#include <map>

namespace Ange {

	//----------------------------------------------------------
	//Structs
	//----------------------------------------------------------

	/*!
	It stores all glyph data in relation to the created atlas texture file.
	*/
	struct GlyphData
	{
		/*!
		Initialized all values to 0/0.0f.
		*/
		GlyphData();

		/* Describes the cursor shift after drawing the glyph. */
		float fAdvX, fAdvY;
		
		/* Describes the dimensions of the glyph. */
		float fWidth, fHeight;

		/* Describes the glyph relative position. */
		float fLeft, fTop;

		/* Describes the position in atlas texture. */
		float fOffsetX, fOffsetY;
		
		/* Describes the id of the character associated with the glyph. */
		int iDescriptor;
	};


	//----------------------------------------------------------
	//Classes
	//----------------------------------------------------------

	class Font;

	/*!
	Inaccessable class whose objects are used in 'main' Font class. Directly represents a font with
	a specific size. Responsible for creating texture atlas.
	*/
	class SubFont
	{
		friend class Font;
	private:

		/*!
		Private constructor. Setups a texture for specific font size. Allows to create textures with
		specific dimensions.
		*/
		SubFont(Font* parent, int fontSize, Dimension<size_t> texDimension);
		
		/*!
		No copy constructor for now.
		*/
		SubFont(const SubFont& copy) = delete;
		
		/*!
		Destructor - deallocates memory.
		*/
		~SubFont();
	
		/*!
		Returns glyph data.
		*/
		const GlyphData* GetGlyph(int ch);

		/*!
		Returns the texture used in the SubFont. 
		*/
		Texture* GetAtlasTexture();
		
		/*!
		Returns new line height for SubFont.
		*/
		const int GetLineHeight() const;
		
		/*!
		Adds a glyph (from the character) to a SubFont.
		*/
		std::map<int, GlyphData>::iterator	AddGlyph(int ch);

		/* Stores font size. */
		int m_iFontSize;

		/* Stores font new line height. */
		int m_iFontHeight;

		/* Stores glyphs data. */
		std::map<int, GlyphData> m_GlyphsInfo;
		
		/* Stores a handle (pointer) to the parent Font object. */
		Font* m_ParentFontObj;

		/* Stores atlas texture dimensions. */
		Dimension<size_t> m_AtlasDim;

		/* Describes the current cursor position on the texture to allow further glyphs to be added. */
		Point<int> m_TexOffsets;

		/*  Stores a handle (pointer) to the Texture object. */
		Texture* m_AtlasTexture;
	};


	/*!
	The font management class. Allows to load several font sizes at the same time.
	*/
	class Font
	{
		friend class SubFont;
		friend class Label;
		friend class Text;
	public:

		/*!
		Default constructor
		*/
		Font(std::string fontPath, int iFlags = 0);
		
		/*!
		No copy constructor for now.
		*/
		Font(const Font& copy) = delete;
		
		/*!
		Descturctor. Release all allocated memory & destroys SubFont objects.
		*/
		~Font();

		/*!
		Loads a specific font size into memory. Allows to automatically load some glyphs.
		*/
		void LoadFontSize(int fontSize, int preloadAmount = 0, Dimension<size_t> maxFontDims = {512, 512});
		
		/*!
		Checks whether the font size is already loaded.
		*/
		bool IsSizeLoaded(int fontSize);
		
		/*!
		Removes a font with a specific size from memory.
		*/
		void ReleaseFontSize(int fontSize);

		/*!
		Returns the DPI of the screen.
		*/
		const int GetFontDpi() const;

		/*!
		Returns glyph data for a specific font size.
		*/
		const GlyphData* GetGlyph(int fontSize, int ch);

		/*!
		Returns atlas texture for a specific font size.
		*/
		Texture* GetAtlasTexture(int fontSize);

		/*!
		Returns new line height for a specific font size.
		*/
		const int GetLineHeight(int fontSize) const;

		/*!
		Adds a glyph (from the character) to a font of a specific size.
		*/
		std::map<int, GlyphData>::iterator AddGlyph(int fontSize, int ch);

	private:

		/* Stores flags. Reserved. */
		int m_iFlags; //Reserved
		
		/* Stores the path of the loaded file. */
		std::string m_sFontPath;

		/* It stores a data of specific sizes of a given font. */
		std::map<int, SubFont*> m_SubFonts;

		/* FreeType variable that stores font Face. */
		FT_Face m_Face;
		
		/* FreeType variable that describes whether the font supports kernings. */
		FT_Bool m_HasKernings;

		/* A handle to a FreeType library instance. */
		static FT_Library s_FTLibrary;
		
		/* Describes whether the FreeType library is initialized or not. */
		static bool s_FT2Initialized;
	};

}
