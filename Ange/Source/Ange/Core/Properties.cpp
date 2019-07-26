#include "Apch.h"
#include "Properties.h"
#include "Ange/Core/Window.h"
#include "Ange/Core/Widgets.h"

namespace Ange {

	//ResizableProps struct -------------------------------------------------
	ResizableProps::ResizableProps()
	{
		BasePosition = { 0, 0 };
		ParentBaseDimension = { 0, 0 };
		BaseDimension = { 0, 0 };
		iMinWidth = iMinHeight = iMaxWidth = iMaxHeight = -1;
		iXProp = iYProp = iWProp = iHProp = 0;
	}

	ResizableProps::ResizableProps(const ResizableProps& copy)
	{
		BasePosition = copy.BasePosition;
		ParentBaseDimension = copy.ParentBaseDimension;
		BaseDimension = copy.BaseDimension;
		iMinWidth = copy.iMinWidth;
		iMinHeight = copy.iMinHeight;
		iMaxWidth = copy.iMaxWidth;
		iMaxHeight = copy.iMaxHeight;
		iXProp = copy.iXProp;
		iYProp = copy.iYProp;
		iWProp = copy.iWProp;
		iHProp = copy.iHProp;
	}

	ResizableProps::~ResizableProps()
	{
	}

	//Resizable2D-------------------------------------------------

	Resizable2D::Resizable2D(Window* window, const Widget2DProps& props)
	{
		m_ResizableProps.BaseDimension = props.Dimensions;
		m_ResizableProps.BasePosition = props.Position;
		if (window != nullptr) {
			m_ResizableProps.ParentBaseDimension = window->GetDimension();
		}
	}

	Resizable2D::Resizable2D(const Resizable2D& copy)
	{
		m_ResizableProps = copy.m_ResizableProps;
	}
	 
	Resizable2D::~Resizable2D() noexcept
	{
	}

	const ResizableProps& Resizable2D::GetResizableProps() const
	{
		return m_ResizableProps;
	}


	void Resizable2D::CalcResizeData(Dimension<size_t>& fromResizeEvent, Dimension<size_t>& newDim, Point<int>& newPos)
	{
		//Calculate diff from base dimension
		float widthDiff = (float)fromResizeEvent.tWidth - (float)m_ResizableProps.ParentBaseDimension.tWidth;
		float heightDiff = (float)fromResizeEvent.tHeight - (float)m_ResizableProps.ParentBaseDimension.tHeight;

		//Calculate new dims and pos
		float widthToAdd = 0;
		float heightToAdd = 0;
		float xToAdd = 0;
		float yToAdd = 0;

		if (m_ResizableProps.iWProp) widthToAdd = widthDiff * (float)m_ResizableProps.iWProp / 100;
		if (m_ResizableProps.iHProp) heightToAdd = heightDiff * (float)m_ResizableProps.iHProp / 100;
		if (m_ResizableProps.iXProp) xToAdd = widthDiff * (float)m_ResizableProps.iXProp / 100;
		if (m_ResizableProps.iYProp) yToAdd = heightDiff * (float)m_ResizableProps.iYProp / 100;

		newDim.Set(
			m_ResizableProps.BaseDimension.tWidth + (size_t)widthToAdd,
			m_ResizableProps.BaseDimension.tHeight + (size_t)heightToAdd
		);
		newPos.Set(
			m_ResizableProps.BasePosition.tX + (int)xToAdd,
			m_ResizableProps.BasePosition.tY + (int)yToAdd
		);

		//Clamp
		if (m_ResizableProps.iMinWidth > -1 && (int)newDim.tWidth < m_ResizableProps.iMinWidth) newDim.tWidth = m_ResizableProps.iMinWidth;
		if (m_ResizableProps.iMaxWidth > -1 && (int)newDim.tWidth > m_ResizableProps.iMaxWidth) newDim.tWidth = m_ResizableProps.iMaxWidth;
		if (m_ResizableProps.iMinHeight > -1 && (int)newDim.tHeight < m_ResizableProps.iMinHeight) newDim.tHeight = m_ResizableProps.iMinHeight;
		if (m_ResizableProps.iMaxHeight > -1 && (int)newDim.tHeight > m_ResizableProps.iMaxHeight) newDim.tHeight = m_ResizableProps.iMaxHeight;
	}


	void Resizable2D::SetResizeProportions(int x, int y, int w, int h)
	{
		m_ResizableProps.iXProp = x;
		m_ResizableProps.iYProp = y;
		m_ResizableProps.iWProp = w;
		m_ResizableProps.iHProp = h;
	}


	void Resizable2D::SetMinMaxDimensions(int iMinWidth, int iMinHeight, int iMaxWidth, int iMaxHeight)
	{
		m_ResizableProps.iMinWidth = iMinWidth;
		m_ResizableProps.iMinHeight = iMinHeight;
		m_ResizableProps.iMaxWidth = iMaxWidth;
		m_ResizableProps.iMaxHeight = iMaxHeight;
	}

};