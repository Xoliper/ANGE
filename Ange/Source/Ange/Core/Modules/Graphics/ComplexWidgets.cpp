#include "Apch.h"
#include "ComplexWidgets.h"
#include "Ange/Core/Modules/Graphics/Widgets.h"
#include "Ange/Core/Modules/Window.h"
#ifdef ANGE_PLATFORM_WINDOWS
#include <comdef.h>
#endif
#include <clocale>
#include <codecvt>
#include <algorithm>


namespace Ange {

	SimpleButton::SimpleButton(Window* window, const Widget2DProps& props, const BackgroundProps& rectProps, const TextProps& textProps)
		: Widget2D(window, props),
		m_BgColors{ rectProps.BaseColor, rectProps.BaseColor, rectProps.BaseColor },
		m_BorderColors{ rectProps.BorderColor, rectProps.BorderColor, rectProps.BorderColor },
		m_Callback(nullptr),
		m_iTicks(0),
		m_AnchorOffsets{ 0,0 },
		m_bDrag(false),
		m_bBypassEventsReturn(false)
	{
		auto textCenterProps = props;
		textCenterProps.Position = Point<int>({ (int)(props.Position.tX + props.Dimensions.tWidth / 2), (int)(props.Position.tY + props.Dimensions.tHeight / 2) });
		textCenterProps.iFlags = Anchor::HorizontalCenter | Anchor::VerticalCenter;

		m_State = WidgetMouseState::Normal;
		m_Text = new Text(window, textCenterProps, textProps);
		m_FrontWidget.SetWidget(WidgetType::Background, new Background(window, props, rectProps));

		EnableWidget();
	}

	SimpleButton::SimpleButton(Window* window, const Widget2DProps& props, const ImageProps& spriteProps, const TextProps& textProps)
		: Widget2D(window, props),
		m_BgColors{ {255,255,255,255}, {255,255,255,255}, {255,255,255,255} },
		m_BorderColors{ spriteProps.BorderColor, spriteProps.BorderColor, spriteProps.BorderColor },
		m_Callback(nullptr),
		m_iTicks(0),
		m_AnchorOffsets{ 0,0 },
		m_bDrag(false),
		m_bBypassEventsReturn(false)
	{
		auto textCenterProps = props;
		textCenterProps.Position = Point<int>({ (int)(props.Position.tX + props.Dimensions.tWidth / 2), (int)(props.Position.tY + props.Dimensions.tHeight / 2) });
		textCenterProps.iFlags = Anchor::HorizontalCenter | Anchor::VerticalCenter;

		m_State = WidgetMouseState::Normal;
		m_FrontWidget.SetWidget(WidgetType::Image, new Image(window, props, spriteProps));
		m_Text = new Text(window, textCenterProps, textProps);

		EnableWidget();
	}

	SimpleButton::~SimpleButton()
	{
		DisableWidget();
		delete m_Text;
	}

	//Setters
	void SimpleButton::SetColor(WidgetMouseState forState, Color& color)
	{
		m_BgColors[forState] = color;
		if (forState == m_State) {
			if (m_FrontWidget.m_Type == WidgetType::Background) m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[forState]);
			if (m_FrontWidget.m_Type == WidgetType::Image) m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[forState]);
		}
	}

	void SimpleButton::SetColor(Color& normal, Color& hover, Color& press)
	{
		m_BgColors[WidgetMouseState::Normal] = normal;
		m_BgColors[WidgetMouseState::Hover] = hover;
		m_BgColors[WidgetMouseState::Press] = press;
		if (m_FrontWidget.m_Type == WidgetType::Background) m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[m_State]);
		if (m_FrontWidget.m_Type == WidgetType::Image) m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[m_State]);
	}

	void SimpleButton::SetBorderColor(WidgetMouseState forState, Color& color)
	{
		m_BorderColors[forState] = color;
		if (forState == m_State) {
			if (m_FrontWidget.m_Type == WidgetType::Background) m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[forState]);
			if (m_FrontWidget.m_Type == WidgetType::Image) m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[forState]);

		}
	}

	void SimpleButton::SetBorderColor(Color& normal, Color& hover, Color& press)
	{
		m_BorderColors[WidgetMouseState::Normal] = normal;
		m_BorderColors[WidgetMouseState::Hover] = hover;
		m_BorderColors[WidgetMouseState::Press] = press;
		if (m_FrontWidget.m_Type == WidgetType::Background) m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[m_State]);
		if (m_FrontWidget.m_Type == WidgetType::Image) m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[m_State]);
	}

	void SimpleButton::SetBoderSize(Dimension<int> newBorderSize)
	{
		if (m_FrontWidget.m_Type == WidgetType::Background) m_FrontWidget.m_Variant.m_Background->SetBoderSize(newBorderSize);
		if (m_FrontWidget.m_Type == WidgetType::Image) m_FrontWidget.m_Variant.m_Image->SetBoderSize(newBorderSize);
	}

	void SimpleButton::SetFontSize(int newSize)
	{
		m_Text->SetFontSize(newSize);
	}

	void SimpleButton::SetText(std::wstring newText)
	{
		m_Text->SetText(newText);
	}

	void SimpleButton::SetFont(Font* newFont)
	{
		m_Text->SetFont(newFont);
	}

	void SimpleButton::SetFontColor(Color& newColor)
	{
		m_Text->SetColor(newColor);
	}

	void SimpleButton::SetFlags(int newFlags)
	{
		Widget2D::SetFlags(newFlags);
		if(m_FrontWidget.m_Variant.m_Accessor != nullptr) m_FrontWidget.m_Variant.m_Accessor->SetFlags(newFlags);
		m_Widget2DProps.iFlags = newFlags;
		CalcAnchorOffsets();
	}

	void SimpleButton::SetBypassEventReturn(bool mode)
	{
		m_bBypassEventsReturn = mode;
	}

	void SimpleButton::UnregisterEvent(EventType eventType)
	{
		m_Text->UnregisterEvent(eventType);
		if (m_FrontWidget.m_Variant.m_Accessor != nullptr) m_FrontWidget.m_Variant.m_Accessor->UnregisterEvent(eventType);
	}


	void SimpleButton::Resize(Dimension<size_t> newDim)
	{
		if (m_FrontWidget.m_Type == WidgetType::Background) m_FrontWidget.m_Variant.m_Background->Resize(newDim);
		if (m_FrontWidget.m_Type == WidgetType::Image) m_FrontWidget.m_Variant.m_Image->Resize(newDim);
		m_Text->Resize(newDim);
		m_Widget2DProps.Dimensions = newDim;
		m_ResizableProps.BaseDimension = newDim;
		CalcAnchorOffsets();
	}


	//Getters
	const Color SimpleButton::GetColor(WidgetMouseState forState) const
	{
		return m_BgColors[forState];
	}

	const Color SimpleButton::GetBorderColor(WidgetMouseState forState) const
	{
		return m_BorderColors[forState];
	}

	const Dimension<int> SimpleButton::GetBoderSize() const
	{
		if (m_FrontWidget.m_Type == WidgetType::Background) m_FrontWidget.m_Variant.m_Background->GetBoderSize();
		if (m_FrontWidget.m_Type == WidgetType::Image) m_FrontWidget.m_Variant.m_Image->GetBoderSize();
		return { 0, 0 };
	}

	const int SimpleButton::GetFontSize() const
	{
		return m_Text->GetFontSize();
	}

	std::wstring SimpleButton::GetText() const
	{
		return m_Text->GetText();
	}

	Color SimpleButton::GetFontColor() const
	{
		return m_Text->GetColor();
	}

	Font* SimpleButton::GetUsedFont() const
	{
		return m_Text->GetFont();
	}

	const Image* SimpleButton::GetImage() const
	{
		if (m_FrontWidget.m_Type == WidgetType::Image) return m_FrontWidget.m_Variant.m_Image;
		return nullptr;
	}
	
	const Background* SimpleButton::GetBackground() const
	{
		if (m_FrontWidget.m_Type == WidgetType::Background) return m_FrontWidget.m_Variant.m_Background;
		return nullptr;
	}

	void SimpleButton::SetResizeProportions(int x, int y, int w, int h)
	{
		if(m_FrontWidget.m_Variant.m_Accessor != nullptr) m_FrontWidget.m_Variant.m_Accessor->SetResizeProportions(x, y, w, h);
		m_Text->SetResizeProportions(0, 0, w, h);
		Resizable2D::SetResizeProportions(x, y, w, h);
	}
		
	void SimpleButton::SetPosition(Point<int> newPosition)
	{
		//Assuming no nullptr in FrontWidget...

		m_Widget2DProps.Position = newPosition;
		m_ResizableProps.BasePosition = newPosition;

		auto currentPosition = m_FrontWidget.m_Variant.m_Accessor->GetPosition();
		Point<int> diff = { newPosition.tX - currentPosition.tX - 1, newPosition.tY - currentPosition.tY  - 1 };
		m_FrontWidget.m_Variant.m_Accessor->SetPosition(newPosition);
		diff.tX += 1;
		diff.tY += 1;
		m_Text->ChangePosition(diff);
	}

	void SimpleButton::ChangePosition(Point<int> positionChange)
	{
		m_FrontWidget.m_Variant.m_Accessor->ChangePosition(positionChange);
		m_Text->ChangePosition(positionChange);

		m_Widget2DProps.Position += positionChange;
		m_ResizableProps.BasePosition += positionChange;
	}

	void SimpleButton::SetVisibility(bool mode)
	{
		m_FrontWidget.m_Variant.m_Accessor->SetVisibility(mode);
		m_Text->SetVisibility(mode);
		m_Widget2DProps.bVisibility = mode;
	}

	bool SimpleButton::GetVisibility() const
	{
		return m_Widget2DProps.bVisibility;
	}

	void SimpleButton::SetCallback(std::function<bool(Event*)> function)
	{
		m_Callback = function;
	}

	void SimpleButton::ResetCallback()
	{
		m_Callback = nullptr;
	}

	WidgetMouseState SimpleButton::GetState()
	{
		/*if (m_State == WidgetMouseState::Press) {
			m_State = WidgetMouseState::Hover;
			return WidgetMouseState::Press;
		}*/
		return m_State;
	}

	bool SimpleButton::GetDragInfo()
	{
		return m_bDrag;
	}


	//Derived
	void SimpleButton::EnableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		ANGE_ZERO_TEST(m_ParentWindow->IfOpen(), "m_ParentWindow == 0! (window is closed)");

		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseMove, I_BIND(SimpleButton, OnMouseMove)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseEnter, I_BIND(SimpleButton, OnMouseEnter)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseClick, I_BIND(SimpleButton, OnMouseClick)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowResize, I_BIND(SimpleButton, OnWindowResize)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowClose, I_BIND(SimpleButton, OnWindowClose)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::Tick, I_BIND(SimpleButton, OnWindowTick)));

		m_FrontWidget.m_Variant.m_Accessor->EnableWidget();
		m_Text->EnableWidget();
	}

	void SimpleButton::DisableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		if (m_ParentWindow == nullptr) return;
		for (auto it : m_Bindings)
		{
			m_ParentWindow->UnbindEvent(it);
		}
		m_Bindings.clear();

		m_FrontWidget.m_Variant.m_Accessor->DisableWidget();
		m_Text->DisableWidget();
	}

	void SimpleButton::Render()
	{
		m_FrontWidget.m_Variant.m_Accessor->Render();
		m_Text->Render();
	}

	//Helper funciton
	void SimpleButton::CalcAnchorOffsets()
	{
		int xFix = 0;
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			m_AnchorOffsets.tX = 0;
			xFix = (int)m_Widget2DProps.Dimensions.tWidth / 2;
		} else if (m_Widget2DProps.iFlags & Anchor::Right) {
			m_AnchorOffsets.tX = -(int)m_Widget2DProps.Dimensions.tWidth;
		} else if (m_Widget2DProps.iFlags & Anchor::HorizontalCenter) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth/2);// 0;
			xFix = (int)m_Widget2DProps.Dimensions.tWidth / 4;
		}

		int yFix = 0;
		if (m_Widget2DProps.iFlags & Anchor::Bottom) {
			m_AnchorOffsets.tY = 0;
			yFix = (int)m_Widget2DProps.Dimensions.tHeight/2;
		} else if (m_Widget2DProps.iFlags & Anchor::Top) {
			m_AnchorOffsets.tY = -(int)m_Widget2DProps.Dimensions.tHeight;
		} else if (m_Widget2DProps.iFlags & Anchor::VerticalCenter) {
			m_AnchorOffsets.tY = -(int)(m_Widget2DProps.Dimensions.tHeight/2);  //0
			yFix = (int)m_Widget2DProps.Dimensions.tHeight / 4;
		}	


		m_Text->SetPosition(Point<int>({
			(int)(m_Widget2DProps.Position.tX + m_AnchorOffsets.tX / 2 + xFix),
			(int)(m_Widget2DProps.Position.tY + m_AnchorOffsets.tY / 2 + yFix)
		}));

	}

	//Events
	bool SimpleButton::OnWindowTick(Event* ev)
	{
		++m_iTicks;
		if (m_State == WidgetMouseState::Press && m_iTicks > 10 && GetVisibility() == true && m_bDrag == false) {

			if (m_FrontWidget.m_Type == WidgetType::Background) {
				m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[WidgetMouseState::Hover]);
				m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Hover]);
			} else if (m_FrontWidget.m_Type == WidgetType::Image) {
				m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[WidgetMouseState::Hover]);
				m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[WidgetMouseState::Hover]);
			}
			
			m_State = WidgetMouseState::Hover;
			m_iTicks = 0;
		}
		return false;
	}

	bool SimpleButton::OnWindowClose(Event* ev)
	{
		DisableWidget();
		return false;
	}

	bool SimpleButton::OnWindowResize(Event* ev)
	{
		WindowResizeEvent* wre = (WindowResizeEvent*)ev;
		if (m_Widget2DProps.iFlags & ResizePolicy::AutoFill) {
			m_Widget2DProps.Position = Point<int>({ 0, 0 });
			m_Widget2DProps.Dimensions = wre->GetDimension();
		} else {
			CalcResizeData(wre->GetDimension(), m_Widget2DProps.Dimensions, m_Widget2DProps.Position);
		}
		CalcAnchorOffsets();
		m_Widget2DProps.bIfChanged = true;
		return false;
	}

	bool SimpleButton::OnMouseMove(Event* ev)
	{

		if (GetVisibility() == false) return false;
		//if (m_bDrag == true) return false;
		MouseMoveEvent* mme = (MouseMoveEvent*)ev;
		auto pos = mme->GetPosition() - m_AnchorOffsets;
		if (bool inside = CheckCoords(pos); inside || m_bDrag) {
			mme->SetCheckStatus(inside);
			//Update graphics & state
			if (m_State != WidgetMouseState::Hover){

				if (m_FrontWidget.m_Type == WidgetType::Background) {
					m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[WidgetMouseState::Hover]);
					m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Hover]);
				} else if (m_FrontWidget.m_Type == WidgetType::Image) {
					m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[WidgetMouseState::Hover]);
					m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[WidgetMouseState::Hover]);
				}
			}

			m_State = WidgetMouseState::Hover;
			if (m_Callback != nullptr) m_Callback(mme);
			if (m_bBypassEventsReturn) return false;
			return false;
		}
		//Update graphics & state
		if (m_State != WidgetMouseState::Normal) {

			if (m_FrontWidget.m_Type == WidgetType::Background) {
				m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[WidgetMouseState::Normal]);
				m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
			} else if (m_FrontWidget.m_Type == WidgetType::Image) {
				m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[WidgetMouseState::Normal]);
				m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
			}
		}
		m_State = WidgetMouseState::Normal;
		return false;
	}

	bool SimpleButton::OnMouseClick(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseClickEvent* mce = (MouseClickEvent*)ev;
		if (auto pos = mce->GetPosition() - m_AnchorOffsets; CheckCoords(pos)) {
			if (mce->GetAction() == 1) {
				//Update graphics & state
				if (m_State != WidgetMouseState::Press) {

					if (m_FrontWidget.m_Type == WidgetType::Background) {
						m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[WidgetMouseState::Press]);
						m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Press]);
					} else if (m_FrontWidget.m_Type == WidgetType::Image) {
						m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[WidgetMouseState::Press]);
						m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[WidgetMouseState::Press]);
					}
				}
				m_iTicks = 0;
				m_State = WidgetMouseState::Press;
				m_bDrag = true;
			} else {
				m_bDrag = false;
			}
			if (m_Callback != nullptr) m_Callback(mce);
			if (m_bBypassEventsReturn) return false;
			return true;
		} else if (m_bDrag == true){
			m_bDrag = false;
		}
		//Update graphics & state
		if (m_State != WidgetMouseState::Normal) {

			if (m_FrontWidget.m_Type == WidgetType::Background) {
				m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[WidgetMouseState::Normal]);
				m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
			} else if (m_FrontWidget.m_Type == WidgetType::Image) {
				m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[WidgetMouseState::Normal]);
				m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
			}
		}
		m_State = WidgetMouseState::Normal;
		return false;
	}

	bool SimpleButton::OnMouseEnter(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseEnterEvent* mee = (MouseEnterEvent*)ev;
		if (mee->GetState() == 0){
			//Update graphics & state
			if (m_State != WidgetMouseState::Normal) {
				if (m_FrontWidget.m_Type == WidgetType::Background) {
					m_FrontWidget.m_Variant.m_Background->SetColor(m_BgColors[WidgetMouseState::Normal]);
					m_FrontWidget.m_Variant.m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
				} else if (m_FrontWidget.m_Type == WidgetType::Image) {
					m_FrontWidget.m_Variant.m_Image->SetColor(m_BgColors[WidgetMouseState::Normal]);
					m_FrontWidget.m_Variant.m_Image->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
				}
			}
			m_State = WidgetMouseState::Normal;
		}
		return false;
	}

	//--------------------------------------------------------------------------------

	SimpleInput::SimpleInput(Window* window, const Widget2DProps& props, const BackgroundProps& rectProps, const TextProps& defaultTextProps, const TextProps& textProps):
		Widget2D(window, props),
		m_BgColors{ rectProps.BaseColor, rectProps.BaseColor, rectProps.BaseColor },
		m_BorderColors{ rectProps.BorderColor, rectProps.BorderColor, rectProps.BorderColor },
		m_Callback(nullptr),
		m_iTicks(0),
		m_bDrag(false),
		m_iDragStart(-1),
		m_iDragEnd(-1),
		m_Margins{10,4},
		m_FilterFunc(nullptr)
	{
		//Calculate some vars
		Widget2DProps textFixedProps = props;
		textFixedProps.Position += m_Margins;
		textFixedProps.iFlags = Anchor::Left | Anchor::Bottom;
		textFixedProps.Dimensions -= {(size_t)m_Margins.tX*2, (size_t)m_Margins.tY*2};

		Color selectColor = textProps.TextColor;
		if (selectColor.GetBrightness() >= 128){
			selectColor -= Color(50, 50, 50, 0);
		} else {
			selectColor += Color(50, 50, 50, 0);
		}
		selectColor.SetColor(50, 50, 50, 255);
		selectColor.Clamp();

		//Fill everything up
		m_State = WidgetMouseState::Normal;
		m_Background = new Background(window, props, rectProps);
		m_DefaultText = new Text(window, textFixedProps, defaultTextProps);
		m_Text = new Text(window, textFixedProps, textProps);

		m_Selection = new Rectangle2D(window, {{0,0}, {0,0} }, { selectColor,{0,0,0,0}, {0,0} });
		m_Selection->SetVisibility(false);
		m_Prompt = new Rectangle2D(
			window,
			//{ "Prompt", textFixedProps.Position, {2, props.Dimensions.tHeight-2*m_Margins.tY}, Anchor::Left | Anchor::Bottom},
			{{textFixedProps.Position.tX + m_Margins.tX, textFixedProps.Position.tY + m_Margins.tY}, {2, (size_t)m_Text->GetFont()->GetLineHeight(m_Text->GetFontSize())}, Anchor::Left | Anchor::Bottom },
			{textProps.TextColor, {0,0,0,0}, {0,0}}
		);

		if (!defaultTextProps.wsText.empty() && textProps.wsText.empty()) {
			m_Text->SetVisibility(false);
			m_DefaultText->SetVisibility(true);
		} else {
			m_Text->SetVisibility(true);
			m_DefaultText->SetVisibility(false);
		}
		m_Prompt->SetVisibility(false);

		EnableWidget();
	}

	SimpleInput::~SimpleInput()
	{
		DisableWidget();
		delete m_Background;
		delete m_Text;
		delete m_Prompt;
		delete m_Selection;
		delete m_DefaultText;
	}

	//Setters
	void SimpleInput::SetColor(WidgetMouseState forState, Color& color)
	{
		m_BgColors[forState] = color;
		if (forState == m_State) m_Background->SetColor(m_BgColors[forState]);
	}

	void SimpleInput::SetColor(Color& normal, Color& hover, Color& press)
	{
		m_BgColors[WidgetMouseState::Normal] = normal;
		m_BgColors[WidgetMouseState::Hover] = hover;
		m_BgColors[WidgetMouseState::Press] = press;
		m_Background->SetColor(m_BgColors[m_State]);
	}

	void SimpleInput::SetBorderColor(WidgetMouseState forState, Color& color)
	{
		m_BorderColors[forState] = color;
		if (forState == m_State) m_Background->SetBorderColor(m_BorderColors[forState]);
	}

	void SimpleInput::SetBorderColor(Color& normal, Color& hover, Color& press)
	{
		m_BorderColors[WidgetMouseState::Normal] = normal;
		m_BorderColors[WidgetMouseState::Hover] = hover;
		m_BorderColors[WidgetMouseState::Press] = press;
		m_Background->SetBorderColor(m_BorderColors[m_State]);
	}

	void SimpleInput::SetBoderSize(Dimension<int> newBorderSize)
	{
		m_Background->SetBoderSize(newBorderSize);
	}

	void SimpleInput::SetFontSize(int newSize)
	{
		m_Text->SetFontSize(newSize);
	}

	void SimpleInput::SetText(std::wstring newText)
	{
		m_Text->SetText(newText);
	}

	void SimpleInput::SetFont(Font* newFont)
	{
		m_Text->SetFont(newFont);
	}

	void SimpleInput::SetFontColor(Color& newColor)
	{
		m_Text->SetColor(newColor);
	}

	void SimpleInput::SetFlags(int newFlags)
	{
		Widget2D::SetFlags(newFlags);
		m_Background->SetFlags(newFlags);
		m_Widget2DProps.iFlags = newFlags;
		CalcAnchorOffsets();
	}

	void SimpleInput::UnregisterEvent(EventType eventType)
	{
		m_Text->UnregisterEvent(eventType);
		m_Background->UnregisterEvent(eventType);
		m_Prompt->UnregisterEvent(eventType);
		m_Selection->UnregisterEvent(eventType);
		m_DefaultText->UnregisterEvent(eventType);
	}

	//Getters
	const Color SimpleInput::GetColor(WidgetMouseState forState) const
	{
		return m_BgColors[forState];
	}

	const Color SimpleInput::GetBorderColor(WidgetMouseState forState) const
	{
		return m_BorderColors[forState];
	}

	const Dimension<int>& SimpleInput::GetBoderSize() const
	{
		return m_Background->GetBoderSize();
	}

	const int SimpleInput::GetFontSize() const
	{
		return m_Text->GetFontSize();
	}

	std::wstring SimpleInput::GetText() const
	{
		return m_Text->GetText();
	}

	std::wstring SimpleInput::GetTexWhenEnter()
	{
		if (m_Enter == true) {
			m_Enter = false;
			return m_Text->GetText();
		}
		return L"";
	}


	Color SimpleInput::GetFontColor() const
	{
		return m_Text->GetColor();
	}

	Font* SimpleInput::GetUsedFont() const
	{
		return m_Text->GetFont();
	}

	void SimpleInput::SetResizeProportions(int x, int y, int w, int h)
	{
		int xFix = 0;
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			xFix = x;
		} else if (m_Widget2DProps.iFlags & Anchor::Right) {
			xFix = -(w-x);
		}

		m_Prompt->SetResizeProportions(xFix, y, 0, h);
		m_Selection->SetResizeProportions(xFix, y, 0, h);

		m_Background->SetResizeProportions(x, y, w, h);
		m_Text->SetResizeProportions(0, 0, w, h);
		m_DefaultText->SetResizeProportions(0, 0, w, h);
		Resizable2D::SetResizeProportions(x, y, w, h);
	}


	//TODO: Test
	void SimpleInput::SetPosition(Point<int> newPosition)
	{
		m_Widget2DProps.Position = newPosition;
		m_ResizableProps.BasePosition = newPosition;

		Point<int> diff = {newPosition.tX - m_Background->GetPosition().tX -1, newPosition.tY - m_Background->GetPosition().tY - 1 };
		m_Background->SetPosition(newPosition);
		
		diff.tX += 1;
		diff.tY += 1;
		
		m_Text->ChangePosition(diff);
		m_DefaultText->ChangePosition(diff);
		m_Prompt->ChangePosition(diff);
		m_Selection->ChangePosition(diff);
	}

	void SimpleInput::ChangePosition(Point<int> positionChange)
	{
		m_Background->ChangePosition(positionChange);
		m_Widget2DProps.Position += positionChange;
		m_ResizableProps.BasePosition += positionChange;

		m_Text->ChangePosition(positionChange);
		m_DefaultText->ChangePosition(positionChange);
		m_Prompt->ChangePosition(positionChange);
		m_Selection->ChangePosition(positionChange);
	}

	void SimpleInput::SetVisibility(bool mode)
	{
		if (mode == false) {
			m_Text->SetVisibility(mode);
			m_DefaultText->SetVisibility(mode);
		} else {
			if (!m_DefaultText->GetText().empty() && m_Text->GetText().empty()) {
				m_Text->SetVisibility(false);
				m_DefaultText->SetVisibility(true);
			} else {
				m_Text->SetVisibility(true);
				m_DefaultText->SetVisibility(false);
			}
		}
		m_Background->SetVisibility(mode);
		m_Prompt->SetVisibility(false);
		m_Selection->SetVisibility(mode);
		m_Widget2DProps.bVisibility = mode;

	}

	bool SimpleInput::GetVisibility() const
	{
		return m_Widget2DProps.bVisibility;
	}

	void SimpleInput::SetDefaultTextColor(Color& newColor)
	{
		m_DefaultText->SetColor(newColor);
	}

	void SimpleInput::SetDefaultText(std::wstring newDefaultText)
	{
		m_DefaultText->SetText(newDefaultText);
	}

	int SimpleInput::GetPromptPos() const
	{
		return m_iPromptIdx;
	}


	Color SimpleInput::GetDefaultTextColor() const 
	{
		return m_DefaultText->GetColor();
	}

	std::wstring SimpleInput::GetDefaultText() const
	{
		return m_DefaultText->GetText();
	}

	const std::wstring& SimpleInput::GetTextRef() const
	{
		return m_Text->GetTextRef();
	}


	void SimpleInput::SetCallback(std::function<bool(Event*)> function)
	{
		m_Callback = function;
	}

	void SimpleInput::ResetCallback()
	{
		m_Callback = nullptr;
	}

	void SimpleInput::SetFilter(std::function<bool(KbCharAppearEvent*)> function)
	{
		m_FilterFunc = function;
	}

	void SimpleInput::ResetFilter()
	{
		m_FilterFunc = nullptr;
	}

	WidgetMouseState SimpleInput::GetState()
	{
		/*if (m_State == WidgetMouseState::Press) {
			m_State = WidgetMouseState::Hover;
			return WidgetMouseState::Press;
		}*/
		return m_State;
	}

	void SimpleInput::SetTextMargin(Point<int> newMargins)
	{
		m_Margins = newMargins;
	}

	const Point<int>& SimpleInput::GetTextMargin() const
	{
		return m_Margins;
	}

	//Derived
	void SimpleInput::EnableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		ANGE_ZERO_TEST(m_ParentWindow->IfOpen(), "m_ParentWindow == 0! (window is closed)");

		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseMove, I_BIND(SimpleInput, OnMouseMove)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseEnter, I_BIND(SimpleInput, OnMouseEnter)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseClick, I_BIND(SimpleInput, OnMouseClick)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowResize, I_BIND(SimpleInput, OnWindowResize)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowClose, I_BIND(SimpleInput, OnWindowClose)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::Tick, I_BIND(SimpleInput, OnWindowTick)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::KbCharAppear, I_BIND(SimpleInput, OnNewChar)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::KbKeyAppear, I_BIND(SimpleInput, OnNewKey)));


		m_Background->EnableWidget();
		m_Selection->EnableWidget();
		m_Text->EnableWidget();
		m_DefaultText->EnableWidget();
		m_Prompt->EnableWidget();
	}

	void SimpleInput::DisableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		if (m_ParentWindow == nullptr) return;
		for (auto it : m_Bindings)
		{
			m_ParentWindow->UnbindEvent(it);
		}
		m_Bindings.clear();

		m_Background->DisableWidget();
		m_DefaultText->DisableWidget();
		m_Text->DisableWidget();
		m_Prompt->DisableWidget();
		m_Selection->DisableWidget();
	}

	void SimpleInput::Render()
	{
		m_Background->Render();
		m_Selection->Render();
		m_Text->Render();
		m_DefaultText->Render();
		m_Prompt->Render();
	}

	void SimpleInput::CalcAnchorOffsets()
	{
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			m_AnchorOffsets.tX = 0;
		} else if (m_Widget2DProps.iFlags & Anchor::Right) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth);
		} else if (m_Widget2DProps.iFlags & Anchor::HorizontalCenter) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth / 2);// 0;
		}

		if (m_Widget2DProps.iFlags & Anchor::Bottom) {
			m_AnchorOffsets.tY = 0;//m_Widget2DProps.Dimensions.tHeight; 
		}
		else if (m_Widget2DProps.iFlags & Anchor::Top) {
			m_AnchorOffsets.tY = -((int)m_Widget2DProps.Dimensions.tHeight);
		}
		else if (m_Widget2DProps.iFlags & Anchor::VerticalCenter) {
			m_AnchorOffsets.tY = -(int)(m_Widget2DProps.Dimensions.tHeight) / 2;  //0
		}

		m_Text->SetPosition(Point<int>({
			(int)(m_Widget2DProps.Position.tX + m_Margins.tX + m_AnchorOffsets.tX),
			(int)(m_Widget2DProps.Position.tY + m_Margins.tY + m_AnchorOffsets.tY)
		}));

		m_DefaultText->SetPosition(Point<int>({
			(int)(m_Widget2DProps.Position.tX + m_Margins.tX + m_AnchorOffsets.tX),
			(int)(m_Widget2DProps.Position.tY + m_Margins.tY + m_AnchorOffsets.tY)
		}));
	}

	//Events
	bool SimpleInput::OnWindowTick(Event* ev)
	{
		++m_iTicks;
		if (m_State == WidgetMouseState::Press && m_iTicks % 120 == 0 && GetVisibility() == true){
			m_Prompt->SetVisibility(!m_Prompt->GetVisibility());
		}

		if (m_iTicks > 600) {
			m_iTicks = 0;
		}
		return false;
	}

	//TODO: writing only when active
	bool SimpleInput::OnNewChar(Event* ev)
	{
		if (GetVisibility() == false) return false;
		if (m_State == WidgetMouseState::Press) {
			//Update text
			KbCharAppearEvent* kie = (KbCharAppearEvent*)ev;

			if (m_FilterFunc != nullptr) {
				bool passState = m_FilterFunc(kie);
				if (passState == false) return true;
			}

			unsigned int c = kie->GetCodePoint();
			std::wstring textTemp = m_Text->GetText();

			if (m_iDragStart == m_iDragEnd || std::min(m_iDragStart, m_iDragEnd) == -1) {
				textTemp.insert((size_t)m_iPromptIdx, 1, c);
				m_Text->SetText(textTemp);
				SetPromptPos(m_iPromptIdx + 1);
			} else {
				//Calc fix
				float fixPos = m_Text->GetDisplayOffset().tX + m_Text->GetPromptPosition(std::max(m_iDragStart, m_iDragEnd)) - m_Text->GetPromptPosition(std::min(m_iDragStart, m_iDragEnd));
				if (fixPos > 0.0f) fixPos = 0.0f;

				//Erase
				int charStart = std::min(m_iDragStart, m_iDragEnd);
				int charLength = std::max(m_iDragEnd, m_iDragStart) - std::min(m_iDragEnd, m_iDragStart);
				textTemp.erase(charStart, charLength);
				//Insert
				textTemp.insert(charStart, 1, c);
				m_Text->SetText(textTemp);

				m_iPromptIdx = charStart + 1;
				m_Text->SetDisplayOffset({ fixPos, m_Text->GetDisplayOffset().tY });
				SetPromptPos(m_iPromptIdx);

				//Disable m_Selection
				m_Selection->SetVisibility(false);
				m_iDragStart = m_iDragEnd = -1;

			}
			/*//Update prompt
			m_Text->GetNextPosition(m_iPromptIdx, m_fPromptPos);
			m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)m_fPromptPos, m_Widget2DProps.Position.tY+2 });
			m_Prompt->SetVisibility(true);

			//Update text position

			if (m_fPromptPos > (int)m_Text->GetDimension().tWidth) {
				float change = (m_fPromptPos - (int)m_Text->GetDimension().tWidth);
				m_Text->SetDisplayOffset({ -change, 0 });
				m_Prompt->SetPosition({ m_Widget2DProps.Position.tX+(int)m_fPromptPos-(int)change, m_Widget2DProps.Position.tY + 2 });
			}*/
			return true;
		}
		return false;
	}


#ifdef ANGE_PLATFORM_WINDOWS
	std::string utf8_encode(const std::wstring &wstr)
	{
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	std::wstring utf8_decode(const std::string &str)
	{
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}
#elif ANGE_PLATFORM_LINUX
	std::string utf8_encode(const std::wstring& wstr){ return "";}
	std::wstring utf8_decode(const std::string& str){ return L"";}
#endif


	bool SimpleInput::OnNewKey(Event* ev)
	{
		if (GetVisibility() == false) return false;
		if (m_State == WidgetMouseState::Press) {

			KbKeyAppearEvent* kie = (KbKeyAppearEvent*)ev;
			int k = kie->GetKey();
			if (k == 67 && kie->GetState() > 0 && kie->GetMods() == 2){ //Ctrl+C
				int start = std::min(m_iDragStart, m_iDragEnd);
				int	length = std::max(m_iDragEnd, m_iDragStart) - std::min(m_iDragEnd, m_iDragStart);
				if (length <= 0) return true;
				std::wstring tempWStr = m_Text->GetText().substr(start, length);
				std::string temp = utf8_encode(tempWStr);
				glfwSetClipboardString(m_ParentWindow->GetTopWindow()->GetGLFWwindow(), temp.c_str());
			} else if (k == 86 && kie->GetState() > 0 && kie->GetMods() == 2) {	//Ctrl+V
				auto data = glfwGetClipboardString(m_ParentWindow->GetTopWindow()->GetGLFWwindow());
				if (data == nullptr) return true;
				std::string toPaste(data);
				std::wstring temp = utf8_decode(toPaste);

				if (m_iDragStart == m_iDragEnd){				//Just paste					
					std::wstring curText = m_Text->GetText();
					curText.insert(m_iPromptIdx, temp);
					m_Text->SetText(curText);
					m_iPromptIdx += (int)temp.size();
					SetPromptPos(m_iPromptIdx);
					/*float fPos = 0.0f;
					float fix = 0.0f;
					float fOffset = m_Text->GetDisplayOffset().tX;
					if (fOffset > 0.0f) fOffset = 0.0f;
					fPos = m_Text->GetPromptPosition(m_iPromptIdx);
					if (fPos + fOffset > m_Widget2DProps.Dimensions.tWidth){
						fix = (fPos + fOffset) - m_Widget2DProps.Dimensions.tWidth;
						m_Text->SetDisplayOffset({ fOffset-fix,0 });
					}
					m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)fPos + (int)fOffset - (int)fix, m_Widget2DProps.Position.tY + 2 });
					m_Prompt->SetVisibility(true);
					m_fPromptPos = fPos;
					*/

				} else {				//Pase with erase
					std::wstring curText = m_Text->GetText();

					//Erase
					int charStart = std::min(m_iDragStart, m_iDragEnd);
					int charLength = std::max(m_iDragEnd, m_iDragStart) - std::min(m_iDragEnd, m_iDragStart);
					curText.erase(charStart, charLength);
					//Insert
					curText.insert(charStart, temp);
					m_Text->SetText(curText);
					//Update
					m_iPromptIdx = charStart+(int)temp.size();
					
					//m_iPromptIdx += charStart + temp.size();// temp.size();
					//Update graphics

					/*float fPos = 0.0f;
					float fix = 0.0f;
					float fOffset = m_Text->GetDisplayOffset().tX;
					if (fOffset > 0.0f) fOffset = 0.0f;
					fPos = m_Text->GetPromptPosition(m_iPromptIdx);
					if (fPos + fOffset > m_Widget2DProps.Dimensions.tWidth) {
						fix = (fPos + fOffset) - m_Widget2DProps.Dimensions.tWidth;
						m_Text->SetDisplayOffset({ fOffset - fix,0 });
					}
					m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)fPos + (int)fOffset - (int)fix - (int)diff, m_Widget2DProps.Position.tY + 2 });
					m_Prompt->SetVisibility(true);
					m_fPromptPos = fPos;
					*/

					SetPromptPos(m_iPromptIdx);

					//Disable m_Selection
					m_Selection->SetVisibility(false);
					m_iDragStart = m_iDragEnd = -1;

				}

			} else if (k == 259 && kie->GetState() > 0) { //Backspace

				if (std::wstring textTemp = m_Text->GetText(); m_iPromptIdx <= (int)textTemp.size() && m_iPromptIdx >= 0){
					//No selection
					if (m_iDragStart == m_iDragEnd){
						if (m_iPromptIdx == 0) return true;
						textTemp.erase(m_iPromptIdx-1, 1);
						m_Text->SetText(textTemp);
						SetPromptPos(m_iPromptIdx-1);
					} else {
						//Selection
						int toRemove = std::max(m_iDragStart, m_iDragEnd) - std::min(m_iDragStart, m_iDragEnd);
						int newPosIdx = std::min(m_iDragStart, m_iDragEnd);
						if (newPosIdx < 0) newPosIdx = 0;
						float fixPos = m_Text->GetDisplayOffset().tX + m_Text->GetPromptPosition(std::max(m_iDragStart, m_iDragEnd)) - m_Text->GetPromptPosition(std::min(m_iDragStart, m_iDragEnd));
						if (fixPos > 0.0f) fixPos = 0.0f;
						textTemp.erase(newPosIdx, toRemove);
						m_Text->SetText(textTemp);
						m_Text->SetDisplayOffset({ fixPos, m_Text->GetDisplayOffset().tY });
						SetPromptPos(newPosIdx);
						m_Selection->SetVisibility(false);
						m_iDragStart = m_iDragEnd = -1;
					}
				}

														  
				/*
				if (std::wstring textTemp = m_Text->GetText(); m_iPromptIdx <= textTemp.size()) {


					if (m_iDragStart == m_iDragEnd && m_iPromptIdx > 0) {
						//Update prompt & offsets
						float backupPos = m_fPromptPos;
						m_Text->GetPrevPosition(m_iPromptIdx, m_fPromptPos);

						if (m_Text->GetDisplayOffset().tX < 0.0f) {// > m_Widget2DProps.Dimensions.tWidth) {
							float fix = m_Text->GetDisplayOffset().tX + backupPos - m_fPromptPos;
							if (fix > 0.0f) fix = 0.0f;
							m_Text->SetDisplayOffset({ fix, 0 });
						} else {
							m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)m_fPromptPos, m_Widget2DProps.Position.tY + 2 });
						}
						m_Prompt->SetVisibility(true);

						//Update text
						textTemp.erase(m_iPromptIdx, 1);
						m_Text->SetText(textTemp);
					}
					else if (m_iDragStart != -1 && m_iDragEnd != -1 && m_iDragStart != m_iDragEnd) {
						//Selection case
						int toRemove = std::max(m_iDragStart, m_iDragEnd) - std::min(m_iDragStart, m_iDragEnd);
						int newPosIdx = std::min(m_iDragStart, m_iDragEnd);
						if (newPosIdx < 0) newPosIdx = 0;

						float selectStartPos = m_Text->GetPromptPosition(std::min(m_iDragStart, m_iDragEnd));
						float selectEndPos = m_Text->GetPromptPosition(std::max(m_iDragStart, m_iDragEnd));

						textTemp.erase(newPosIdx, toRemove);
						m_Text->SetText(textTemp);


						float fPos = m_Text->GetPromptPosition(newPosIdx);
						if (selectEndPos - selectStartPos > m_Widget2DProps.Dimensions.tWidth) {
							float fix = 0.0f;
							
							if (m_iDragStart < m_iDragEnd) {
								fix = (float)m_Text->GetDisplayOffset().tX + (selectEndPos - selectStartPos);
								if (fix > 0.0f) fix = 0.0f;
								m_Text->SetDisplayOffset({ fix, 0 });
							} else {
								fix = (float)m_Text->GetDisplayOffset().tX + ((selectEndPos - selectStartPos) - m_Widget2DProps.Dimensions.tWidth);
								if (fix > 0.0f) fix = 0.0f;
								if (-fix > m_Text->GetPromptPosition(textTemp.size()) - m_Widget2DProps.Dimensions.tWidth) fix = -(m_Text->GetPromptPosition(textTemp.size()) - m_Widget2DProps.Dimensions.tWidth);
								m_Text->SetDisplayOffset({ fix, 0 });
							}

						}

						m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)m_Text->GetDisplayOffset().tX + (int)fPos, m_Widget2DProps.Position.tY + 2 });
						m_Selection->SetVisibility(false);
						
						m_iDragStart = m_iDragEnd = -1;
						m_fPromptPos = fPos;
						m_iPromptIdx = newPosIdx;
					}

				}

				*/

			}
			else if (k == 263 && kie->GetState() > 0) { //Left
			 //Update prompt & offsets
				//float tooMuchLeftTest = m_fPromptPos + m_Text->GetDisplayOffset().tX;
				//float backupPos = m_fPromptPos;
				//m_Text->GetPrevPosition(m_iPromptIdx, m_fPromptPos);

				//if ((backupPos - m_fPromptPos) > tooMuchLeftTest){
				//	float fix = backupPos - m_fPromptPos - tooMuchLeftTest;
				//	m_Text->SetDisplayOffset({ m_Text->GetDisplayOffset().tX + fix, 0 });
				//}
				//m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)m_fPromptPos + (int)m_Text->GetDisplayOffset().tX, m_Widget2DProps.Position.tY + 2 });
				//m_Prompt->SetVisibility(true);
				if (m_iPromptIdx - 1 >= 0) {
					SetPromptPos(m_iPromptIdx - 1);
				}

				m_Selection->SetVisibility(false);
				m_iDragEnd = m_iDragStart = -1;
			}
			else if (k == 262 && kie->GetState() > 0) { //Right
			 //Update prompt & offsets
			/*	float backupPos = m_fPromptPos;
				m_Text->GetNextPosition(m_iPromptIdx, m_fPromptPos);

				if (m_fPromptPos + m_Text->GetDisplayOffset().tX > m_Widget2DProps.Dimensions.tWidth) {
					float fix = (m_fPromptPos - m_Widget2DProps.Dimensions.tWidth);
					m_Text->SetDisplayOffset({ -fix, 0 });
				}
				m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)m_fPromptPos + (int)m_Text->GetDisplayOffset().tX, m_Widget2DProps.Position.tY + 2 });
				m_Prompt->SetVisibility(true);*/

				if (m_iPromptIdx + 1 <= (int)m_Text->GetText().size()) {
					SetPromptPos(m_iPromptIdx + 1);
				}

				m_Selection->SetVisibility(false);
				m_iDragEnd = m_iDragStart = -1;
			}

			//Enter state update
			if (k == 257 && kie->GetState() > 0) {
				m_Enter = true;
			} else {
				m_Enter = false;
			}

		}
		return true;
	}

	bool SimpleInput::OnWindowClose(Event* ev)
	{
		DisableWidget();
		return false;
	}

	bool SimpleInput::OnWindowResize(Event* ev)
	{
		WindowResizeEvent* wre = (WindowResizeEvent*)ev;
		if (m_Widget2DProps.iFlags & ResizePolicy::AutoFill) {
			m_Widget2DProps.Position = Point<int>({ 0, 0 });
			m_Widget2DProps.Dimensions = wre->GetDimension();
		}
		else {
			CalcResizeData(wre->GetDimension(), m_Widget2DProps.Dimensions, m_Widget2DProps.Position);
		}
		CalcAnchorOffsets();
		m_Widget2DProps.bIfChanged = true;
		return false;
	}

	bool SimpleInput::OnMouseMove(Event* ev)
	{
		if (GetVisibility() == false) return false;
		bool bReturn = false;
		MouseMoveEvent* mme = (MouseMoveEvent*)ev;
		auto pos = mme->GetPosition() - m_AnchorOffsets;
		if (CheckCoords(pos)) {
			//Update graphics & state
			if (m_State == WidgetMouseState::Normal) {
				m_Background->SetColor(m_BgColors[WidgetMouseState::Hover]);
				m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Hover]);
				m_State = WidgetMouseState::Hover;
			}
			bReturn = true;
		} else {
			if (m_State == WidgetMouseState::Hover) {
				m_Background->SetColor(m_BgColors[WidgetMouseState::Normal]);
				m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
				m_State = WidgetMouseState::Normal;
			}
		}

		if (m_bDrag == true) {
			//Animation time
			if (m_iTicks % 360 == 0) {
				return bReturn;
			}
			//Safety Check
			int textSize = (int)m_Text->GetText().size();
			if (textSize == 0) return bReturn;

			//Setup vars
			float fPos = 0.0f;
			float fOffset = m_Text->GetDisplayOffset().tX;
			if (fOffset > 0.0f) fOffset = 0.0f;
			int tempIdx = m_iPromptIdx;

			if (pos.tX >= m_Widget2DProps.Position.tX + (int)m_Widget2DProps.Dimensions.tWidth) {
				++tempIdx;
				if (tempIdx+1 > textSize) tempIdx = textSize;				
				//Do the magic
				//float newOffset = m_Text->GetPromptPosition(tempIdx);
				//m_iPromptIdx = tempIdx;
				//m_fPromptPos = newOffset;



				SetPromptPos(tempIdx);
				m_iDragEnd = m_iPromptIdx;

				//float fix = (m_fPromptPos - m_Widget2DProps.Dimensions.tWidth);
				//if (m_Text->GetPromptPosition(m_Text->GetText().size()) < m_Widget2DProps.Dimensions.tWidth) fix = 0.0f;
				//m_Text->SetDisplayOffset({ -fix, 0 });
				//m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)m_fPromptPos + (int)m_Text->GetDisplayOffset().tX, m_Widget2DProps.Position.tY + 2 });
				//m_Prompt->SetVisibility(true);
			} else if (pos.tX <= m_Widget2DProps.Position.tX) {
				--tempIdx;
				if (tempIdx < 0) tempIdx = 0;

				//Do the magic
				//float newOffset = m_Text->GetPromptPosition(tempIdx);
				//m_iPromptIdx = tempIdx;
				//m_fPromptPos = newOffset;



				SetPromptPos(tempIdx);
				m_iDragEnd = m_iPromptIdx;

				//float fix = m_fPromptPos;
				//m_Text->SetDisplayOffset({ -fix, 0 });
				//m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)m_fPromptPos + (int)m_Text->GetDisplayOffset().tX, m_Widget2DProps.Position.tY + 2 });
				//m_Prompt->SetVisibility(true);
			} else {
				std::tie(m_iPromptIdx, m_fPromptPos) = m_Text->GetPromptPosition({ pos.tX + -1 * (int)fOffset  - m_Widget2DProps.Position.tX - m_Margins.tX, pos.tY + m_Margins.tY });
				SetPromptPos(m_iPromptIdx);
				//m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)fPos + (int)fOffset, m_Widget2DProps.Position.tY + 2 });
				//m_Prompt->SetVisibility(true);
				//m_fPromptPos = fPos;
				m_iDragEnd = m_iPromptIdx;
			}
			
			//Update selection graphics
			float selectStartPos = m_Text->GetPromptPosition(std::min(m_iDragStart, m_iDragEnd));
			float selectEndPos = m_Text->GetPromptPosition(std::max(m_iDragStart, m_iDragEnd));
			
			int newXPos = 0;
			size_t newWidth = 0;
			if (selectStartPos >= -m_Text->GetDisplayOffset().tX && selectEndPos <= -m_Text->GetDisplayOffset().tX + m_Widget2DProps.Dimensions.tWidth) {
				newXPos = (int)(m_Widget2DProps.Position.tX + (selectStartPos + m_Text->GetDisplayOffset().tX) + m_Margins.tX + m_AnchorOffsets.tX);
				newWidth = (size_t)(selectEndPos - selectStartPos);
			} else if (selectStartPos <= -m_Text->GetDisplayOffset().tX) {
				newXPos = m_Widget2DProps.Position.tX+m_Margins.tX + m_AnchorOffsets.tX;
				newWidth = (size_t)(selectEndPos + m_Text->GetDisplayOffset().tX);
			}
			if (newWidth > m_Widget2DProps.Dimensions.tWidth - m_Margins.tX * 2) newWidth = m_Widget2DProps.Dimensions.tWidth - m_Margins.tX * 2;

			if (newXPos == 0 && newWidth == 0) {
				newXPos = m_Prompt->GetPosition().tX;// -m_Widget2DProps.Position.tX;
				int diff = newXPos - m_Widget2DProps.Position.tX - m_Margins.tX;
				newWidth = (m_Widget2DProps.Dimensions.tWidth-m_Margins.tX*2) - diff + m_AnchorOffsets.tX;
			}

			m_Selection->SetPosition({ newXPos, m_Widget2DProps.Position.tY + m_Margins.tY + m_AnchorOffsets.tY });
			m_Selection->Resize({ newWidth, (size_t)m_Text->GetFont()->GetLineHeight(m_Text->GetFontSize())});
			m_Selection->SetVisibility(true);
			
		}

		return bReturn;
	}

	void SimpleInput::SetPromptPos(int pos)
	{
		//Create aliases
		int absPromptPos = (int)m_Text->GetPromptPosition(pos);
		int absOffset = -(int)(m_Text->GetDisplayOffset().tX);
		int width = (int)m_Text->GetWidget2DProps().Dimensions.tWidth;
		int widgetPos = m_Widget2DProps.Position.tX;
		if (absOffset < 0) absOffset = 0;

		//Position inside widget
		if (absPromptPos>=absOffset && absPromptPos <= absOffset+width){
			m_Prompt->SetPosition({absPromptPos-absOffset+m_Widget2DProps.Position.tX + m_Margins.tX + m_AnchorOffsets.tX, m_Widget2DProps.Position.tY + m_Margins.tY + m_AnchorOffsets.tY});
		} else if(absPromptPos < absOffset) {
			//Position outside widget (to the left)
			m_Prompt->SetPosition({m_Widget2DProps.Position.tX + m_Margins.tX + m_AnchorOffsets.tX, m_Widget2DProps.Position.tY + m_Margins.tY + m_AnchorOffsets.tY });
			m_Text->SetDisplayOffset({ -(float)absPromptPos, 0});
		} else if (absPromptPos > absOffset + width ) {
			//Position outside widget (to the right)
			m_Prompt->SetPosition({ (int)m_Widget2DProps.Position.tX + (int)m_Widget2DProps.Dimensions.tWidth - m_Margins.tX + m_AnchorOffsets.tX, m_Widget2DProps.Position.tY + m_Margins.tY + m_AnchorOffsets.tY });
			m_Text->SetDisplayOffset({ -(float)absPromptPos+width, (float)0 });
		}

		//Fix default text
		if (GetVisibility() == true) {
			if (!m_DefaultText->GetText().empty() && m_Text->GetText().empty()) {
				m_Text->SetVisibility(false);
				m_DefaultText->SetVisibility(true);
			}
			else {
				m_Text->SetVisibility(true);
				m_DefaultText->SetVisibility(false);
			}
		}

		m_iPromptIdx = pos;
		m_Prompt->SetVisibility(true);
	}


	bool SimpleInput::OnMouseClick(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseClickEvent* mce = (MouseClickEvent*)ev;
		if (auto pos = mce->GetPosition() - m_AnchorOffsets; CheckCoords(pos)) {
			if (mce->GetAction() == 1) {
				//Click [Press]
				//Update prompt, Text click relative position: ClickPos-Margin

				float fOffset = m_Text->GetDisplayOffset().tX;
				if (fOffset > 0.0f) fOffset = 0.0f;
				std::tie(m_iPromptIdx, m_fPromptPos) = m_Text->GetPromptPosition({ pos.tX + -1*(int)fOffset - m_Widget2DProps.Position.tX - m_Margins.tX, pos.tY + m_Margins.tY });
				SetPromptPos(m_iPromptIdx); //remove that if rollback
				//m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + (int)fPos + (int)fOffset, m_Widget2DProps.Position.tY + 2 });
				//m_Prompt->SetVisibility(true);
				//m_fPromptPos = fPos;

				//Update main graphics & state
				if (m_State != WidgetMouseState::Press) {
					m_Background->SetColor(m_BgColors[WidgetMouseState::Press]);
					m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Press]);
				}

				//Disable previous selection
				if (m_Selection->GetVisibility()) {
					m_iDragEnd = -1;
					m_iDragStart = -1;
					m_Selection->SetVisibility(false);
				}

				m_iTicks = 0;
				m_bActive = true;
				m_State = WidgetMouseState::Press;
				m_bDrag = true;
				m_iDragStart = m_iPromptIdx;
			} else {
				//Click [Release]
				m_bDrag = false;
				m_iDragEnd = m_iPromptIdx;
				//ANGE_INFO("Released");
				if (m_Callback != nullptr) m_Callback(mce);
			}
			return true;
		}

		//Check if dragged
		if (m_bDrag == true) {
			m_bDrag = false;
			return true;
		}

		//Update graphics & state
		if (m_State != WidgetMouseState::Normal && m_bDrag == false) {
			m_Background->SetColor(m_BgColors[WidgetMouseState::Normal]);
			m_Background->SetBorderColor(m_BorderColors[WidgetMouseState::Normal]);
			m_State = WidgetMouseState::Normal;
			m_bActive = false;
			m_Prompt->SetVisibility(false);
			m_Selection->SetVisibility(false);
		}

		return false;
	}

	bool SimpleInput::OnMouseEnter(Event* ev)
	{
		return false;
	}


	bool IntNumericFilter(KbCharAppearEvent* ev)
	{
		if (iswdigit(ev->GetCodePoint())) return true;
		return false;
	}

	bool FloatNumericFilter(KbCharAppearEvent* ev)
	{
		if (iswdigit(ev->GetCodePoint()) || ev->GetCodePoint() == '.' || ev->GetCodePoint() == ',') return true;
		return false;
	}

	bool AlphabeticFilter(KbCharAppearEvent* ev)
	{
		if (iswalpha(ev->GetCodePoint())) return true;
		return false;
	}

	bool AlphaNumericFilter(KbCharAppearEvent* ev)
	{
		if (iswalnum(ev->GetCodePoint())) return true;
		return false;
	}




	//------------------------------------------------------------------------

	VerticalScroller::VerticalScroller(
		Window* window,
		const Widget2DProps& props,
		const Point<int>& scrollerAreaPos,
		const Dimension<size_t>& scrollerAreaDim,
		const BackgroundProps& rectFgProps,
		const BackgroundProps& rectBgProps
	):	
		Widget2D(window,props),
		m_BgColor(rectBgProps.BaseColor),
		m_FgColors{ rectFgProps.BaseColor, rectFgProps.BaseColor, rectFgProps.BaseColor },
		m_Callback(nullptr),
		m_AnchorOffsets{ 0,0 },
		m_iContentHeight(0),
		m_ScrAreaPos(scrollerAreaPos),
		m_ScrAreaDim(scrollerAreaDim),
		m_SmartXPlace(0),
		m_Rows(0)
	{
		m_Background = new Background(window, props, rectBgProps);
		m_Button = new SimpleButton(window, { props.Position, {props.Dimensions.tWidth, 0}, Anchor::Left | Anchor::Bottom }, rectFgProps, { nullptr, 0, L"" });
		m_Button->SetBypassEventReturn(true);
		m_iBtnSave = props.Position;
		m_DragData[2] = { 0,0 };
		m_iDisplayLine = (int)m_Widget2DProps.Dimensions.tHeight;

		CalcAnchorOffsets();
		EnableWidget();
	}

	VerticalScroller::~VerticalScroller()
	{
		DisableWidget();
		delete m_Background;
		delete m_Button;
	}

	void VerticalScroller::SetCallback(std::function<bool(Event*)> function)
	{
		m_Callback = function;
	}

	void VerticalScroller::ResetCallback()
	{
		m_Callback = nullptr;
	}

	void  VerticalScroller::SetFlags(int newFlags)
	{
		Widget2D::SetFlags(newFlags);
		m_Background->SetFlags(newFlags);
		CalcAnchorOffsets();
		UpdateScrollerDim();
	}

	void VerticalScroller::CalcAnchorOffsets()
	{
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			m_AnchorOffsets.tX = 0;
		} else if (m_Widget2DProps.iFlags & Anchor::Right) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth);
		} else if (m_Widget2DProps.iFlags & Anchor::HorizontalCenter) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth / 2);
		}

		if (m_Widget2DProps.iFlags & Anchor::Bottom) {
			m_AnchorOffsets.tY = 0;
		} else if (m_Widget2DProps.iFlags & Anchor::Top) {
			m_AnchorOffsets.tY = -((int)m_Widget2DProps.Dimensions.tHeight);
		} else if (m_Widget2DProps.iFlags & Anchor::VerticalCenter) {
			m_AnchorOffsets.tY = -(int)(m_Widget2DProps.Dimensions.tHeight) / 2;
		}
	}

	void VerticalScroller::SetResizeProportions(int x, int y, int w, int h)
	{
		m_Background->SetResizeProportions	(x, y, 0, h);
		m_Button->SetResizeProportions		(x, y, 0, h);
		Resizable2D::SetResizeProportions	(x, y, 0, h);
	}

	void VerticalScroller::SetPosition(Point<int> newPosition)
	{
		m_Widget2DProps.Position = newPosition;
		m_ResizableProps.BasePosition = newPosition;

		Point<int> diff = { newPosition.tX - m_Background->GetPosition().tX - 1, newPosition.tY - m_Background->GetPosition().tY - 1 };
		m_Background->SetPosition(newPosition);
		m_iBtnSave = newPosition;
		diff.tX += 1;
		diff.tY += 1;
		m_Button->ChangePosition(diff);
	}

	void VerticalScroller::ChangePosition(Point<int> positionChange)
	{
		m_Widget2DProps.Position += positionChange;
		m_ResizableProps.BasePosition += positionChange;
		m_iBtnSave += positionChange;
		m_Background->ChangePosition(positionChange);
		m_Button->ChangePosition(positionChange);
	}

	void VerticalScroller::SetVisibility(bool mode)
	{
		m_Background->SetVisibility(mode);
		m_Button->SetVisibility(mode);
		m_Widget2DProps.bVisibility = mode;
		for (auto widget : m_ConnectedWidgets){
			widget.first->SetVisibility(mode);
		}
	}

	bool VerticalScroller::GetVisibility() const
	{
		return m_Widget2DProps.bVisibility;
	}


	void VerticalScroller::EnableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		ANGE_ZERO_TEST(m_ParentWindow->IfOpen(), "m_ParentWindow == 0! (window is closed)");

		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseMove, I_BIND(VerticalScroller, OnMouseMove)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseEnter, I_BIND(VerticalScroller, OnMouseEnter)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseClick, I_BIND(VerticalScroller, OnMouseClick)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowResize, I_BIND(VerticalScroller, OnWindowResize)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowClose, I_BIND(VerticalScroller, OnWindowClose)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::DrawableInvokeRender, I_BIND(VerticalScroller, OnDrawableInvokeRender)));

		m_Background->EnableWidget();
		m_Button->EnableWidget();

		m_Button->UnregisterEvent(EventType::DrawableInvokeRender);
		m_Background->UnregisterEvent(EventType::DrawableInvokeRender);
	}

	void VerticalScroller::DisableWidget()
	{

		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		if (m_ParentWindow == nullptr) return;
		for (auto it : m_Bindings)
		{
			m_ParentWindow->UnbindEvent(it);
		}
		m_Bindings.clear();

		m_Background->DisableWidget();
		m_Button->DisableWidget();
	}


	void VerticalScroller::Render()
	{
		m_Background->Render();
		m_Button->Render();
	}
	

	void  VerticalScroller::SetForegroundColor(WidgetMouseState forState, Color& color)
	{
		m_FgColors[forState] = color;
		m_Button->SetColor(forState, m_FgColors[forState]);
	}
	
	void  VerticalScroller::SetForegroundColors(Color& normal, Color& hover, Color& press)
	{
		m_FgColors[WidgetMouseState::Normal] = normal;
		m_FgColors[WidgetMouseState::Hover] = hover;
		m_FgColors[WidgetMouseState::Press] = press;
		m_Button->SetColor(normal, hover, press);
	}
	
	void  VerticalScroller::SetBackgroundColor(Color& color)
	{
		m_BgColor = color;
		m_Background->SetColor(color);
	}

	const Color VerticalScroller::GetForegroundColor(WidgetMouseState forState) const
	{
		return m_FgColors[forState];
	}
	
	const Color VerticalScroller::GetBackgroundColor() const
	{
		return m_BgColor;
	}

	//Update scroller btn 
	void VerticalScroller::UpdateScrollerDim() {
		if(m_iContentHeight > (int)m_ScrAreaDim.tHeight) {
			size_t oldHeight = m_Button->GetDimension().tHeight;
			float contentRatio = (float)(m_iContentHeight) / m_Widget2DProps.Dimensions.tHeight;// (int)m_ScrAreaDim.tHeight;
			size_t newHeight = (size_t)((float)1 / contentRatio * m_Widget2DProps.Dimensions.tHeight);// m_ScrAreaDim.tHeight;
			m_Button->Resize({ (size_t)m_Button->GetDimension().tWidth, newHeight });
		} else {
			m_Button->Resize({ (size_t)m_Button->GetDimension().tWidth, 0 });
		}
	}

	void VerticalScroller::SetOffset(float offset) {
	
		if (offset > 1.0f) offset = 1.0f;
		if (offset < 0.0f) offset = 0.0f;
		
		//calculate real offset, & update elements
		float baseYPosition = m_iContentHeight*offset;

		//calculate new scroller position
		int maxYPos = (int)m_Widget2DProps.Dimensions.tHeight - (int)m_Button->GetDimension().tHeight;
		int newYPosRel = (int)(offset * maxYPos);
		m_Button->SetPosition({ m_Button->GetPosition().tX, (int)(newYPosRel + m_iBtnSave.tY) });

		//Shift content
		float contentOffset = (1.0f - offset)*(m_iContentHeight - m_Widget2DProps.Dimensions.tHeight);
		for (auto widget : m_ConnectedWidgets) {
			widget.first->SetPosition({widget.second.tX, widget.second.tY + (int)contentOffset });
		}
	}

	void VerticalScroller::Clear()
	{
		for (std::list<ConnectedWidget>::iterator it = m_ConnectedWidgets.begin(); it != m_ConnectedWidgets.end(); it++) {
			delete (*it).first;
		}
		m_ConnectedWidgets.clear();

		m_DragData[2] = { 0,0 };
		m_iContentHeight = 0;
		m_SmartXPlace = 0;
		m_Rows = 0;
		m_iDisplayLine = (int)m_Widget2DProps.Dimensions.tHeight;

		UpdateScrollerDim();
		SetOffset(0.0f);
	}

	void VerticalScroller::SetInsertOffsets(Point<int> offset)
	{
		m_InsertOffs = offset;
		//Recalculate positions?!
	}


	void VerticalScroller::PushBack(Widget2D* widget, int flags)
	{
		Point<int> pushPos = { 0,0 };

		//Get y offset value
		int applyYOffset = m_InsertOffs.tY;

		//Perform smart adding
		if (flags & ScrollerFlags::SmartPlacement) {

			if (!m_ConnectedWidgets.empty() && m_ConnectedWidgets.back().first->GetDimension().tHeight != widget->GetDimension().tHeight) {
				ANGE_WARNING("VerticalScroller row should have a constant height. PushBack() abroted.");
				return;
			}

				//Check if we are in maximum width range (of scroller area)
				if (m_SmartXPlace + (int)widget->GetDimension().tWidth > (int)m_ScrAreaDim.tWidth) {
					//Perform adding to new row
					++m_Rows;
					m_iContentHeight += (int)widget->GetDimension().tHeight + applyYOffset;


					//Fix x position
					pushPos.tX = m_ScrAreaPos.tX;
					m_SmartXPlace = 0;
					
					//Fix y position
					pushPos.tY = m_ScrAreaPos.tY + (int)m_ScrAreaDim.tHeight - m_iContentHeight + (int)widget->GetDimension().tHeight;

				} else {
					//Perform adding to the same row
					pushPos.tX = m_ScrAreaPos.tX + m_SmartXPlace;
					if (m_Rows == 0) {
						m_iContentHeight = (int)widget->GetDimension().tHeight;
						pushPos.tY = m_ScrAreaPos.tY + (int)m_ScrAreaDim.tHeight;
					} else {
						pushPos.tY = m_ScrAreaPos.tY + (int)m_ScrAreaDim.tHeight - m_iContentHeight + (int)widget->GetDimension().tHeight;
					}
					
				}

			//Inc m_SmartXPlace
			m_SmartXPlace += (int)widget->GetDimension().tWidth + m_InsertOffs.tX;
			
		} else {
			//Perform standard adding (create next row)
			pushPos = { m_ScrAreaPos.tX, -m_iContentHeight + m_ScrAreaPos.tY + (int)m_ScrAreaDim.tHeight };
			m_iContentHeight += (int)widget->GetDimension().tHeight + applyYOffset;
		}

		//Add widget to scroller
		widget->SetPosition(pushPos);
		widget->SetFlags(Anchor::Left | Anchor::Top);
		
		widget->UnregisterEvent(EventType::DrawableInvokeRender);
		widget->UnregisterEvent(EventType::WindowResize);

		ANGE_WARNING("%i %i  CH: %i", pushPos.tX, pushPos.tY, m_iContentHeight);

		m_ConnectedWidgets.push_back(std::pair<Widget2D*, Point<int>>(widget, pushPos)); //+ (int)widget->GetDimension().tHeight


		if (m_iContentHeight > (int)m_ScrAreaDim.tHeight) {
			UpdateScrollerDim();
			int max = (m_iContentHeight - (int)m_Widget2DProps.Dimensions.tHeight);
			int curLine = (m_iDisplayLine - (int)m_Widget2DProps.Dimensions.tHeight);
			SetOffset(1.0f - ((float)curLine /max));
		}

	}


	bool VerticalScroller::OnDrawableInvokeRender(Event* ev)
	{
		Render();
		GLint scissorBackup[4];
		glGetIntegerv(GL_SCISSOR_BOX, &scissorBackup[0]);

		glScissor(m_ScrAreaPos.tX, m_ScrAreaPos.tY, (GLsizei)m_ScrAreaDim.tWidth, (GLsizei)m_ScrAreaDim.tHeight);
		for (auto w : m_ConnectedWidgets){
			w.first->Render();
		}
		glScissor(scissorBackup[0], scissorBackup[1], scissorBackup[2], scissorBackup[3]);

		return false;
	}

	bool VerticalScroller::OnMouseMove(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseMoveEvent* mme = (MouseMoveEvent*)ev;
		auto pos = mme->GetPosition();

			//Update graphics & state
			if (m_Button->GetState() == WidgetMouseState::Press) {
				m_DragData[1] = pos - m_AnchorOffsets;


				//Calculate some ****
				int fix = m_DragData[2].tY - m_iBtnSave.tY - m_AnchorOffsets.tY;

				auto offset = (m_DragData[1].tY - m_DragData[0].tY) + m_iBtnSave.tY + m_AnchorOffsets.tY + fix;
				if (offset < m_Widget2DProps.Position.tY + m_AnchorOffsets.tY) {
					offset = m_Widget2DProps.Position.tY + m_AnchorOffsets.tY;
				} else if(offset + m_Button->GetDimension().tHeight > m_Widget2DProps.Position.tY + m_Widget2DProps.Dimensions.tHeight + m_AnchorOffsets.tY) {
					offset = m_Widget2DProps.Position.tY + (int)m_Widget2DProps.Dimensions.tHeight - (int)m_Button->GetDimension().tHeight + +m_AnchorOffsets.tY;
				}

				int maximum = (int)m_Widget2DProps.Dimensions.tHeight - (int)m_Button->GetDimension().tHeight;
				int current = offset - (m_Widget2DProps.Position.tY+m_AnchorOffsets.tY);
				float ratio = 1.0f-(float)current / maximum;
				int contentOffset = (int)(ratio * (m_iContentHeight - m_Widget2DProps.Dimensions.tHeight));

				//Update
				m_Button->SetPosition({ m_iBtnSave.tX + m_AnchorOffsets.tX, offset });
				m_iDisplayLine = contentOffset+(int)m_Widget2DProps.Dimensions.tHeight;
				SetOffset(1.0f-ratio);

			}

			if (CheckCoords(pos)) {
				return true;
			}

		return false;
	}

	bool VerticalScroller::OnMouseClick(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseClickEvent* mce = (MouseClickEvent*)ev;
		if (auto pos = mce->GetPosition(); CheckCoords(pos- m_AnchorOffsets)) {
			if (mce->GetAction() == 1) {
				m_DragData[0] = pos;
				m_DragData[2] = m_Button->GetPosition();
				m_iDisplayLineBackup = m_iDisplayLine;
			}
			return true;
		}
		return false;
	}
	
	bool VerticalScroller::OnWindowResize(Event* ev)
	{
		Dimension<size_t> diff = m_Widget2DProps.Dimensions;
		WindowResizeEvent* wre = (WindowResizeEvent*)ev;
		if (m_Widget2DProps.iFlags & ResizePolicy::AutoFill) {
			m_Widget2DProps.Position = Point<int>({ 0, 0 });
			m_Widget2DProps.Dimensions = wre->GetDimension();
		} else {
			CalcResizeData(wre->GetDimension(), m_Widget2DProps.Dimensions, m_Widget2DProps.Position);
		}

		diff = diff - m_Widget2DProps.Dimensions;

		//Update
		//m_ScrAreaDim += diff;
		m_iBtnSave = m_Widget2DProps.Position;
		CalcAnchorOffsets();
		//UpdateBtnPosition();
		m_Widget2DProps.bIfChanged = true;
		return false;
	}
	
	bool VerticalScroller::OnMouseEnter(Event* ev)
	{
		return false;
	}

	bool VerticalScroller::OnWindowClose(Event* ev)
	{
		DisableWidget();
		return false;
	}


	//------------------------------------------------------------------------------------------------------------------

	CustomWidget::CustomWidget(Window* window, const Widget2DProps& props) : Widget2D(window, props)
	{
		m_WidgetType = WidgetType::Custom;
	}
	
	CustomWidget::CustomWidget(const CustomWidget& copy)
	{
		std::cout << "CW copy" << std::endl;
	}


	CustomWidget::~CustomWidget()
	{
	}

	void CustomWidget::SetPosition(Point<int> newPosition)
	{
		Point<int> diff = { newPosition.tX - m_Widget2DProps.Position.tX, newPosition.tY - m_Widget2DProps.Position.tY };
		m_Widget2DProps.Position = newPosition;
		m_ResizableProps.BasePosition = newPosition;
		for (auto it : m_Components) {
			it.second->ChangePosition(diff);
		}
	}

	void CustomWidget::ChangePosition(Point<int> positionChange)
	{
		m_Widget2DProps.Position += positionChange;
		m_ResizableProps.BasePosition += positionChange;
		for (auto it : m_Components) {
			it.second->ChangePosition(positionChange);
		}
	}

	void CustomWidget::SetVisibility(bool mode)
	{
		m_Widget2DProps.bVisibility = mode;
		for (auto it : m_Components) {
			it.second->SetVisibility(mode);
		}
	}
	
	bool CustomWidget::GetVisibility() const
	{
		return 	m_Widget2DProps.bVisibility;
	}

	void CustomWidget::UnregisterEvent(EventType ev)
	{
		for (auto it : m_Components) {
			it.second->UnregisterEvent(ev);
		}
	}


	void CustomWidget::EnableWidget()
	{
		for (auto it : m_Components) {
			it.second->EnableWidget();
		}
	}
	
	void CustomWidget::DisableWidget()
	{
		for (auto it : m_Components) {
			it.second->DisableWidget();
		}
	}
	
	void CustomWidget::Render()
	{
		for (auto it : m_Components) {
			it.second->Render();
		}
	}

	void CustomWidget::TranslateAnchor(Point<int>& position, int oldFlags, int newFlags)
	{
		if (oldFlags & Anchor::Left) {
			if (newFlags & Anchor::Left) {
				position.tX = m_Widget2DProps.Position.tX;
			} else if (newFlags & Anchor::Right) {
				position.tX = m_Widget2DProps.Position.tX + ((int)m_Widget2DProps.Dimensions.tWidth);
			} else if (newFlags & Anchor::HorizontalCenter) {
				position.tX = m_Widget2DProps.Position.tX + (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			}
		} else if (oldFlags & Anchor::Right) {
			if (newFlags & Anchor::Left) {
				position.tX = m_Widget2DProps.Position.tX - ((int)m_Widget2DProps.Dimensions.tWidth);
			} else if (newFlags & Anchor::Right) {
				position.tX = m_Widget2DProps.Position.tX;
			} else if (newFlags & Anchor::HorizontalCenter) {
				position.tX = m_Widget2DProps.Position.tX - (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			}
		} else if (oldFlags & Anchor::HorizontalCenter) {
			if (newFlags & Anchor::Left) {
				position.tX = m_Widget2DProps.Position.tX - (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			} else if (newFlags & Anchor::Right) {
				position.tX = m_Widget2DProps.Position.tX + (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			} else if (newFlags & Anchor::HorizontalCenter) {
				position.tX = m_Widget2DProps.Position.tX;
			}
		}

		if (oldFlags & Anchor::Bottom) {
			if (newFlags & Anchor::Bottom) {
				position.tY = m_Widget2DProps.Position.tY;
			} else if (newFlags & Anchor::Top) {
				position.tY = m_Widget2DProps.Position.tY + ((int)m_Widget2DProps.Dimensions.tHeight);
			} else if (newFlags & Anchor::VerticalCenter) {
				position.tY = m_Widget2DProps.Position.tY + (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			}
		} else if (oldFlags & Anchor::Top) {
			if (newFlags & Anchor::Bottom) {
				position.tY = m_Widget2DProps.Position.tY - ((int)m_Widget2DProps.Dimensions.tHeight);
			} else if (newFlags & Anchor::Top) {
				position.tY = m_Widget2DProps.Position.tY;
			} else if (newFlags & Anchor::VerticalCenter) {
				position.tY = m_Widget2DProps.Position.tY - (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			}
		} else if (oldFlags & Anchor::VerticalCenter) {
			if (newFlags & Anchor::Bottom) {
				position.tY = m_Widget2DProps.Position.tY - (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			} else if (newFlags & Anchor::Top) {
				position.tY = m_Widget2DProps.Position.tY + (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			} else if (newFlags & Anchor::VerticalCenter) {
				position.tY = m_Widget2DProps.Position.tY;
			}
		}
	}

	void  CustomWidget::SetResizeProportions(int x, int y, int w, int h)
	{
		for (auto it : m_Components) {
			it.second->SetResizeProportions(x,y,0,0);
		}
	}

	Widget2D* CustomWidget::GetComponent(int idx)
	{
		if (auto it = m_Components.find(idx); it != m_Components.end()) {
			return it->second;
		}
		return nullptr;
	}

	void CustomWidget::AddComponent(int idx, Widget2D* widget)
	{
		if (widget != nullptr) {
			m_Components.insert(std::pair<int, Widget2D*>(idx, widget));
		}
	}


	//------------------------------------------------------------------------------------------------------------------


}
