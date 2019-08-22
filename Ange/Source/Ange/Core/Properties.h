#pragma once

#include "Ange/Core/Primitives.h"

namespace Ange {

	//------------------------------------------------------------------------------------------------------
	//Defines & Forward declarations
	//------------------------------------------------------------------------------------------------------

	#define A_DONT_CARE -1

	class Window;
	struct Widget2DProps;

	//------------------------------------------------------------------------------------------------------
	//Common Structs
	//------------------------------------------------------------------------------------------------------

	/*!
	A structure that stores data, which are used mainly when resizing a parent window.
	*/
	struct ResizableProps
	{
		/*!
		Default constructor.
		*/
		ResizableProps();

		/*!
		Copy constructor.
		*/
		ResizableProps(const ResizableProps& copy);

		/*!
		Destructor.
		*/
		~ResizableProps();

		/*Stores information about the original location of the widget.*/
		Point<int> BasePosition;

		/*Stores information about the size of the parent when creating the widget.*/
		Dimension<size_t> ParentBaseDimension;

		/*Stores information about the original dimension of the widget.*/
		Dimension<size_t> BaseDimension;

		/*Stores information about the maximum or minimum allowed widget size. The value
		"A_DONT_CARE" means that the dimension is not constrained.*/
		int iMinWidth, iMinHeight, iMaxWidth, iMaxHeight;

		/*Stores the proportions in which a given parameter changes its value in relation to the
		size change of the parent.*/
		int iXProp, iYProp, iWProp, iHProp;
	};

	//------------------------------------------------------------------------------------------------------
	//Base Classes
	//------------------------------------------------------------------------------------------------------

	/*!
	A class that is a kind of set of tools for automatically changing the size and position of the widget.
	*/
	class Resizable2D
	{
	public:

		/*!
		Default constructor.
		*/
		Resizable2D(Window* window, const Widget2DProps& props);

		/*!
		Copy constructor.
		*/
		Resizable2D(const Resizable2D& copy);

		/*!
		Virtual destructor.
		*/
		virtual ~Resizable2D() noexcept;
		
		/*!
		A function that allows to read data which determines the operation of the automatic resizing and
		positioning mechanism of the widget.
		*/
		const ResizableProps& GetResizableProps() const;

		/*!
		Sets the aspect ratio that changes the position and size of the widget in relation to the size and
		position change of the parent.
		*/
		virtual void SetResizeProportions(int x, int y, int w, int h);

		/*!
		Sets the maximum and minimum dimensions of the widget. Pass "A_DONT_CARE" if you do not want to
		set one of the values.
		*/
		virtual void SetMinMaxDimensions(int iMinWidth, int iMinHeight, int iMaxWidth, int iMaxHeight);

	protected:

		/*!
		Calculates the widget position and dimension change.
		*/
		virtual void CalcResizeData(Dimension<size_t>& fromResizeEvent, Dimension<size_t>& newDim, Point<int>& newPos);

		/* It stores all the data needed to automatically change the position and size of the widget. */
		ResizableProps m_ResizableProps;
	};

	/*!
	Adds the ability to move the widget. Important - manually connect the widget to the window by
	registering the MouseClick and MouseMove events to the OnDraggable() function. Make the changes
	in EnableWidget() and DisableWidget() functions.
	*/

	//class Draggable {
	//public:
	//	void SetDraggableMode(bool mode);
	//private:
	//	bool m_Draggable;
	//	Point<int> m_DragStartPos;
	//	Point<int> m_WidgetStartPos;
	//};

};
