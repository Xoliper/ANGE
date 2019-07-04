#include "Apch.h"
#include "Font.h"
#ifdef ANGE_PLATFORM_LINUX
#include <X11/Xlib.h>
#endif

namespace Ange {

	//Static initialization -----------------------------------------

	bool Font::s_FT2Initialized = false;
	FT_Library Font::s_FTLibrary;


	GlyphData::GlyphData()
	{
		fAdvX = fAdvY = fWidth = fHeight = 0.0f;
		fLeft = fTop = fOffsetX = fOffsetY = 0.0f;
		iDescriptor = 0;
	}

	//---------------------------------------------------------------
	//SubFont
	//---------------------------------------------------------------
	SubFont::SubFont(Font* parentFont, int fontSize, Dimension<size_t> texDimension) :
		m_ParentFontObj(parentFont),
		m_iFontSize(fontSize),
		m_AtlasDim(texDimension),
		m_TexOffsets({ 0,0 }),
		m_AtlasTexture(nullptr)
	{

		//Create Texture
		int dpi = m_ParentFontObj->GetFontDpi();
		m_AtlasTexture = new Texture;
		FT_Set_Char_Size(m_ParentFontObj->m_Face, 0, m_iFontSize * 64, dpi, dpi);
		m_iFontHeight = (m_ParentFontObj->m_Face->size->metrics.height / 64);

		glGenTextures(1, &m_AtlasTexture->m_iTextureId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindTexture(GL_TEXTURE_2D, m_AtlasTexture->m_iTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (GLsizei)m_AtlasDim.tWidth, (GLsizei)m_AtlasDim.tHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		m_AtlasTexture->m_Dimension = m_AtlasDim;
		m_AtlasTexture->m_sTexturePath = m_ParentFontObj->m_sFontPath;

	}

	SubFont::~SubFont()
	{
		delete m_AtlasTexture;
	}

	const GlyphData* SubFont::GetGlyph(int ch)
	{
		//Check if already loaded
		auto glyphIt = m_GlyphsInfo.find(ch);
		if (glyphIt == m_GlyphsInfo.end()) {
			glBindTexture(GL_TEXTURE_2D, m_AtlasTexture->m_iTextureId);
			glyphIt = AddGlyph(ch);
			if (glyphIt == m_GlyphsInfo.end()) {
				char number[6]; snprintf(number, 6, "%d", ch);
				ANGE_ERROR("Cannot reach glyph [%s] in font [%s].", number, m_ParentFontObj->m_sFontPath.c_str());
				return nullptr;
			}
		}
		return &glyphIt->second;
	}

	Texture* SubFont::GetAtlasTexture()
	{
		return m_AtlasTexture;
	}

	const int SubFont::GetLineHeight() const
	{
		return m_iFontHeight;
	}


	std::map<int, GlyphData>::iterator SubFont::AddGlyph(int ch)
	{
		//Set size
		int dpi = m_ParentFontObj->GetFontDpi();
		FT_Set_Char_Size(m_ParentFontObj->m_Face, 0, m_iFontSize * 64, dpi, dpi);

		//Check if already loaded
		if (m_GlyphsInfo.find(ch) != m_GlyphsInfo.end()) {
			char number[6]; snprintf(number, 6, "%d", ch);
			ANGE_WARNING("Glyph [%s] in font [%s] already loaded.", number, m_ParentFontObj->m_sFontPath.c_str());
			return m_GlyphsInfo.end();
		}

		//Load Glyph
		unsigned long cIdx = FT_Get_Char_Index(m_ParentFontObj->m_Face, ch);
		FT_Load_Glyph(m_ParentFontObj->m_Face, cIdx, FT_LOAD_DEFAULT | FT_LOAD_COLOR);
		FT_Render_Glyph(m_ParentFontObj->m_Face->glyph, FT_RENDER_MODE_NORMAL);
		FT_GlyphSlot gs = m_ParentFontObj->m_Face->glyph;

		//Read glyph data
		GlyphData tempGlyph;
		tempGlyph.fAdvX = (float)(gs->advance.x >> 6);
		tempGlyph.fAdvY = (float)(gs->advance.y >> 6);
		tempGlyph.fWidth = (float)(gs->bitmap.width);
		tempGlyph.fHeight = (float)(gs->bitmap.rows);
		tempGlyph.fLeft = (float)(gs->bitmap_left);
		tempGlyph.fTop = (float)(gs->bitmap_top);

		if ((int)(m_TexOffsets.tX + gs->bitmap.width + 2) > m_AtlasDim.tWidth) {
			m_TexOffsets.tX = 2;
			m_TexOffsets.tY += GetLineHeight() + 2;
			if ((m_TexOffsets.tY + GetLineHeight() + 2) > (int)m_AtlasDim.tHeight) {
				char number[6]; snprintf(number, 6, "%d", ch);
				ANGE_WARNING("No space left on font [%s] texture while adding a new character [%s]. Font size: [%i]", m_ParentFontObj->m_sFontPath.c_str(), number, m_iFontSize);
				return m_GlyphsInfo.end();
			}
		}

		tempGlyph.fOffsetX = static_cast<float>(m_TexOffsets.tX) / m_AtlasDim.tWidth;
		tempGlyph.fOffsetY = static_cast<float>(m_TexOffsets.tY) / m_AtlasDim.tHeight;
		tempGlyph.iDescriptor = cIdx;

		std::pair<std::map<int, GlyphData>::iterator, bool> ret = m_GlyphsInfo.insert(std::pair<int, GlyphData>(ch, tempGlyph));
		if (ret.second == false) {

			char number[6]; snprintf(number, 6, "%d", ch);
			ANGE_WARNING("Error occured in font [%s] while adding a new character [%s].", m_ParentFontObj->m_sFontPath.c_str(), number);
			return m_GlyphsInfo.end();
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, m_TexOffsets.tX, m_TexOffsets.tY, gs->bitmap.width, gs->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, gs->bitmap.buffer);
		m_TexOffsets.tX += gs->bitmap.width + 2;

		return ret.first;

	}

	//---------------------------------------------------------------
	//Font
	//---------------------------------------------------------------

	Font::Font(std::string fontPath, int iFlags):
		m_sFontPath(fontPath),
		m_iFlags(iFlags)
	{
		//Initialize FT2
		if (s_FT2Initialized == false){
			if (FT_Init_FreeType(&s_FTLibrary)) {
				ANGE_ERROR("Cannot initialize FreeType library!");
			} else {
				s_FT2Initialized = true;
			}
		}

		//Check filepath
		struct stat fcStruct;
		int accessibility = (stat(m_sFontPath.c_str(), &fcStruct) == 0);
		if (accessibility == false){
			//Try to pull this file from Platform-Specific Fonts folder
			const size_t slashPos = m_sFontPath.find_last_of("\\/");
			if (std::string::npos != slashPos)
			{
				m_sFontPath.erase(0, slashPos + 1);
			}
			#ifdef ANGE_PLATFORM_WINDOWS
			m_sFontPath = "C:\\Windows\\Fonts\\" + m_sFontPath;
			#elif ANGE_PLATFORM_LINUX
			m_sFontPath = "/usr/share/fonts/TTF/" + m_sFontPath;
			#endif
			accessibility = (stat(m_sFontPath.c_str(), &fcStruct) == 0);
			if (accessibility == false){
				ANGE_WARNING("Cannot find font file: [%s]", m_sFontPath.c_str());
				return;
			}
		}

		//Create face
		if (FT_New_Face(s_FTLibrary, m_sFontPath.c_str(), 0, &m_Face)) {
			ANGE_WARNING("Cannot open font file: [%s]", m_sFontPath.c_str());
			return;
		}

		//Check if face have kernings and setup unicode
		m_HasKernings = FT_HAS_KERNING(m_Face);
		FT_Select_Charmap(m_Face, ft_encoding_unicode);

		ANGE_INFO("Font [%s] opened. Kerning support: [%s]", m_sFontPath.c_str(), m_HasKernings ? "Yes" : "No");
	}

	Font::~Font()
	{
		for (auto it : m_SubFonts){
			delete it.second;
		}
		m_SubFonts.clear();

		s_FT2Initialized = false;
		FT_Done_FreeType(s_FTLibrary);
	}

	void Font::LoadFontSize(int fontSize, int preloadAmount, Dimension<size_t> maxFontDims)
	{
		if (m_SubFonts.find(fontSize) == m_SubFonts.end()) {
			auto it = m_SubFonts.insert(std::pair<int, SubFont*>(fontSize, new SubFont(this, fontSize, maxFontDims)));
			if (it.second == true) {
				for (int i = 0; i < preloadAmount; i++) {
					it.first->second->AddGlyph(i);
				}
			} else {
				ANGE_WARNING("Cannot load font size. Font: [%s] Size: [%i]", m_sFontPath.c_str(), fontSize);
			}
		}
	}

	bool Font::IsSizeLoaded(int fontSize)
	{
		auto it = m_SubFonts.find(fontSize);
		if (it == m_SubFonts.end()) return false;
		return true;
	}


	void Font::ReleaseFontSize(int fontSize)
	{
		auto it = m_SubFonts.find(fontSize);
		if (it != m_SubFonts.end()) {
			delete it->second;
		}
		m_SubFonts.erase(fontSize);
	}

	const GlyphData* Font::GetGlyph(int fontSize, int ch)
	{
		auto it = m_SubFonts.find(fontSize);
		if (it == m_SubFonts.end()) return nullptr;
		return it->second->GetGlyph(ch);
	}

	Texture* Font::GetAtlasTexture(int fontSize)
	{
		auto it = m_SubFonts.find(fontSize);
		if (it == m_SubFonts.end()) return nullptr;
		return it->second->GetAtlasTexture();
	}

	const int Font::GetLineHeight(int fontSize) const
	{
		auto it = m_SubFonts.find(fontSize);
		if (it == m_SubFonts.end()) return 0;
		return it->second->GetLineHeight();
	}

	std::map<int, GlyphData>::iterator Font::AddGlyph(int fontSize, int ch)
	{
		auto it = m_SubFonts.find(fontSize);
		if (it != m_SubFonts.end()) return it->second->m_GlyphsInfo.end();
		return it->second->AddGlyph(ch);
	}

	const int Font::GetFontDpi() const
	{
		int dpi = 0;
#ifdef ANGE_PLATFORM_WINDOWS
		SetProcessDPIAware(); //true
		HDC screen = GetDC(NULL);
		double hPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSX);
		double vPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSY);
		ReleaseDC(NULL, screen);

#elif ANGE_PLATFORM_LINUX
		Display* disp = XOpenDisplay(NULL);
		int scr = 0;
 		double hPixelsPerInch = ((double) DisplayWidth(disp,scr)) * 25.4 / ((double) DisplayWidthMM(disp,scr));
        double vPixelsPerInch = ((double) DisplayHeight(disp,scr)) * 25.4 / ((double) DisplayHeightMM(disp,scr));
#endif
		return (int)((hPixelsPerInch + vPixelsPerInch) * 0.5);
	}

}
