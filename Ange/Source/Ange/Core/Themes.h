#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Font.h"

namespace Ange {

	//------------------------------------------------------------------------------------------------------
	//Classes
	//------------------------------------------------------------------------------------------------------

	/*!
	The basic structure describing the appearance of a rectangular space.
	*/
	struct RectTheme
	{
		/* Default constructor. */
		RectTheme(
			Color tint = Color(255,255,255,255),
			Color borderColor = Color(128,128,128,255)
		){
			Tint = tint;
			BorderColor = borderColor;
		}

		/* Background color. */
		Color Tint;

		/* Border color. */
		Color BorderColor;
	};

	//----------------------------------------------------------------------------------


	/*!
	Contains basic information about the colors of the Background widget.
	*/
	struct BackgroundTheme
	{
		/* Default constructor. */
		BackgroundTheme(
			Color tint = Color(255,255,255,255),
			Color borderColor = Color(128,128,128,255),
			Dimension<int> borderSize = Dimension<int>(1,1)
		){
			Base.Tint = tint;
			Base.BorderColor = borderColor;
			BorderSize = borderSize;
		}

		/* Background & Border color. */
		RectTheme Base;
	
		/* Border size in both direction (X and Y). */
		Dimension<int> BorderSize;
	};

	//----------------------------------------------------------------------------------

	/*!
	Contains basic information about the Image widget theme.
	*/
	struct ImageTheme
	{
		/* Default constructor. */
		ImageTheme(
			Color tint = Color(255, 255, 255, 255),
			Color borderColor = Color(0, 0, 0, 0),
			Dimension<int> borderSize = Dimension<int>(0, 0)
		){
			Base.Tint = tint;
			Base.BorderColor = borderColor;
			BorderSize = borderSize;
		}

		/* Image tint & Border color. */
		RectTheme Base;

		/* Border size in both direction (X and Y). */
		Dimension<int> BorderSize;
	};

	//----------------------------------------------------------------------------------

	/*!
	Contains basic information about the Text widget theme.
	*/
	struct TextTheme
	{
		/* Default constructor. */
		TextTheme(
			int size = 12,
			Color tint = Color(255,255,255, 255),
			Font* fnt = nullptr
		){
			iFontSize = size;
			Tint = tint;
			UsedFont = fnt;
		}

		/* Destructor - does not delete "UsedFont". Please cleanup by yourself. */
		~TextTheme()
		{
		}

		/* Describes font size. */
		int iFontSize;

		/* Describes font color. */
		Color Tint;

		/* Pointer to the loaded Font. */
		Font* UsedFont;
	};

	//----------------------------------------------------------------------------------

	/*!
	Contains basic information about the SimpleButton widget theme.
	*/
	struct SimpleButtonTheme
	{
		/* Default constructor. */
		SimpleButtonTheme(
			RectTheme normalColor = RectTheme(),
			RectTheme hoverColor = RectTheme(),
			RectTheme activeColor = RectTheme(),
			Dimension<int> borderSize = Dimension<int>(1,1),
			TextTheme textTheme = TextTheme()
		){
			Base[0] = normalColor;
			Base[1] = hoverColor;
			Base[2] = activeColor;
			BorderSize = borderSize;
			TextTh = textTheme;
		}

		/* Button tint & border color. [0 - Normal, 1 - Hover, 2 - Active] */
		RectTheme Base[3];
		
		/* Border size in both direction (X and Y). */
		Dimension<int> BorderSize;

		/* Button text theme. */
		TextTheme TextTh;
	};

	//----------------------------------------------------------------------------------

	/*!
	Contains basic information about the SimpleInput widget theme.
	*/
	struct SimpleInputTheme
	{

		SimpleInputTheme(
			RectTheme normalColor = RectTheme(),
			RectTheme hoverColor = RectTheme(),
			RectTheme activeColor = RectTheme(),
			Dimension<int> borderSize = Dimension<int>(1, 1),
			TextTheme defaultTextTh = TextTheme(),
			TextTheme textTh = TextTheme(),
			Color selection = Color(128, 128, 128, 255),
			Color prompt = Color(255, 255, 255, 255),
			Color bar = Color(255, 255, 255, 255),
			Point<int> margins = Point<int>(0, 0),
			int barHeight = 2
		){
			Base[0] = normalColor;
			Base[1] = hoverColor;
			Base[2] = activeColor;
			BorderSize = borderSize;
			DefaultText = defaultTextTh;
			Text = textTh;
			Selection = selection;
			Prompt = prompt;
			Bar = bar;
			Margins = margins;
			iBarHeight = barHeight;
		}

		/* Button color & Border color. [0 - Normal, 1 - Hover, 2 - Active] */
		RectTheme Base[3];

		/* Border size in both direction (X and Y). */
		Dimension<int> BorderSize;

		/* Default text theme. */
		TextTheme DefaultText;

		/* Text theme. */
		TextTheme Text;

		/* Color of the text selection. */
		Color Selection;

		/* Color of the input prompt. */
		Color Prompt;

		/* Color of the input bottom bar.*/
		Color Bar;

		/* Text margins within widget. */
		Point<int> Margins;

		/* Bar height. */
		int iBarHeight;

	};

	//----------------------------------------------------------------------------------

	/*!
	Contains basic information about the VScroller widget theme.
	*/
	struct VScrollerTheme
	{

		VScrollerTheme(
			RectTheme backgroundColor = RectTheme(),
			RectTheme normalColor = RectTheme(),
			RectTheme hoverColor = RectTheme(),
			RectTheme activeColor = RectTheme(),
			Dimension<int> scrollerBorderSize = Dimension<int>(1, 1),
			Dimension<int> sliderBorderSize = Dimension<int>(1, 1),
			int sliderWidth = 18
		) {
			Base = backgroundColor;
			Slider[0] = normalColor;
			Slider[1] = hoverColor;
			Slider[2] = activeColor;
			iWidth = sliderWidth;
			BaseBorderSize = scrollerBorderSize;
			SliderBorderSize = sliderBorderSize;
		}

		/* Button color & border color for Widget background. */
		RectTheme Base;

		/* Button color & border color for Slider. [0 - Normal, 1 - Hover, 2 - Active] */
		RectTheme Slider[3];

		/* Widget border size in both direction (X and Y). */
		Dimension<int> BaseBorderSize;

		/* Slider border size in both direction (X and Y). */
		Dimension<int> SliderBorderSize;

		/* Scroller width height. */
		int iWidth;

	};

	//-------------------------------------------------------------------------------------------------------

	/*!
	Contains the themes for all basic widgets of the library.
	*/
	struct Theme
	{
		/*!
		Assigns a font to all sub-themes. Font is not deleted inside Theme descturctor.
		*/
		void AssignFontToAll(Font* font)
		{
			ContentText.UsedFont = Header1.UsedFont = Header2.UsedFont = Header3.UsedFont = font;
			SimpleButtonBG.TextTh.UsedFont = SimpleButtonIMG.TextTh.UsedFont = font;
			SimpleInput.Text.UsedFont = SimpleInput.DefaultText.UsedFont = font;
		}

		BackgroundTheme Background;
		ImageTheme Image;
		TextTheme ContentText;
		TextTheme Header1;
		TextTheme Header2;
		TextTheme Header3;
		SimpleButtonTheme SimpleButtonBG;
		SimpleButtonTheme SimpleButtonIMG;
		SimpleInputTheme SimpleInput;
		VScrollerTheme VScroller;
	};

	/* Default ANGE widgets theme. Note: user have to load the fonts, in this case "Noto" font. */
	static const Theme DefTheme = {
		{ {255,255,255,255},{128,128,128,255}, {1,1} },
		{ {255,255,255,255},{0,0,0,0}, {0,0} },
		{ 12, {255,255,255,255} },
		{ 24, {255,255,255,255} },
		{ 19, {255,255,255,255} },
		{ 15, {255,255,255,255} },
		{
			{{0x7b7478}, {0x4d2d52}},
			{{0x655f63}, {0x39213c}},
			{{0x444042}, {0x2a192d}},
			{1,1},
			{12, {255,255,2555,255}}
		},
		{
			{{0x7b7478}, {0x4d2d52}},
			{{0xFFFFFF}, {0xFFFFFF}},
			{{0x444042}, {0x2a192d}},
			{0,0},
			{12, {255,255,2555,255}}
		},
		{
			{{0x7b7478}, {0x4d2d52}},
			{{0x655f63}, {0x39213c}},
			{{0x444042}, {0x2a192d}},
			{0,0},
			{12, {20,20,20,255}},
			{12, {255,255,255,255}},
			{128,128,128,255},
			{255,255,255,255},
			{20,20,20,255},
			{4,1},
			2
		},
		{
			{{255,255,255,255},{0x444042}},
			{{0x7b7478}, {0x4d2d52}},
			{{0x655f63}, {0x39213c}},
			{{0x444042}, {0x2a192d}},
			{1,1},
			{1,1},
			18
		}
	};

}