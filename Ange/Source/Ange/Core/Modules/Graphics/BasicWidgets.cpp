#include "Apch.h"
#include "BasicWidgets.h"
#include "Ange/Shaders/Quad.h"
#include "Ange/Shaders/Image.h"
#include "Ange/Shaders/Text.h"
#include "Ange/Core/Modules/Logger.h"


namespace Ange {
	
	BackgroundProps::BackgroundProps(Color baseColor, Color borderColor, Dimension<int> borderSize)
	{
		BaseColor = baseColor;
		BorderColor = borderColor;
		BorderSize = borderSize;
	}

	Background::Background(Window* window, const Widget2DProps props, const BackgroundProps rectProps) :
		BasicWidget2D(window, props),
		m_RectangleProps(rectProps)
	{
		if (window == nullptr) {
			std::string message = "[Background Constructor] A 'nullptr' was passed instead of a valid pointer to the Windows object.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		} else {
			m_WidgetType = WidgetType::Background;
			UseShader(Shader::Quad::Name);
			CreateBuffers();
			BindBuffers();
			EnableWidget();
			CalculateAnchorVec();
			SetPosition(m_Widget2DProps.Position);
		}
	}

	Background::Background(const Background& copy) :
		BasicWidget2D(copy),
		m_RectangleProps(copy.m_RectangleProps)
	{
		CreateBuffers();
		BindBuffers();
		EnableWidget();
	}

	Background& Background::operator=(Background rhs)
	{
		swap(*this, rhs);
		return *this;
	}


	Background::~Background()
	{
		DisableWidget();
		Cleanup();
	}

	void swap(Background & first, Background & second) noexcept
	{
		using std::swap;
		swap(first.m_VertexArrayId, second.m_VertexArrayId);
		swap(first.m_VertexBufferId, second.m_VertexBufferId);
		swap(first.m_RectangleProps, second.m_RectangleProps);
		swap(static_cast<BasicWidget2D&>(first), static_cast<BasicWidget2D&>(second));
	}

	const Color& Background::GetColor() const
	{
		return m_RectangleProps.BaseColor;
	}

	void Background::SetColor(Color color)
	{
		m_RectangleProps.BaseColor = color;
	}

	const Color& Background::GetBorderColor() const
	{
		return m_RectangleProps.BorderColor;
	}

	void Background::SetBorderColor(Color color)
	{
		m_RectangleProps.BorderColor = color;
	}

	const Dimension<int>& Background::GetBoderSize() const
	{
		return m_RectangleProps.BorderSize;
	}

	void Background::SetBoderSize(Dimension<int> newBorderSize)
	{
		m_RectangleProps.BorderSize = newBorderSize;
	}

	void Background::CreateBuffers()
	{
		glGenVertexArrays(1, &m_VertexArrayId);
		glBindVertexArray(m_VertexArrayId);
		glGenBuffers(1, &m_VertexBufferId);
	}

	void Background::BindBuffers()
	{
		float VertexData[18] =
		{
			-(float)m_Widget2DProps.Dimensions.tWidth*1.0f, -(float)m_Widget2DProps.Dimensions.tHeight * 1.0f, 0.0f,
			 (float)m_Widget2DProps.Dimensions.tWidth*1.0f, -(float)m_Widget2DProps.Dimensions.tHeight * 1.0f, 0.0f,
			-(float)m_Widget2DProps.Dimensions.tWidth*1.0f,  (float)m_Widget2DProps.Dimensions.tHeight * 1.0f, 0.0f,
			-(float)m_Widget2DProps.Dimensions.tWidth*1.0f,  (float)m_Widget2DProps.Dimensions.tHeight * 1.0f, 0.0f,
			 (float)m_Widget2DProps.Dimensions.tWidth*1.0f,  (float)m_Widget2DProps.Dimensions.tHeight * 1.0f, 0.0f,
			 (float)m_Widget2DProps.Dimensions.tWidth*1.0f, -(float)m_Widget2DProps.Dimensions.tHeight * 1.0f, 0.0f
		};

		glBindVertexArray(m_VertexArrayId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	}

	void Background::Cleanup()
	{
		glBindVertexArray(m_VertexArrayId);
		glDeleteBuffers(1, &m_VertexBufferId);
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}


	void Background::Render()
	{
		if (!m_ParentWindow->IfOpen()) {
			ANGE_WARNING("Trying to render on closed window!");
			return;
		}

		if (!m_Widget2DProps.bVisibility) return;
		RecalculateMatrices();

		glUseProgram(m_UsedShader->at("ShaderId"));
		glUniformMatrix4fv(m_UsedShader->at(Shader::Quad::Uniforms::MVP), 1, false, &m_Matrices.m4Mvp[0][0]);
		glUniform4fv(m_UsedShader->at(Shader::Quad::Uniforms::Tint), 1, &m_RectangleProps.BaseColor.GetVec4()[0]);
		glUniform4fv(m_UsedShader->at(Shader::Quad::Uniforms::BorderData), 1, &glm::vec4(((float)m_Widget2DProps.Dimensions.tWidth - (float)m_RectangleProps.BorderSize.tWidth), ((float)m_Widget2DProps.Dimensions.tHeight - (float)m_RectangleProps.BorderSize.tHeight), (float)m_RectangleProps.BorderSize.tWidth, (float)m_RectangleProps.BorderSize.tHeight)[0]);
		glUniform3fv(m_UsedShader->at(Shader::Quad::Uniforms::Anchor), 1, &m_Matrices.v3AnchorTranslation[0]);
		glUniform4fv(m_UsedShader->at(Shader::Quad::Uniforms::BorderColor), 1, &m_RectangleProps.BorderColor.GetVec4()[0]);
		//glUniform4fv(m_UsedShader->at(Shader::Quad::Uniforms::ClipArea), 1, &glm::vec4((float)m_ClipPos.tX, (float)m_ClipPos.tY, (float)m_ClipArea.tWidth, (float)m_ClipArea.tHeight)[0]);
		if (m_ParentWindow->GetWindowType() == WindowType::Child) {
			glUniform3fv(m_UsedShader->at(Shader::Quad::Uniforms::ChildPos), 1, &glm::vec3((float)m_ParentWindow->GetRealPosition().tX * 2, (float)m_ParentWindow->GetRealPosition().tY * 2, 0.0f)[0]);
		} else {
			glUniform3fv(m_UsedShader->at(Shader::Text::Uniforms::ChildPos), 1, &glm::vec3(0.0f, 0.0f, 0.0f)[0]);
		}

		// 1st attribute buffer : vertices

		glBindVertexArray(m_VertexArrayId);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,			// normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_CULL_FACE);
		glDisableVertexAttribArray(0);
	}

	//------------------------------------------------------------------------------
	//Image
	//------------------------------------------------------------------------------

	ImageProps::ImageProps(Texture* spriteTex, Color spriteTint, Color borderColor, Dimension<int> borderSize)
	{
		ImageTexture = spriteTex;
		ImageTint = spriteTint;
		BorderColor = borderColor;
		BorderSize = borderSize;
	}

	Image::Image(Window* window, Widget2DProps props, const ImageProps& spriteProps) :
		BasicWidget2D(window, props),
		m_ImageProps(spriteProps)
	{
		if (window == nullptr) {
			std::string message = "[Image Constructor] A 'nullptr' was passed instead of a valid pointer to the Windows object.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		} else {
			m_WidgetType = WidgetType::Image;
			UseShader(Shader::Image::Name);
			CreateBuffers();
			BindBuffers();
			EnableWidget();
			CalculateAnchorVec();
			SetPosition(m_Widget2DProps.Position);
		}
	}

	Image::Image(const Image& copy) :
		BasicWidget2D(copy),
		m_ImageProps(copy.m_ImageProps)
	{
		CreateBuffers();
		BindBuffers();
		EnableWidget();
	}

	Image& Image::operator=(Image rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	Image::~Image()
	{
		DisableWidget();
		Cleanup();
	}

	void swap(Image & first, Image & second) noexcept
	{
		using std::swap;
		swap(first.m_VertexArrayId, second.m_VertexArrayId);
		swap(first.m_VertexBufferId, second.m_VertexBufferId);
		swap(first.m_UvBufferId, second.m_UvBufferId);
		swap(first.m_ImageProps, second.m_ImageProps);
		swap(static_cast<BasicWidget2D&>(first), static_cast<BasicWidget2D&>(second));
	}

	const Color& Image::GetColor() const
	{
		return m_ImageProps.ImageTint;
	}

	void Image::SetColor(Color color)
	{
		m_ImageProps.ImageTint = color;
	}

	const Color& Image::GetBorderColor() const
	{
		return m_ImageProps.BorderColor;
	}

	void Image::SetBorderColor(Color color)
	{
		m_ImageProps.BorderColor = color;
	}

	const Dimension<int>& Image::GetBoderSize() const
	{
		return m_ImageProps.BorderSize;
	}

	void Image::SetBoderSize(Dimension<int> newBorderSize)
	{
		m_ImageProps.BorderSize = newBorderSize;
	}

	void Image::SetTexture(Texture* newTexture)
	{
		m_ImageProps.ImageTexture = newTexture;
		BindBuffers();
		CalculateAnchorVec();
	}

	const Texture* Image::GetTexture() const
	{
		return m_ImageProps.ImageTexture;
	}


	void Image::CreateBuffers()
	{
		glGenVertexArrays(1, &m_VertexArrayId);
		glBindVertexArray(m_VertexArrayId);
		glGenBuffers(1, &m_VertexBufferId);
		glGenBuffers(1, &m_UvBufferId);
	}

	void Image::BindBuffers()
	{
		//1.
		//"DetectSize": Anuluje "AutoFill"
		//	->workWidth & workHeight bazuje na Tex Dim
		//	->koordynaty UV standardowe

		//2.
		//"DetectSize" i "Repeat": Aktywuje "AutoFill"
		//	->workWidth & workHeight bazuje na m_Widget2DProps
		//	->koordynaty UV bazuj� na workXXX i Tex Dim

		//3.
		//"AutoFill": Dezaktywuje "DetectSize" i "Repeat"
		//	->workXXX bazuj� na m_Widget2DProps
		//	->koordynaty UV s� standardowe

		float workWidth = 0.0f;
		float workHeight = 0.0f;
		float uvWidth = 1.0f;
		float uvHeight = 1.0f;

		if (m_Widget2DProps.iFlags & ImageFlags::DetectSize && m_Widget2DProps.iFlags & ImageFlags::Repeat) {
			workWidth = (float)m_Widget2DProps.Dimensions.tWidth;
			workHeight = (float)m_Widget2DProps.Dimensions.tHeight;
			float hAmount = workWidth / m_ImageProps.ImageTexture->GetDimension().tWidth;
			float vAmount = workHeight / m_ImageProps.ImageTexture->GetDimension().tHeight;
			uvWidth = hAmount;
			uvHeight = vAmount;
		}
		else if (m_Widget2DProps.iFlags & ImageFlags::DetectSize) {
			m_ResizableProps.BaseDimension = m_ImageProps.ImageTexture->GetDimension();
			workWidth = (float)m_ImageProps.ImageTexture->GetDimension().tWidth;
			workHeight = (float)m_ImageProps.ImageTexture->GetDimension().tHeight;
			m_Widget2DProps.Dimensions = {(size_t)workWidth, (size_t)workHeight};
		}
		else if (m_Widget2DProps.iFlags & ResizePolicy::AutoFill) {
			workWidth = (float)m_Widget2DProps.Dimensions.tWidth;
			workHeight = (float)m_Widget2DProps.Dimensions.tHeight;
		}
		else {
			workWidth = (float)m_Widget2DProps.Dimensions.tWidth;
			workHeight = (float)m_Widget2DProps.Dimensions.tHeight;
		}


		float VertexData[18] =
		{
			-(float)workWidth, -(float)workHeight, 0.0f,
			 (float)workWidth, -(float)workHeight, 0.0f,
			-(float)workWidth,  (float)workHeight, 0.0f,
			-(float)workWidth,  (float)workHeight, 0.0f,
			 (float)workWidth, (float)workHeight, 0.0f,
			 (float)workWidth, -(float)workHeight, 0.0f
		};

		float UvData[12] = {
			0.0f, 0.0f,
			uvWidth, 0.0f,
			0.0f, uvHeight,
			0.0f, uvHeight,
			uvWidth, uvHeight,
			uvWidth, 0.0f
		};

		glBindVertexArray(m_VertexArrayId);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_UvBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(UvData), UvData, GL_STATIC_DRAW);
	}

	void Image::Cleanup()
	{
		glBindVertexArray(m_VertexArrayId);
		glDeleteBuffers(1, &m_VertexBufferId);
		glDeleteBuffers(1, &m_UvBufferId);
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	void Image::Render()
	{
		if (!m_ParentWindow->IfOpen()) {
			ANGE_WARNING("Trying to render on closed window!");
			return;
		}

		if (!m_Widget2DProps.bVisibility) return;
		RecalculateMatrices();

		glUseProgram(m_UsedShader->at("ShaderId"));
		glUniformMatrix4fv(m_UsedShader->at(Shader::Image::Uniforms::MVP), 1, false, &m_Matrices.m4Mvp[0][0]);
		glUniform4fv(m_UsedShader->at(Shader::Image::Uniforms::Tint), 1, &m_ImageProps.ImageTint.GetVec4()[0]);
		glUniform4fv(m_UsedShader->at(Shader::Image::Uniforms::BorderData), 1, &glm::vec4((float)m_Widget2DProps.Dimensions.tWidth - (float)m_ImageProps.BorderSize.tWidth, (float)m_Widget2DProps.Dimensions.tHeight - (float)m_ImageProps.BorderSize.tHeight, (float)m_ImageProps.BorderSize.tWidth, (float)m_ImageProps.BorderSize.tHeight)[0]);
		glUniform3fv(m_UsedShader->at(Shader::Image::Uniforms::Anchor), 1, &m_Matrices.v3AnchorTranslation[0]);
		glUniform4fv(m_UsedShader->at(Shader::Image::Uniforms::BorderColor), 1, &m_ImageProps.BorderColor.GetVec4()[0]);
		if (m_ParentWindow->GetWindowType() == WindowType::Child) {
			glUniform3fv(m_UsedShader->at(Shader::Image::Uniforms::ChildPos), 1, &glm::vec3((float)m_ParentWindow->GetRealPosition().tX * 2, (float)m_ParentWindow->GetRealPosition().tY * 2, 0.0f)[0]);
		} else {
			glUniform3fv(m_UsedShader->at(Shader::Text::Uniforms::ChildPos), 1, &glm::vec3(0.0f, 0.0f, 0.0f)[0]);
		}

		//Set texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ImageProps.ImageTexture->GetTexId());
		glUniform1i(m_UsedShader->at(Shader::Image::Uniforms::DiffuseTex), 0);

		// 1st attribute buffer : vertices

		glBindVertexArray(m_VertexArrayId);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,					// normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_UvBufferId);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,	                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_CULL_FACE);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}


	//------------------------------------------------------------------------------
	//Text
	//------------------------------------------------------------------------------

	TextProps::TextProps(Font* usedFont, int size, std::wstring text, Color color)
	{	
		iFontSize = size;
		UsedFont = usedFont;
		wsText = text;
		TextColor = color;
	}

	Text::Text(Window* window, const Widget2DProps& props, const TextProps& textProps) :
		BasicWidget2D(window, props),
		m_TextProps(textProps),
		m_Offset({0,0})
	{
		if (window == nullptr) {
			std::string message = "[Text Constructor] A 'nullptr' was passed instead of a valid pointer to the Windows object.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		} else {
			m_WidgetType = WidgetType::Text;
			UseShader(Shader::Text::Name);
			CreateBuffers();
			BindBuffers();
			EnableWidget();
			CalculateAnchorVec();
			SetPosition(m_Widget2DProps.Position);
		}
	}

	Text& Text::operator=(Text rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	Text::Text(const Text& copy):
		BasicWidget2D(copy),
		m_TextProps(copy.m_TextProps),
		m_Vertexs(copy.m_Vertexs),
		m_UVs(copy.m_UVs),
		m_TrueDim(copy.m_TrueDim)
	{
		CreateBuffers();
		BindBuffers();
		EnableWidget();
	}

	Text::~Text()
	{
		DisableWidget();
		Cleanup();
	}

	void swap(Text & first, Text & second) noexcept
	{
		using std::swap;
		swap(first.m_VertexArrayId, second.m_VertexArrayId);
		swap(first.m_VertexBufferId, second.m_VertexBufferId);
		swap(first.m_UvBufferId, second.m_UvBufferId);
		swap(first.m_TextProps, second.m_TextProps);
		swap(first.m_TrueDim, second.m_TrueDim);
		swap(first.m_Vertexs, second.m_Vertexs);
		swap(first.m_UVs, second.m_UVs);
		swap(static_cast<BasicWidget2D&>(first), static_cast<BasicWidget2D&>(second));
	}

	void Text::Cleanup()
	{
		glBindVertexArray(m_VertexArrayId);
		glDeleteBuffers(1, &m_VertexBufferId);
		glDeleteBuffers(1, &m_UvBufferId);
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	const int Text::GetFontSize() const
	{
		return m_TextProps.iFontSize;
	}


	void Text::SetFontSize(int newSize)
	{
		m_TextProps.iFontSize = newSize;
		SetText(m_TextProps.wsText);
	}

	const Color& Text::GetColor() const
	{
		return m_TextProps.TextColor;
	}

	void Text::SetColor(Color color)
	{
		m_TextProps.TextColor = color;
	}

	void Text::SetFont(Font* newFont)
	{
		m_TextProps.UsedFont = newFont;
		BindBuffers();
	}

	Font* Text::GetFont() const
	{
		return m_TextProps.UsedFont;
	}

	void Text::SetText(std::wstring newText)
	{
		m_TextProps.wsText = newText;
		BindBuffers();
	}

	std::wstring Text::GetText() const
	{
		return m_TextProps.wsText;
	}

	const std::wstring& Text::GetTextRef() const
	{
		return m_TextProps.wsText;
	}

	/*void Text::SetMargin(Point<int> newMargin)
	{
		m_Margins = newMargin;
	}

	const Point<int>& Text::GetMargin() const
	{
		return m_Margins;
	}*/

	void Text::SetDisplayOffset(Point<float> newOffset)
	{
		m_Offset = newOffset;
	}

	const Point<float>& Text::GetDisplayOffset() const
	{
		return m_Offset;
	}

	void Text::CreateBuffers()
	{
		glGenVertexArrays(1, &m_VertexArrayId);
		glBindVertexArray(m_VertexArrayId);
		glGenBuffers(1, &m_VertexBufferId);
		glGenBuffers(1, &m_UvBufferId);
	}

	const float Text::GetKerning(int idxPrev, int idxCur) const
	{
		if (m_TextProps.UsedFont == nullptr) return 0.0f;
		if (m_TextProps.UsedFont->m_HasKernings && idxCur > 0) {
			FT_Vector delta;
			FT_UInt  prev = FT_Get_Char_Index(m_TextProps.UsedFont->m_Face, (FT_ULong)m_TextProps.wsText.at(idxPrev));
			FT_UInt  cur = FT_Get_Char_Index(m_TextProps.UsedFont->m_Face, (FT_ULong)m_TextProps.wsText.at(idxCur));
			FT_Get_Kerning(m_TextProps.UsedFont->m_Face, prev, cur, FT_KERNING_DEFAULT, &delta);
			//Fix krening
			return (float)(delta.x >> 6);
		}
		return 0.0f;
	}


	void Text::BindBuffers()
	{
		if (m_TextProps.UsedFont == nullptr) return;
		if (!m_TextProps.UsedFont->IsSizeLoaded(m_TextProps.iFontSize)){
			ANGE_WARNING("Used font size is not loaded, Text will not be displayed. Font: [%s] Size: [%i]", m_TextProps.UsedFont->m_sFontPath.c_str(), m_TextProps.iFontSize);
			return;
		}

		//Check if string is empty
		//Note that Render() function also have this check
		if (m_TextProps.wsText.empty()) return

		//Resize containers
		m_Vertexs.clear();
		m_Vertexs.resize(18 * m_TextProps.wsText.size(), 0.0f);
		m_UVs.clear();
		m_UVs.resize(12 * m_TextProps.wsText.size(), 0.0f);

		//Prepare counters
		int gVertexCounter = 0, gUVCounter = 0;
		float xCursor = 0.0f, yCursor = 0.0f;
		int lastSpace = -1;

		//Prepade data
		for (size_t s = 0; s < m_TextProps.wsText.size(); s++) {

			//Newline support
			if ((m_Widget2DProps.iFlags & TextFlags::EnableNewlineChar) && m_TextProps.wsText[s] == '\n') {
				xCursor = 0.0f;
				yCursor -= m_TextProps.UsedFont->GetLineHeight(m_TextProps.iFontSize);
				if ((int)yCursor / -1 >= (int)m_Widget2DProps.Dimensions.tHeight) {
					ANGE_WARNING("Cannot fit text in 'Text' widget (height). Part of it will not be displayed.");
					break;
				}
				break;
			}

			//Get glyph data
			auto glyph = m_TextProps.UsedFont->GetGlyph(m_TextProps.iFontSize, m_TextProps.wsText[s]);
			auto texData = m_TextProps.UsedFont->GetAtlasTexture(m_TextProps.iFontSize)->GetDimension();

			//Check if not exceed width
			if(m_Widget2DProps.iFlags & TextFlags::Multiline)
			if ((int)(xCursor + glyph->fLeft + glyph->fWidth) >= (int)m_Widget2DProps.Dimensions.tWidth) {
				xCursor = 0.0f;
				yCursor -= m_TextProps.UsedFont->GetLineHeight(m_TextProps.iFontSize);
				if ((int)yCursor / -1 >= (int)m_Widget2DProps.Dimensions.tHeight) {
					ANGE_WARNING("Cannot fit text in 'Text' widget (height). Part of it will not be displayed.");
					break;
				}
				//Fix everything after last detected space
				if (lastSpace != -1) {
					gVertexCounter -= 18 * ((int)s - lastSpace);
					gUVCounter -= 12 * ((int)s - lastSpace);
					s = lastSpace;
					continue;
				}
			}

			//Calculate new pos
			float glyphLeft = (xCursor + glyph->fLeft) * 2;
			float glyphTop = (-yCursor - glyph->fTop) * 2;
			float glyphWidth = (glyph->fWidth) * 2;
			float glyphHeight = (glyph->fHeight) * 2;

			//Save last space position
			if (m_TextProps.wsText[s] == ' ') lastSpace = (int)s;

			//Advance cursor
			xCursor += glyph->fAdvX;
			yCursor += glyph->fAdvY;


			//Add kerning
			float kerning = GetKerning((int)s - 1, (int)s);
			xCursor += kerning;
			glyphLeft += kerning;

			m_Vertexs[gVertexCounter] = glyphLeft;
			m_Vertexs[gVertexCounter + 1] = -glyphTop;
			m_Vertexs[gVertexCounter + 2] = 0.0f;
			m_Vertexs[gVertexCounter + 3] = glyphLeft + glyphWidth;
			m_Vertexs[gVertexCounter + 4] = -glyphTop;
			m_Vertexs[gVertexCounter + 5] = 0.0f;
			m_Vertexs[gVertexCounter + 6] = glyphLeft;
			m_Vertexs[gVertexCounter + 7] = -glyphTop - glyphHeight;
			m_Vertexs[gVertexCounter + 8] = 0.0f;
			m_Vertexs[gVertexCounter + 9] = glyphLeft + glyphWidth;
			m_Vertexs[gVertexCounter + 10] = -glyphTop;
			m_Vertexs[gVertexCounter + 11] = 0.0f;
			m_Vertexs[gVertexCounter + 12] = glyphLeft;
			m_Vertexs[gVertexCounter + 13] = -glyphTop - glyphHeight;
			m_Vertexs[gVertexCounter + 14] = 0.0f;
			m_Vertexs[gVertexCounter + 15] = glyphLeft + glyphWidth;
			m_Vertexs[gVertexCounter + 16] = -glyphTop - glyphHeight;
			m_Vertexs[gVertexCounter + 17] = 0.0f;

			//L-D
			m_UVs[gUVCounter] = glyph->fOffsetX;
			m_UVs[gUVCounter + 1] = glyph->fOffsetY;
			//R-D
			m_UVs[gUVCounter + 2] = glyph->fOffsetX + glyph->fWidth / texData.tWidth;
			m_UVs[gUVCounter + 3] = glyph->fOffsetY;
			//L-G
			m_UVs[gUVCounter + 4] = glyph->fOffsetX;
			m_UVs[gUVCounter + 5] = glyph->fOffsetY + (glyph->fHeight + 0) / texData.tHeight;
			m_UVs[gUVCounter + 6] = glyph->fOffsetX + glyph->fWidth / texData.tWidth;
			m_UVs[gUVCounter + 7] = glyph->fOffsetY;
			m_UVs[gUVCounter + 8] = glyph->fOffsetX;
			m_UVs[gUVCounter + 9] = glyph->fOffsetY + (glyph->fHeight + 0) / texData.tHeight;
			//R-G
			m_UVs[gUVCounter + 10] = glyph->fOffsetX + glyph->fWidth / texData.tWidth;
			m_UVs[gUVCounter + 11] = glyph->fOffsetY + (glyph->fHeight + 0) / texData.tHeight;

			gVertexCounter += 18;
			gUVCounter += 12;
		}

		m_TrueDim.Set((int)xCursor, m_TextProps.UsedFont->GetLineHeight(m_TextProps.iFontSize));
		CalculateAnchorVec();

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, 18 * m_TextProps.wsText.size() * sizeof(GLfloat), &m_Vertexs[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_UvBufferId);
		glBufferData(GL_ARRAY_BUFFER, 12 * m_TextProps.wsText.size() * sizeof(GLfloat), &m_UVs[0], GL_STATIC_DRAW);

	}

	void Text::CalculateAnchorVec()
	{
		m_Matrices.v3AnchorTranslation = glm::vec3(-(float)m_TrueDim.tWidth, -(float)m_TrueDim.tHeight / 2, 0.0f);
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			m_Matrices.v3AnchorTranslation += glm::vec3((float)m_TrueDim.tWidth, 0.0f, 0.0f);
		}
		else if (m_Widget2DProps.iFlags & Anchor::Right) {
			m_Matrices.v3AnchorTranslation += glm::vec3(-(float)m_TrueDim.tWidth, 0.0f, 0.0f);
		}
		if (m_Widget2DProps.iFlags & Anchor::Bottom) {
			m_Matrices.v3AnchorTranslation += glm::vec3(0.0f, (float)m_TrueDim.tHeight, 0.0f);
		}
		else if (m_Widget2DProps.iFlags & Anchor::Top) {
			m_Matrices.v3AnchorTranslation += glm::vec3(0.0f, -(float)m_TrueDim.tHeight, 0.0f);
		}
		m_Matrices.v3AnchorTranslation = glm::floor(m_Matrices.v3AnchorTranslation);
		if (abs((int)m_Matrices.v3AnchorTranslation.x % 2) == 1) m_Matrices.v3AnchorTranslation.x += 1;
		if (abs((int)m_Matrices.v3AnchorTranslation.y % 2) == 1) m_Matrices.v3AnchorTranslation.y += 1;
	}

	void Text::RecalculateMatrices()
	{
		if (m_Widget2DProps.bIfChanged == true){

			//Fix alignment
			auto realDim = m_ParentWindow->GetPhysicalWindowDim();
			int fixX = 0, fixY = 0;
			if (realDim.tWidth % 2 == 1) fixX -= 1;
			if (realDim.tHeight % 2 == 1) fixY -= 1;
			if (realDim.tWidth % 2 == 1 || realDim.tHeight % 2 == 1) {
				m_Matrices.m4Translation = glm::translate(
					glm::mat4(1.0f),
					glm::vec3(2 * m_Widget2DProps.Position.tX + fixX, 2 * m_Widget2DProps.Position.tY + fixY, 0.0f)
				);
			}
			//Standard mat calc.
			m_Matrices.m4Model = m_Matrices.m4Translation*m_Matrices.m4Rotation*m_Matrices.m4Scale;
			m_Widget2DProps.bIfChanged = false;
		}
		m_Matrices.m4Mvp = m_ParentWindow->GetWorld()->GetGuiMatrices().m4Projection*m_ParentWindow->GetWorld()->GetGuiMatrices().m4View*m_Matrices.m4Model;

	}

	void Text::Render()
	{

		if (!m_ParentWindow->IfOpen()) {
			ANGE_WARNING("Trying to render on closed window!");
			return;
		}

		if (!m_Widget2DProps.bVisibility) return;
		if (m_TextProps.UsedFont == nullptr || m_TextProps.UsedFont->IsSizeLoaded(m_TextProps.iFontSize) == false) {
			return;
		}
		if (m_TextProps.wsText.empty()) return;
		RecalculateMatrices();

		glUseProgram(m_UsedShader->at("ShaderId"));
		glUniformMatrix4fv(m_UsedShader->at(Shader::Text::Uniforms::MVP), 1, false, &m_Matrices.m4Mvp[0][0]);
		glUniform4fv(m_UsedShader->at(Shader::Text::Uniforms::Tint), 1, &m_TextProps.TextColor.GetVec4()[0]);
		glUniform3fv(m_UsedShader->at(Shader::Text::Uniforms::Anchor), 1, &m_Matrices.v3AnchorTranslation[0]);
		//glUniform4fv(m_UsedShader->at(Shader::Text::Uniforms::ClipArea), 1, &glm::vec4(m_Margins.tX * 2, m_Margins.tY * 2, m_Widget2DProps.Dimensions.tWidth * 2 - m_Margins.tX * 4, m_Widget2DProps.Dimensions.tHeight * 2)[0]);
		glUniform4fv(m_UsedShader->at(Shader::Text::Uniforms::ClipArea), 1, &glm::vec4(0, 0, m_Widget2DProps.Dimensions.tWidth * 2, m_Widget2DProps.Dimensions.tHeight * 2)[0]);
		glUniform2fv(m_UsedShader->at(Shader::Text::Uniforms::DispOffset), 1, &glm::vec2(m_Offset.tX*2, m_Offset.tY*2)[0]);

		if (m_ParentWindow->GetWindowType() == WindowType::Child) {
			glm::vec3 cPos = glm::vec3((float)m_ParentWindow->GetRealPosition().tX * 2, (float)m_ParentWindow->GetRealPosition().tY * 2, 0.0f);
			cPos = glm::floor(cPos);
			glUniform3fv(m_UsedShader->at(Shader::Text::Uniforms::ChildPos), 1, &cPos[0]);
		} else {
			glUniform3fv(m_UsedShader->at(Shader::Text::Uniforms::ChildPos), 1, &glm::vec3(0.0f, 0.0f, 0.0f)[0]);
			//glUniform3fv(m_UsedShader->at(Shader::Text::Uniforms::ChildPos), 1, &glm::vec3(m_Widget2DProps.Position.tX * 2, m_Widget2DProps.Position.tY * 2, 0.0f)[0]);
		}

		//Set texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextProps.UsedFont->GetAtlasTexture(m_TextProps.iFontSize)->GetTexId());
		glUniform1i(m_UsedShader->at(Shader::Text::Uniforms::DiffuseTex), 0);

		// 1st attribute buffer : vertices

		glBindVertexArray(m_VertexArrayId);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,					// normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_UvBufferId);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,	                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, 6 * (GLsizei)m_TextProps.wsText.size());
		glEnable(GL_CULL_FACE);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}

	std::tuple<int, float> Text::GetPromptPosition(Point<int> relClickPosition)
	{
		if (m_TextProps.UsedFont == nullptr) return std::make_tuple(0, 0.0f);
		float xCursor = 0.0f;
		int pos = 0;
		for (size_t s = 0; s < m_TextProps.wsText.size(); s++) {
			auto glyph = m_TextProps.UsedFont->GetGlyph(m_TextProps.iFontSize, m_TextProps.wsText[s]);
			if (xCursor + glyph->fAdvX / 2 > relClickPosition.tX) { //First half of Glyph
				break;
			}

			//Get glyph Adv and kerning
			xCursor += glyph->fAdvX + GetKerning((int)s - 1, (int)s);

			if (xCursor - glyph->fAdvX / 2 > relClickPosition.tX){ //Second half of Glyph
				++pos;
				break;
			}
			++pos;
		}
		return std::make_tuple(pos, xCursor-1);
	}

	float Text::GetPromptPosition(int idx)
	{
		if (m_TextProps.UsedFont == nullptr) return 0.0f;
		if (idx > (int)m_TextProps.wsText.size() || idx < 0){
			ANGE_WARNING("GetPromptPosition 'idx' out of Text range! [%i]", idx);
			return 0.0f;
		}
		float xCursor = 0.0f;
		for (size_t s = 0; s < (size_t)idx; s++) {
			auto glyph = m_TextProps.UsedFont->GetGlyph(m_TextProps.iFontSize, m_TextProps.wsText[s]);
			xCursor += glyph->fAdvX + GetKerning((int)s - 1, (int)s);
		}
		return xCursor-1;
	}


	void Text::GetNextPosition(int& charIdx, float& charPos)
	{
		if (m_TextProps.UsedFont == nullptr) return;
		if (charIdx < (int)m_TextProps.wsText.size() && charIdx >= 0) {
			auto glyph = m_TextProps.UsedFont->GetGlyph(m_TextProps.iFontSize, m_TextProps.wsText[charIdx]);
			charPos += glyph->fAdvX + GetKerning(charIdx - 1, charIdx);
			++charIdx;
		}
	}

	void Text::GetPrevPosition(int& charIdx, float& charPos)
	{
		if (m_TextProps.UsedFont == nullptr) return;
		if (charIdx-1 < (int)m_TextProps.wsText.size() && charIdx >= 0) {
			--charIdx;
			auto glyph = m_TextProps.UsedFont->GetGlyph(m_TextProps.iFontSize, m_TextProps.wsText[charIdx]);
			charPos -= glyph->fAdvX + GetKerning(charIdx - 1, charIdx);
		}
	}


	const Dimension<float> Text::GetTextRealWidth() const
	{
		if (m_TextProps.UsedFont == nullptr) return {0.0f,0.0f};
		Dimension<float> ret;
		ret.Set(0.0f, 0.0f);
		for (size_t s = 0; s < m_TextProps.wsText.size(); s++) {
			auto glyph = m_TextProps.UsedFont->GetGlyph(m_TextProps.iFontSize, m_TextProps.wsText[s]);
			ret.tWidth += glyph->fAdvX;
		}
		return ret;
	}

}
