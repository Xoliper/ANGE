#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Modules/Window.h"
#include "Ange/Core/Modules/Graphics/Widgets.h"
#include "Ange/Core/Modules/Graphics/Texture.h"
#include "Ange/Core/Modules/Graphics/Font.h"

namespace Ange {

	

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	/*!
	Contains basic information about the colors of the widget.
	*/
	struct BackgroundProps
	{
		/*!
		Default constructor.
		*/
		BackgroundProps(
			Color baseColor = Color(255, 0, 0, 255),
			Color borderColor = Color(0, 0, 0, 255),
			Dimension<int> borderSize = Dimension<int>({ 0, 0 })
		);
		
		/* Background color. */
		Color BaseColor;
		
		/* Border color. */
		Color BorderColor;
		
		/* Border size in both direction (X and Y). */
		Dimension<int> BorderSize;
	};

	/*!
	Widget displaying a solid color in your area. It can be used as a background in sub-widgets or windows.
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
		Background(
			Window* window = nullptr,
			const Widget2DProps props = Widget2DProps({ 0,0 }, {0,0}, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const BackgroundProps rectProps = BackgroundProps()
		);

		/*!
		Copy constructor.
		*/
		Background(const Background& copy);
		
		/*!
		Destructor.
		*/
		~Background();

		/*!
		Assignment operator.
		*/
		Background& operator=(Background rhs);

		/*!
		Swap function.
		*/
		friend void swap(Background& first, Background& second) noexcept;

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
		const Dimension<int>& GetBoderSize() const;
		
		/*!
		Sets the widget border size.
		*/
		void SetBoderSize(Dimension<int> newBorderSize);

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
		BackgroundProps m_RectangleProps;

		/* Stores the information about the shift of the widget according to the Anchor flags. */
		Point<int> m_AnchorOffsets;

		/* Helps to draw the border. */
		Dimension<size_t> m_ClipArea;

		/* Helps to draw the border. */
		Point<int> m_ClipPos;

		//OpenGL Vertex Array Object 
		GLuint m_VertexArrayId;
		
		//OpenGL Vertex Buffer object 
		GLuint m_VertexBufferId;
	};

	using Rectangle2D = Background;

	//-------------------------------------------------------------------------------

	struct ImageProps
	{
		ImageProps(
			Texture* spriteTex = nullptr,
			Color spriteTint = Color(255, 255, 255, 255),
			Color borderColor = Color(0.0f, 0.0f, 0.0f, 0.0f),
			Dimension<int> borderSize = Dimension<int>({ 0, 0 })
		);

		Texture* ImageTexture;
		Color ImageTint;
		Color BorderColor;
		Dimension<int> BorderSize;
	};

	class Image : public BasicWidget2D 
	{
	public:
		Image(
			Window* window,
			Widget2DProps props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ImageFlags::DetectSize),
			const ImageProps& spriteProps = ImageProps()
		);
		~Image();

		const Color& GetColor() const;
		void SetColor(Color& color);

		const Color& GetBorderColor() const;
		void SetBorderColor(Color& color);

		const Dimension<int>& GetBoderSize() const;
		void SetBoderSize(Dimension<int> newBorderSize);

		const Texture* GetTexture() const;

		//Derived
		void Render() override;

	private:
		//Memory managment
		void CreateBuffers();
		void BindBuffers() override;
		void Cleanup();

		//Image-specific variables
		ImageProps m_SpriteProps;

		//VAO+VBO variables
		GLuint m_VertexArrayId;
		GLuint m_VertexBufferId;
		GLuint m_UvBufferId;
	};

	//-------------------------------------------------------------------------------

	struct TextProps
	{
		TextProps(Font* usedFont = nullptr, int size = 12, std::wstring text = L"", Color color = Color(255, 255, 255, 255))
			: iFontSize(size),
			UsedFont(usedFont),
			wsText(text),
			TextColor(color)
		{
		}

		int iFontSize;
		Font* UsedFont;
		std::wstring wsText;
		Color TextColor;
	};

	class Text : public BasicWidget2D
	{
		friend class SimpleInput;
	public:
		Text(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Top),
			const TextProps& textProps = TextProps()
		);
		~Text();

		//Set/Get methods
		const int GetFontSize() const;
		void SetFontSize(int newSize);

		const Color& GetColor() const;
		void SetColor(Color& color);

		void SetFont(Font* newFont);
		Font* GetFont() const;

		void SetText(std::wstring newText);
		std::wstring GetText() const;
		const std::wstring& GetTextRef() const;

		//void SetMargin(Point<int> newMargin);
		//const Point<int>& GetMargin() const;

		void SetDisplayOffset(Point<float> newOffset);
		const Point<float>& GetDisplayOffset() const;

		//Helper methods
		const Dimension<float> GetTextRealWidth() const;


//		void SetSubStringToDisplay(int startPos, int endPos);
//		std::tuple<int, int> GetCharsInWidth(int iMaxWidth);
//		int GetCharPositionInPx(int charPosition);
//		std::tuple<int, int, int> GetHorizontalScrollData(int charPosition, int iMaxWidth, bool mode);	//start, end, shift
		std::tuple<int, float> GetPromptPosition(Point<int> relClickPosition); //Returns char idx and rel position for cursor
		float GetPromptPosition(int idx);
		void GetNextPosition(int& charIdx, float& charPos);
		void GetPrevPosition(int& charIdx, float& charPos);

		//Derived & overrided
		void CalculateAnchorVec() override;
		void Render() override;

	private:
		//Private overrides
		void RecalculateMatrices() override;

		//Helper functions
		const float GetKerning(int idxPrev, int idxCur) const;

		//Memory managment
		void CreateBuffers();
		void BindBuffers() override;
		void Cleanup();

		//Text-specific variables
		TextProps m_TextProps;
		Dimension<int> m_TrueDim;

		//VAO+VBO variables
		GLuint m_VertexArrayId;
		GLuint m_VertexBufferId;
		GLuint m_UvBufferId;

		//Data
		std::vector<float> m_Vertexs;
		std::vector<float> m_UVs;

		//Point<int> m_Margins;
		Point<float> m_Offset;
	};


}
