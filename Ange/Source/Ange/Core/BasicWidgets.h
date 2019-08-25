#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Window.h"
#include "Ange/Core/Widgets.h"
#include "Ange/Core/Texture.h"
#include "Ange/Core/Font.h"
#include "Ange/Core/Themes.h"

namespace Ange {

	

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	/*!
	Widget - ghost. Serves as a description of a specific space and performs automatic recalculation
	of this space after changing the size of the parent.
	*/
	class AreaWidget : public BasicWidget2D
	{
		friend class VScroller;
	public:

		/*!
		Default constructor.
		*/
		AreaWidget(Window* window, const Widget2DProps& props);
		
		/*!
		Destructor.
		*/
		virtual ~AreaWidget();

		/*!
		Clones the object (usefull while making copy of object that is casted to its base).
		*/
		AreaWidget* Clone() const override;

		/*!
		Bindings override (connecting for: WindowResize, WindowClose).
		*/
		void EnableWidget() override;

		/*!
		Renders nothing for now.
		*/
		void Render() override;

	private:

		/*!
		This function do nothing.
		*/
		void BindBuffers() override;

	};

	/*!
	Widget, displays a solid color area. It can be used as a background in sub-widgets or windows.
	Specifying a nullptr value as a window is prohibited.
	Example (using widget as background for window):
	** Window window(nullptr, "Bg test", { {250,100}, {980,620}, WindowFlags::AutoInvokeRender});
	** Background bg(&window, {{0,0}, {0,0}, ResizePolicy::AutoFill});
	*/
	class Background : public BasicWidget2D
	{
	public:

		/*!
		Default constructor.
		*/
		Background(Window* window, const Widget2DProps& props, const BackgroundTheme& rectProps);

		/*!
		Delegating constructor with theme application.
		*/
		Background(Window* window, const Widget2DProps& props, const Theme& theme);

		/*!
		Copy constructor.
		*/
		Background(const Background& copy);
		
		/*!
		Destructor.
		*/
		virtual ~Background();

		/*!
		Assignment operator.
		*/
		Background& operator=(Background rhs);

		/*!
		Swap function.
		*/
		friend void swap(Background& first, Background& second) noexcept;

		/*!
		Clones the object (usefull while making copy of object that is casted to its base).
		*/
		Background* Clone() const override;

		/*!
		Returns the color of the widget.
		*/
		const Color& GetColor() const;

		/*!
		Sets the color of the widget.
		*/
		void SetColor(Color color);

		/*!
		Returns the border color of the widget.
		*/
		const Color& GetBorderColor() const;
		
		/*!
		Sets the border color of the widget.
		*/
		void SetBorderColor(Color color);

		/*!
		Returns the border size of the widget.
		*/
		const Dimension<int>& GetBorderSize() const;
		
		/*!
		Sets the widget border size.
		*/
		void SetBorderSize(Dimension<int> newBorderSize);

		/*!
		Renders the widget.
		*/
		void Render() override;

	private:

		/*!
		Creates OpenGL VAO and VBO.
		*/
		void CreateBuffers();
		
		/*!
		Binds the filled-in data arrays that are used in the rendering process.
		*/
		void BindBuffers() override;
		
		/*!
		Deletes OpenGL VAO and VBO.
		*/
		void Cleanup();

		/* Stores information about widget theme.*/
		BackgroundTheme m_RectangleTheme;

		/* OpenGL Vertex Array Object */
		GLuint m_VertexArrayId;
		
		/* OpenGL Vertex Buffer object */
		GLuint m_VertexBufferId;
	};

	using Rectangle2D = Background;

	//-------------------------------------------------------------------------------

	/*!
	Widget that displays an image that uses a loaded texture. Example:
	** Window window(nullptr, "Image test", { {250,100}, {980,620}, WindowFlags::AutoInvokeRender});
	** Texture texture("path.png");
	** Image image(&window, {{500, 250}, {0, 0}, ImageFlags::DetectSize}, ImageTheme(), &texture);
	*/
	class Image : public BasicWidget2D 
	{
	public:

		/*!
		Default constructor. Specifying a nullptr value as a texture is prohibited.
		*/
		Image(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ImageFlags::DetectSize),
			const ImageTheme& spriteProps = ImageTheme(),
			Texture* texture = nullptr
		);

		/*!
		Delegating constructor with theme application.
		*/
		Image(Window* window, const Widget2DProps& props, const Theme& theme, Texture* texture);

		/*!
		Copy constructor.
		*/
		Image(const Image& copy);

		/*!
		Destructor.
		*/
		virtual ~Image();

		/*!
		Assignment operator.
		*/
		Image& operator=(Image rhs);

		/*!
		Swap function.
		*/
		friend void swap(Image& first, Image& second) noexcept;

		/*!
		Clones the object (usefull while making copy of object that is casted to its base).
		*/
		Image* Clone() const override;

		/*!
		Returns the color of the widget.
		*/
		const Color& GetColor() const;

		/*!
		Sets the color of the widget.
		*/
		void SetColor(Color color);

		/*!
		Returns the border color of the widget.
		*/
		const Color& GetBorderColor() const;

		/*!
		Sets the border color of the widget.
		*/
		void SetBorderColor(Color color);

		/*!
		Returns the border size of the widget.
		*/
		const Dimension<int>& GetBorderSize() const;
		
		/*!
		Sets the widget border size.
		*/
		void SetBorderSize(Dimension<int> newBorderSize);

		/*!
		Sets a new texture. New texture should not be nullptr. The widget will recalculate
		its dimensions according to flags.
		*/
		void SetTexture(Texture* newTexture);

		/*!
		Returns a pointer to the used texture.
		*/
		const Texture* GetTexture() const;

		/*!
		Renders the widget.
		*/
		void Render() override;

	private:
		
		/*!
		Creates OpenGL VAO and VBO.
		*/
		void CreateBuffers();

		/*!
		Binds the filled-in data arrays that are used in the rendering process.
		*/
		void BindBuffers() override;

		/*!
		Deletes OpenGL VAO and VBO.
		*/
		void Cleanup();

		/* Stores information about widget theme.*/
		ImageTheme m_ImageTheme;

		/* OpenGL Vertex Array object. */
		GLuint m_VertexArrayId;

		/* OpenGL Vertex Buffer object. */
		GLuint m_VertexBufferId;

		/* OpenGL UV Buffer object. */
		GLuint m_UvBufferId;

		/* Texture used by the Image widget. */
		Texture* m_UsedTexture;
	};

	//-------------------------------------------------------------------------------

	/*!
	Widget that allows to display text on the screen.
	** Window window(nullptr, "Text test", { {250,100}, {980,620}, WindowFlags::AutoInvokeRender});
	** Font font("fontname.ttf");
	** font.LoadFontSize(12)
	** Text text(&window, {{500, 250}, {0, 0}, Anchor::Left|Anchor::Bottom}, {12, Color{255,0,0,255}, &font}, L"My Text");
	*/
	class Text : public BasicWidget2D
	{
		friend class SimpleInput;
	public:

		/*!
		Default constructor.
		*/
		Text(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Top),
			const TextTheme& textProps = TextTheme(),
			std::wstring text = L""
		);

		/*!
		Delegating constructor with theme application.
		*/
		Text(Window* window, const Widget2DProps& props, const Theme& theme, std::wstring text);

		/*!
		Copy constructor.
		*/
		Text(const Text& copy);

		/*!
		Default destructor.
		*/
		virtual ~Text();

		/*!
		Assignment operator.
		*/
		Text& operator=(Text rhs);

		/*!
		Swap function.
		*/
		friend void swap(Text& first, Text& second) noexcept;

		/*!
		Clones the object (usefull while making copy of object that is casted to its base).
		*/
		Text* Clone() const override;

		/*!
		Returns the used font size by the widget.
		*/
		const int GetFontSize() const;

		/*!
		Sets new font size for the widget.
		*/
		void SetFontSize(int newSize);

		/*!
		Returns text color data.
		*/
		const Color& GetColor() const;
		
		/*!
		Sets text color.
		*/
		void SetColor(Color color);

		/*!
		Sets new font for the widget.
		*/
		void SetFont(Font* newFont);

		/*!
		Returns pointer to the font used by widget.
		*/
		Font* GetFont() const;

		/*!
		Sets new text.
		*/
		void SetText(std::wstring newText);
		
		/*!
		Returns the text displayed by the widget in the form of std::wstring.
		*/
		std::wstring GetText() const;

		/*!
		Returns the text displayed by the widget in the form of std::wstring&. (Reference wersion.)
		*/
		const std::wstring& GetTextRef() const;

		/*!
		Sets text offset. Used mainly in SimpleInput widget.
		*/
		void SetDisplayOffset(Point<float> newOffset);
		
		/*!
		Returns the text offset. Used mailny in SimpleInput widget.
		*/
		const Point<float>& GetDisplayOffset() const;

		/*!
		Returns the width of the widget (the size of the longest line displayed).
		*/
		const Dimension<float> GetTextRealWidth() const;

		/*!
		Renders the widget.
		*/
		void Render() override;

		/*
		Function to implement????
		void SetMargin(Point<int> newMargin);
		const Point<int>& GetMargin() const;
		void SetSubStringToDisplay(int startPos, int endPos);
		std::tuple<int, int> GetCharsInWidth(int iMaxWidth);
		int GetCharPositionInPx(int charPosition);
		*/

	private:
		
		/*!
		Returns character identifier(position) and relative cursor position from the beggining
		in correlation to some specific point.
		*/
		std::tuple<int, float> GetPromptPosition(Point<int> relClickPosition);

		/*!
		Returns the position of the promp at the place of the text character.
		*/
		float GetPromptPosition(int idx);

		/*!
		Returns the cursor position based on the previous position and size of the next character.
		(Specify the position of the next character.)
		*/
		void GetNextPosition(int& charIdx, float& charPos);

		/*!
		Returns the cursor position based on the previous position and size of the previous character.
		(Specify the position of the previous character.)
		*/
		void GetPrevPosition(int& charIdx, float& charPos);

		/*!
		Based on the m_iFlags from m_Widget2DProps, and true widget dimensions calculates the widget's
		shift vector.
		*/
		void CalculateAnchorVec() override;

		/*!
		Returns the kerning value between two characters in the text.
		*/
		const float GetKerning(int idxPrev, int idxCur) const;

		/*!
		Creates OpenGL VAO and VBO.
		*/
		void CreateBuffers();

		/*!
		Fills-in m_Vertexs and m_Uvs arrays with appropriate data, then binds them to their VBO.
		*/
		void BindBuffers() override;

		/*!
		Deletes OpenGL VAO and VBO.
		*/
		void Cleanup();

		/* Stores information about widget theme and used font.*/
		TextTheme m_TextTheme;
		
		/* Actual text displayed by the widget. */
		std::wstring m_Text;

		/* Represents the true dimension of the widget. */
		Dimension<int> m_TrueDim;

		/* OpenGL Vertex Array id. */
		GLuint m_VertexArrayId;

		/* OpenGL Vertex Buffer id. */
		GLuint m_VertexBufferId;

		/* OpenGL UV Buffer id. */
		GLuint m_UvBufferId;

		/* Stores all vertices representing the text. */
		std::vector<float> m_Vertexs;

		/* Stores texture position for each vertex. */
		std::vector<float> m_UVs;

		/* Reserved. */
		Point<float> m_Offset;
	};


}
