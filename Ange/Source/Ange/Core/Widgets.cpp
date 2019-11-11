#include "Apch.h"
#include "Ange/Core/Widgets.h"
#include "Ange/Core/Window.h"
#include "Ange/Core/Framebuffer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Ange {

	//Matrices struct -------------------------------------------------

	Matrices::Matrices()
	{
		m4Translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		m4Rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		m4Scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
		v3AnchorTranslation = glm::vec3(0.0f);
		m4Model = glm::mat4(1.0f);
		m4Mvp = glm::mat4(1.0f);
	}

	Matrices::Matrices(const Matrices& copy)
	{
		v3AnchorTranslation = copy.v3AnchorTranslation;
		m4Translation = copy.m4Translation;
		m4Rotation = copy.m4Rotation;
		m4Scale = copy.m4Scale;
		m4Model = copy.m4Model;
		m4Mvp = copy.m4Mvp;
	}

	Matrices::~Matrices()
	{
	}

	//WidgetDProps struct -------------------------------------------------

	Widget2DProps::Widget2DProps(Point<int> position, Dimension<size_t> dimension, int flags)
	{
		Position = position;
		Dimensions = dimension;
		iFlags = flags;
		bVisibility = true;
		bIfChanged = true;
		bDisabled = false;
		iId = -1;
	}

	Widget2DProps::Widget2DProps(const Widget2DProps& copy)
	{
		Position = copy.Position;
		Dimensions = copy.Dimensions;
		iFlags = copy.iFlags;
		bVisibility = copy.bVisibility;
		bIfChanged = copy.bIfChanged;
		bDisabled = copy.bDisabled;
		iId = -1;
	}

	Widget2DProps::~Widget2DProps()
	{
	}

	//Widget2D----------------------------------------------------
	Widget2D::Widget2D() :
		Resizable2D(nullptr, Widget2DProps())
	{
		ANGE_ERROR("You should not be there...");
		m_ParentWindow = nullptr;
		m_Widget2DProps = Widget2DProps();
		m_WidgetType = WidgetType::Base;
		exit(10);
	}

	Widget2D::Widget2D(Window* parent, const Widget2DProps& props) :
		Resizable2D(parent, props)
	{
		m_ParentWindow = parent;
		m_Widget2DProps = props;
		m_WidgetType = WidgetType::Base;
	}

	Widget2D::Widget2D(const Widget2D& copy) :
		Resizable2D(copy)
	{
		m_ParentWindow = copy.m_ParentWindow;
		m_WidgetType = copy.m_WidgetType;
		m_Widget2DProps = copy.m_Widget2DProps;
	}

	Widget2D::~Widget2D()
	{
		m_ParentWindow = nullptr;
	}

	void swap(Widget2D& first, Widget2D& second) noexcept
	{
		std::swap(first.m_Widget2DProps, second.m_Widget2DProps);
		std::swap(first.m_ParentWindow, second.m_ParentWindow);
		std::swap(first.m_WidgetType, second.m_WidgetType);
		std::swap(first.m_ResizableProps, second.m_ResizableProps);
	}

	Window* Widget2D::GetParentWindow() const
	{
		return m_ParentWindow;
	}


	const Dimension<size_t> Widget2D::GetDimension() const
	{
		return m_Widget2DProps.Dimensions;
	}

	const Point<int> Widget2D::GetPosition() const
	{
		return m_Widget2DProps.Position;
	}

	const Widget2DProps & Widget2D::GetWidget2DProps() const
	{
		return m_Widget2DProps;
	}

	void Widget2D::SetWidgetId(int id)
	{
		m_Widget2DProps.iId = id;
	}

	int Widget2D::GetWidgetId() const
	{
		return m_Widget2DProps.iId;
	}


	void Widget2D::SetFlags(int newFlags)
	{
		m_Widget2DProps.iFlags = newFlags;
		m_Widget2DProps.bIfChanged = true;
	}

	void Widget2D::UnregisterEvent(EventType eventType)
	{
		if (m_ParentWindow == nullptr) return;
		for (std::list<BindListIterator>::iterator it = m_Bindings.begin(); it != m_Bindings.end(); it++)
		{
			if ((*it)->first == eventType) {
				m_ParentWindow->UnbindEvent((*it));
				it = m_Bindings.erase(it);
			}
		}
	}

	void Widget2D::ChangeParent(Window* newParent)
	{
		if (!m_Bindings.empty()){
			DisableWidget();
			m_ParentWindow = newParent;
			EnableWidget();
		} else {
			m_ParentWindow = newParent;
		}
	}

	int Widget2D::GetFlags() const
	{
		return m_Widget2DProps.iFlags;
	}

	WidgetType Widget2D::GetWidgetType()
	{
		return m_WidgetType;
	}

	/*Framebuffer* Widget2D::RenderToTexture()
	{
		//Create framebuffer & render to texture
		Framebuffer* f = new Framebuffer(GetDimension());
		f->Bind();
		Render();
		f->Unbind();
		
		//Return new texture
		return f;
	}*/

	bool Widget2D::CheckCoords(Point<int> & point)
	{
		if (point.tX >= m_Widget2DProps.Position.tX &&
			point.tX <= m_Widget2DProps.Position.tX + (int)m_Widget2DProps.Dimensions.tWidth &&
			point.tY >= m_Widget2DProps.Position.tY &&
			point.tY <= m_Widget2DProps.Position.tY + (int)m_Widget2DProps.Dimensions.tHeight) {
			return true;
		}
		return false;
	}

	void Widget2D::AlterCoords(Point<int>& point)
	{
		point.tX -= m_Widget2DProps.Position.tX;
		point.tY -= m_Widget2DProps.Position.tY;
	}

	//BasicWidget2D ---------------------------------------------

	BasicWidget2D::BasicWidget2D(Window* parent, const Widget2DProps& props) : Widget2D(parent, props)
	{
		m_UsedShader = nullptr;
		if (parent == nullptr) {
			std::string message = "[BasicWidget2D Constructor] A 'nullptr' was passed instead of a valid pointer to the Windows object.";
			ANGE_FATAL(message.c_str());
			throw std::runtime_error(message);
		}
	}

	BasicWidget2D::BasicWidget2D(const BasicWidget2D& copy) : Widget2D(copy)
	{
		m_Matrices = copy.m_Matrices;
		m_UsedShader = copy.m_UsedShader;
		m_fRotationAngle = copy.m_fRotationAngle;
	}

	BasicWidget2D::~BasicWidget2D()
	{
		m_UsedShader = nullptr;
	}

	void swap(BasicWidget2D & first, BasicWidget2D & second) noexcept
	{
		using std::swap;
		swap(first.m_fRotationAngle, second.m_fRotationAngle);
		swap(first.m_Matrices, second.m_Matrices);
		swap(first.m_UsedShader, second.m_UsedShader);
		swap(static_cast<Widget2D&>(first), static_cast<Widget2D&>(second));
	}

	bool BasicWidget2D::OnWindowResize(Event* ev)
	{
		WindowResizeEvent* wre = (WindowResizeEvent*)ev;
		if (m_Widget2DProps.iFlags & ResizePolicy::AutoFill) {
			m_Widget2DProps.Position = Point<int>({ 0, 0 });
			m_Widget2DProps.Dimensions = wre->GetDimension();
		} else {
			CalcResizeData(wre->GetDimension(), m_Widget2DProps.Dimensions, m_Widget2DProps.Position);
			m_Matrices.m4Translation = glm::translate(
				glm::mat4(1.0f),
				glm::vec3(2 * m_Widget2DProps.Position.tX, 2 * m_Widget2DProps.Position.tY, 0.0f)
			);
		}
		CalculateAnchorVec();
		BindBuffers();
		m_Widget2DProps.bIfChanged = true;
		return false;
	}

	bool BasicWidget2D::OnDrawableInvokeRender(Event* ev)
	{
		Render();
		return false;
	}

	bool BasicWidget2D::OnWindowClose(Event* ev)
	{
		DisableWidget();
		return false;
	}

	void BasicWidget2D::EnableWidget()
	{
		if (m_ParentWindow->IfOpen() == false) {
			std::string message = "[BasicWidget2D EnableWidget] Parent window is closed.";
			ANGE_WARNING(message.c_str());
		} else {
			m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowResize, I_BIND(BasicWidget2D, OnWindowResize)));
			m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::DrawableInvokeRender, I_BIND(BasicWidget2D, OnDrawableInvokeRender)));
			m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowClose, I_BIND(BasicWidget2D, OnWindowClose)));
		}
	}

	void BasicWidget2D::DisableWidget()
	{
		if (m_ParentWindow == nullptr) return;
		for (auto it : m_Bindings)
		{
			m_ParentWindow->UnbindEvent(it);
		}
		m_Bindings.clear();
	}

	void BasicWidget2D::UseShader(const char* shaderName)
	{
		m_UsedShader = m_ParentWindow->GetShaderManager()->GetShaderData(shaderName);
	}

	void BasicWidget2D::SetPosition(Point<int> newPosition)
	{	
		m_ResizableProps.BasePosition = newPosition;
		m_Widget2DProps.Position.tX = newPosition.tX;
		m_Widget2DProps.Position.tY = newPosition.tY;
		m_Matrices.m4Translation = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(2 * newPosition.tX, 2 * newPosition.tY, 0.0f)
		);
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::ChangePosition(Point<int> positionChange)
	{
		m_ResizableProps.BasePosition += positionChange;
		m_Widget2DProps.Position.tX += positionChange.tX;
		m_Widget2DProps.Position.tY += positionChange.tY;
		m_Matrices.m4Translation = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(2 * m_Widget2DProps.Position.tX, 2 * m_Widget2DProps.Position.tY, 0.0f)
		);
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::Resize(Dimension<size_t> newSize)
	{
		m_ResizableProps.BaseDimension = newSize;
		m_Widget2DProps.Dimensions = newSize;
		CalculateAnchorVec();
		BindBuffers();
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::ChangeSize(Dimension<size_t> sizeChange)
	{
		m_ResizableProps.BaseDimension += sizeChange;
		m_Widget2DProps.Dimensions += sizeChange;
		CalculateAnchorVec();
		BindBuffers();
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::SetRotation(float newAngle)
	{
		m_fRotationAngle = newAngle;
		m_Matrices.m4Rotation = glm::rotate(
			glm::radians(newAngle),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::ChangeRotation(float angleChange)
	{
		m_fRotationAngle += angleChange;
		m_Matrices.m4Rotation *= glm::rotate(
			glm::radians(angleChange),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::SetScale(Scale<float> newScale)
	{
		m_Matrices.m4Scale = glm::scale(
			glm::vec3(newScale.tX, newScale.tY, 1.0f)
		);
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::ChangeScale(Scale<float> scaleChange)
	{
		m_Matrices.m4Scale += glm::scale(
			glm::vec3(scaleChange.tX, scaleChange.tY, 0.0f)
		);
		m_Widget2DProps.bIfChanged = true;
	}

	void BasicWidget2D::SetVisibility(bool mode)
	{
		m_Widget2DProps.bVisibility = mode;
	}

	bool BasicWidget2D::GetVisibility() const
	{
		return m_Widget2DProps.bVisibility;
	}

	void BasicWidget2D::SetFlags(int newFlags)
	{
		Widget2D::SetFlags(newFlags);
		CalculateAnchorVec();
	}

	void BasicWidget2D::CalculateAnchorVec()
	{
		m_Matrices.v3AnchorTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			m_Matrices.v3AnchorTranslation += glm::vec3((float)m_Widget2DProps.Dimensions.tWidth, 0.0f, 0.0f);
		}
		else if (m_Widget2DProps.iFlags & Anchor::Right) {
			m_Matrices.v3AnchorTranslation += glm::vec3(-(float)m_Widget2DProps.Dimensions.tWidth, 0.0f, 0.0f);
		}
		if (m_Widget2DProps.iFlags & Anchor::Bottom) {
			m_Matrices.v3AnchorTranslation += glm::vec3(0.0f, (float)m_Widget2DProps.Dimensions.tHeight, 0.0f);
		}
		else if (m_Widget2DProps.iFlags & Anchor::Top) {
			m_Matrices.v3AnchorTranslation += glm::vec3(0.0f, -(float)m_Widget2DProps.Dimensions.tHeight, 0.0f);
		}

	}

	void BasicWidget2D::RecalculateMatrices()
	{
		if (m_Widget2DProps.bIfChanged == true) {
			//Fix alignment
			auto realDim = m_ParentWindow->GetPhysicalWindowDim();
			int fixX = 0, fixY = 0;
			if (realDim.tWidth % 2 == 1) fixX += 1;
			if (realDim.tHeight % 2 == 1) fixY += 1;

			//if (realDim.tWidth % 2 == 1 || realDim.tHeight % 2 == 1) {
				m_Matrices.m4Translation = glm::translate(
					glm::mat4(1.0f),
					glm::vec3(2 * m_Widget2DProps.Position.tX + fixX, 2 * m_Widget2DProps.Position.tY + fixY, 0.0f)
				);
			//}
			//Mat calc.
			m_Matrices.m4Model = m_Matrices.m4Translation*m_Matrices.m4Rotation*m_Matrices.m4Scale;
			m_Widget2DProps.bIfChanged = false;
		}
		m_Matrices.m4Mvp = m_ParentWindow->GetWorld()->GetGuiMatrices().m4Projection*m_ParentWindow->GetWorld()->GetGuiMatrices().m4View*m_Matrices.m4Model;
	}

	//FrontWidget ---------------------------------------------

	FrontWidget::FrontWidget()
	{
		m_Type = WidgetType::Nullptr;
		m_Variant.m_Accessor = nullptr;
	}

	FrontWidget::FrontWidget(WidgetType baseWidgetType, Widget2D* realWidget)
	{
		SetWidget(baseWidgetType, realWidget);
	}

	FrontWidget::FrontWidget(const FrontWidget& copy)
	{
		m_Type = copy.m_Type;
		if (copy.m_Type == WidgetType::Background) {
			m_Variant.m_Background = copy.m_Variant.m_Background;
		} else if (copy.m_Type == WidgetType::Image){
			m_Variant.m_Image = copy.m_Variant.m_Image;
		}
	}


	FrontWidget& FrontWidget::operator=(FrontWidget rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	FrontWidget::~FrontWidget()
	{
		Cleanup();
	}

	void swap(FrontWidget& first, FrontWidget& second) noexcept
	{
		using std::swap;
		swap(first.m_Type, second.m_Type);
		swap(first.m_Variant.m_Accessor, second.m_Variant.m_Accessor);
	}

	void FrontWidget::Cleanup()
	{
		if (m_Variant.m_Accessor != nullptr) delete m_Variant.m_Accessor;
	}

	void FrontWidget::SetWidget(WidgetType baseWidgetType, Widget2D* realWidget)
	{
		Cleanup();
		m_Type = baseWidgetType;
		m_Variant.m_Accessor = realWidget;
	}

}
