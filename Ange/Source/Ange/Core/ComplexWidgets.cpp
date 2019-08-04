#include "Apch.h"
#include "ComplexWidgets.h"
#include "Ange/Core/Widgets.h"
#include "Ange/Core/Window.h"
#include "Ange/Core/Functions.h"
#include <clocale>
#include <codecvt>
#include <algorithm>
#include <limits>
#include <type_traits>


namespace Ange {

	template <class T>
	SimpleButton<T>::SimpleButton(Window* window, const Widget2DProps& props, const SimpleButtonTheme& btnTheme, std::wstring btnText, Texture* imageTex) :
		Widget2D(window, props),
		m_BtnTheme(btnTheme),
		m_Callback(nullptr),
		m_iTicks(0),
		m_AnchorOffsets{ 0,0 },
		m_bDrag(false),
		m_bBypassEventsReturn(false),
		m_bEnableMoveEv(false)
	{
		Widget2DProps textCenterProps = props;
		textCenterProps.Position = Point<int>({ (int)(props.Position.tX + props.Dimensions.tWidth / 2), (int)(props.Position.tY + props.Dimensions.tHeight / 2) });
		textCenterProps.iFlags = Anchor::HorizontalCenter | Anchor::VerticalCenter;

		m_State = WidgetMouseState::Normal;
		m_Text = new Text(window, textCenterProps, btnTheme.TextTh, btnText);
		
		if (std::is_same<T, Background>::value) {
			m_FrontWidget = (T*)new Background(window, props, { btnTheme.Base[0].Tint, btnTheme.Base[0].BorderColor, m_BtnTheme.BorderSize });
		} else if (std::is_same<T, Image>::value) {
			if(imageTex == nullptr) ANGE_ERROR("[SimpleButton] When front widget is 'Image', you need to pass valid pointer to texture. [imageTex == nullptr]");
			m_FrontWidget = (T*)new Image(window, props, {btnTheme.Base[0].Tint, btnTheme.Base[0].BorderColor, m_BtnTheme.BorderSize}, imageTex);
		}
		
		EnableWidget();
		CalcAnchorOffsets();
	}

	template<class T>
	SimpleButton<T>::SimpleButton(Window* window, const Widget2DProps& props, const Theme& theme, std::wstring btnText, Texture* imageTex) :
		SimpleButton<T>(window, props, std::is_same<T, Background>::value ? theme.SimpleButtonBG : theme.SimpleButtonIMG, btnText, imageTex)
	{
	}

	template <class T>
	SimpleButton<T>::SimpleButton(const SimpleButton& copy) :
		Widget2D(copy)
	{
		m_BtnTheme = copy.m_BtnTheme;
		m_iTicks = copy.m_iTicks;
		m_AnchorOffsets = copy.m_AnchorOffsets;
		m_bDrag = copy.m_bDrag;
		m_bBypassEventsReturn = copy.m_bBypassEventsReturn;
		m_bEnableMoveEv = copy.m_bEnableMoveEv;
		m_State = copy.m_State;
		m_Callback = nullptr;

		m_Text = new Text(*(Text*)copy.m_Text);
		m_FrontWidget = new T(*(T*)copy.GetFrontObject());

		EnableWidget();
		CalcAnchorOffsets();
	}

	template <class T>
	SimpleButton<T>::~SimpleButton()
	{
		DisableWidget();
		delete m_Text;
		delete m_FrontWidget;
	}

	template <class T>
	SimpleButton<T>& SimpleButton<T>::operator=(SimpleButton<T> rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	template <class T>
	void swap(SimpleButton<T>& first, SimpleButton<T>& second) noexcept
	{
		using std::swap;
		swap(first.m_BtnTheme, second.m_BtnTheme);
		swap(first.m_iTicks, second.m_iTicks);
		swap(first.m_AnchorOffsets, second.m_AnchorOffsets);
		swap(first.m_bDrag, second.m_bDrag);
		swap(first.m_bBypassEventsReturn, second.m_bBypassEventsReturn);
		swap(first.m_bEnableMoveEv, second.m_bEnableMoveEv);
		swap(first.m_State, second.m_State);
		//wap(first.m_Callback, second.m_Callback);
		swap(*(first.m_FrontWidget), *(second.m_FrontWidget));
		swap(*(first.m_Text), *(second.m_Text)); //Hmmmm, should i rly swap this if position is swapped? 
		swap(static_cast<Widget2D&>(first), static_cast<Widget2D&>(second));
	}

	template <class T>
	SimpleButton<T>* SimpleButton<T>::Clone() const
	{
		return new SimpleButton(*this);
	}

	//Setters
	template<class T>
	void SimpleButton<T>::SetColor(WidgetMouseState forState, Color color)
	{
		m_BtnTheme.Base[forState].Tint = color;
		if (forState == m_State) {
			m_FrontWidget->SetColor(m_BtnTheme.Base[forState].Tint);
		}
	}

	template<class T>
	void SimpleButton<T>::SetColor(Color normal, Color hover, Color press)
	{
		m_BtnTheme.Base[WidgetMouseState::Normal].Tint = normal;
		m_BtnTheme.Base[WidgetMouseState::Hover].Tint = hover;
		m_BtnTheme.Base[WidgetMouseState::Press].Tint = press;
		m_FrontWidget->SetColor(m_BtnTheme.Base[m_State].Tint);
	}

	template<class T>
	void SimpleButton<T>::SetBorderColor(WidgetMouseState forState, Color color)
	{
		m_BtnTheme.Base[forState].BorderColor = color;
		if (forState == m_State) {
			m_FrontWidget->SetBorderColor(m_BtnTheme.Base[forState].BorderColor);
		}
	}

	template <class T>
	void SimpleButton<T>::SetBorderColor(Color normal, Color hover, Color press)
	{
		m_BtnTheme.Base[WidgetMouseState::Normal].BorderColor = normal;
		m_BtnTheme.Base[WidgetMouseState::Hover].BorderColor = hover;
		m_BtnTheme.Base[WidgetMouseState::Press].BorderColor = press;
		m_FrontWidget->SetBorderColor(m_BtnTheme.Base[m_State].BorderColor);
	}

	template <class T>
	void SimpleButton<T>::SetBoderSize(Dimension<int> newBorderSize)
	{
		m_FrontWidget->SetBorderSize(newBorderSize);
		m_BtnTheme.BorderSize = newBorderSize;
	}

	template <class T>
	void SimpleButton<T>::SetFontSize(int newSize)
	{
		m_Text->SetFontSize(newSize);
		m_BtnTheme.TextTh.iFontSize = newSize;
	}

	template <class T>
	void SimpleButton<T>::SetText(std::wstring newText)
	{
		m_Text->SetText(newText);
	}

	template <class T>
	void SimpleButton<T>::SetFont(Font* newFont)
	{
		m_Text->SetFont(newFont);
		m_BtnTheme.TextTh.UsedFont = newFont;
	}

	template <class T>
	void SimpleButton<T>::SetFontColor(Color newColor)
	{
		m_Text->SetColor(newColor);
		m_BtnTheme.TextTh.Tint = newColor;
	}

	template <class T>
	void SimpleButton<T>::SetState(WidgetMouseState state)
	{
		m_State = state;
		m_FrontWidget->SetColor(m_BtnTheme.Base[m_State].Tint);
		m_FrontWidget->SetBorderColor(m_BtnTheme.Base[m_State].BorderColor);
		m_iTicks = 0;
		if (state != WidgetMouseState::Press) {
			m_bDrag = false;
		}
	}

	template <class T>
	void SimpleButton<T>::SetFlags(int newFlags)
	{
		Widget2D::SetFlags(newFlags);
		m_FrontWidget->SetFlags(newFlags);
		m_Widget2DProps.iFlags = newFlags;
		CalcAnchorOffsets();
	}

	template <class T>
	void SimpleButton<T>::SetBypassEventReturn(bool mode)
	{
		m_bBypassEventsReturn = mode;
	}

	template <class T>
	void SimpleButton<T>::SetBypassMoveEvWhileDrag(bool mode)
	{
		m_bEnableMoveEv = mode;
	}


	template <class T>
	void SimpleButton<T>::UnregisterEvent(EventType eventType)
	{
		m_Text->UnregisterEvent(eventType);
		m_FrontWidget->UnregisterEvent(eventType);
	}

	template <class T>
	void SimpleButton<T>::Resize(Dimension<size_t> newDim)
	{
		m_FrontWidget->Resize(newDim);
		m_Text->Resize(newDim);
		m_Widget2DProps.Dimensions = newDim;
		m_ResizableProps.BaseDimension = newDim;
		CalcAnchorOffsets();
	}


	//Getters
	template <class T>
	Color SimpleButton<T>::GetColor(WidgetMouseState forState) const
	{
		return m_BtnTheme.Base[forState].Tint;
	}

	template <class T>
	Color SimpleButton<T>::GetBorderColor(WidgetMouseState forState) const
	{
		return m_BtnTheme.Base[forState].BorderColor;
	}

	template <class T>
	Dimension<int> SimpleButton<T>::GetBoderSize() const
	{
		return m_FrontWidget->GetBorderSize();
	}

	template <class T>
	int SimpleButton<T>::GetFontSize() const
	{
		return m_Text->GetFontSize();
	}

	template <class T>
	std::wstring SimpleButton<T>::GetText() const
	{
		return m_Text->GetText();
	}

	template <class T>
	Color SimpleButton<T>::GetFontColor() const
	{
		return m_Text->GetColor();
	}

	template <class T>
	Font* SimpleButton<T>::GetUsedFont() const
	{
		return m_Text->GetFont();
	}

	template <class T>
	T* SimpleButton<T>::GetFrontObject() const
	{
		return m_FrontWidget;
	}
	

	template <class T>
	void SimpleButton<T>::SetResizeProportions(int x, int y, int w, int h)
	{
		m_FrontWidget->SetResizeProportions(x, y, w, h);
		m_Text->SetResizeProportions(0, 0, w, h);
		Resizable2D::SetResizeProportions(x, y, w, h);
	}
		
	template <class T>
	void SimpleButton<T>::SetPosition(Point<int> newPosition)
	{
		m_Widget2DProps.Position = newPosition;
		m_ResizableProps.BasePosition = newPosition;

		auto currentPosition = m_FrontWidget->GetPosition();
		Point<int> diff = { newPosition.tX - currentPosition.tX - 1, newPosition.tY - currentPosition.tY  - 1 };
		m_FrontWidget->SetPosition(newPosition);
		diff.tX += 1;
		diff.tY += 1;
		m_Text->ChangePosition(diff);
	}

	template <class T>
	void SimpleButton<T>::ChangePosition(Point<int> positionChange)
	{
		m_FrontWidget->ChangePosition(positionChange);
		m_Text->ChangePosition(positionChange);

		m_Widget2DProps.Position += positionChange;
		m_ResizableProps.BasePosition += positionChange;
	}

	template <class T>
	void SimpleButton<T>::SetVisibility(bool mode)
	{
		m_FrontWidget->SetVisibility(mode);
		m_Text->SetVisibility(mode);
		m_Widget2DProps.bVisibility = mode;
	}

	template <class T>
	bool SimpleButton<T>::GetVisibility() const
	{
		return m_Widget2DProps.bVisibility;
	}

	template <class T>
	void SimpleButton<T>::SetCallback(Callback function)
	{
		m_Callback = function;
	}

	template <class T>
	void SimpleButton<T>::ResetCallback()
	{
		m_Callback = nullptr;
	}

	template <class T>
	WidgetMouseState SimpleButton<T>::GetState()
	{
		return m_State;
	}

	template <class T>
	bool SimpleButton<T>::GetDragInfo()
	{
		return m_bDrag;
	}


	//Derived
	template <class T>
	void SimpleButton<T>::EnableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		ANGE_ZERO_TEST(m_ParentWindow->IfOpen(), "m_ParentWindow == 0! (window is closed)");

		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseMove, I_BIND(SimpleButton, OnMouseMove)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseEnter, I_BIND(SimpleButton, OnMouseEnter)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseClick, I_BIND(SimpleButton, OnMouseClick)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowResize, I_BIND(SimpleButton, OnWindowResize)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowClose, I_BIND(SimpleButton, OnWindowClose)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::Tick, I_BIND(SimpleButton, OnWindowTick)));

		m_FrontWidget->EnableWidget();
		m_Text->EnableWidget();
	}

	template <class T>
	void SimpleButton<T>::DisableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		if (m_ParentWindow == nullptr) return;
		for (auto it : m_Bindings)
		{
			m_ParentWindow->UnbindEvent(it);
		}
		m_Bindings.clear();

		m_FrontWidget->DisableWidget();
		m_Text->DisableWidget();
	}

	template <class T>
	void SimpleButton<T>::Render()
	{
		m_FrontWidget->Render();
		m_Text->Render();
	}

	//Helper funciton
	template <class T>
	void SimpleButton<T>::CalcAnchorOffsets()
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
	template <class T>
	bool SimpleButton<T>::OnWindowTick(Event* ev)
	{
		++m_iTicks;
		if (m_State == WidgetMouseState::Press && m_iTicks > 10 && GetVisibility() == true && m_bDrag == false) {

			m_FrontWidget->SetColor(m_BtnTheme.Base[WidgetMouseState::Hover].Tint);
			m_FrontWidget->SetBorderColor(m_BtnTheme.Base[WidgetMouseState::Hover].BorderColor);

			m_State = WidgetMouseState::Hover;
			m_iTicks = 0;
		}
		return false;
	}

	template <class T>
	bool SimpleButton<T>::OnWindowClose(Event* ev)
	{
		DisableWidget();
		return false;
	}

	template <class T>
	bool SimpleButton<T>::OnWindowResize(Event* ev)
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

	template <class T>
	bool SimpleButton<T>::OnMouseMove(Event* ev)
	{

		if (GetVisibility() == false) return false;
		if (m_bDrag == true && m_bEnableMoveEv == false) {
			return false;
		}
		MouseMoveEvent* mme = (MouseMoveEvent*)ev;
		auto pos = mme->GetPosition() - m_AnchorOffsets;
		if (bool inside = CheckCoords(pos); inside || m_bDrag) {

			mme->SetCheckStatus(inside);

			//Update graphics & state
			if (m_State != WidgetMouseState::Hover){

				m_FrontWidget->SetColor(m_BtnTheme.Base[WidgetMouseState::Hover].Tint);
				m_FrontWidget->SetBorderColor(m_BtnTheme.Base[WidgetMouseState::Hover].BorderColor);
			}

			m_State = WidgetMouseState::Hover;
			if (m_Callback != nullptr) m_Callback(mme);
			if (m_bBypassEventsReturn) return false;
			return false;
		}
		//Update graphics & state
		if (m_State != WidgetMouseState::Normal) {

			m_FrontWidget->SetColor(m_BtnTheme.Base[WidgetMouseState::Normal].Tint);
			m_FrontWidget->SetBorderColor(m_BtnTheme.Base[WidgetMouseState::Normal].BorderColor);

		}
		m_State = WidgetMouseState::Normal;
		return false;
	}

	template <class T>
	bool SimpleButton<T>::OnMouseClick(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseClickEvent* mce = (MouseClickEvent*)ev;
		if (auto pos = mce->GetPosition() - m_AnchorOffsets; CheckCoords(pos)) {
			if (mce->GetAction() == 1) {
				//Update graphics & state
				if (m_State != WidgetMouseState::Press) {

					m_FrontWidget->SetColor(m_BtnTheme.Base[WidgetMouseState::Press].Tint);
					m_FrontWidget->SetBorderColor(m_BtnTheme.Base[WidgetMouseState::Press].BorderColor);
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

			m_FrontWidget->SetColor(m_BtnTheme.Base[WidgetMouseState::Normal].Tint);
			m_FrontWidget->SetBorderColor(m_BtnTheme.Base[WidgetMouseState::Normal].BorderColor);

		}
		m_State = WidgetMouseState::Normal;
		return false;
	}

	template <class T>
	bool SimpleButton<T>::OnMouseEnter(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseEnterEvent* mee = (MouseEnterEvent*)ev;
		if (mee->GetState() == 0){
			//Update graphics & state
			if (m_State != WidgetMouseState::Normal) {
				m_FrontWidget->SetColor(m_BtnTheme.Base[WidgetMouseState::Normal].Tint);
				m_FrontWidget->SetBorderColor(m_BtnTheme.Base[WidgetMouseState::Normal].BorderColor);
			}
			m_State = WidgetMouseState::Normal;
		}
		return false;
	}

	template class SimpleButton<Background>;
	template class SimpleButton<Image>;

	//--------------------------------------------------------------------------------


	SimpleInput::SimpleInput(Window* window, const Widget2DProps& props, const SimpleInputTheme& theme, std::wstring defaultText) :
		Widget2D(window, props),
		m_InpTheme(theme),
		m_Callback(nullptr),
		m_iTicks(0),
		m_bDrag(false),
		m_iDragStart(-1),
		m_iDragEnd(-1),
		m_FilterFunc(nullptr)
	{
		//Calculate some vars
		Widget2DProps textFixedProps = props;
		textFixedProps.Position += m_InpTheme.Margins;
		textFixedProps.iFlags = Anchor::Left | Anchor::Bottom;
		textFixedProps.Dimensions -= {(size_t)m_InpTheme.Margins.tX * 2, (size_t)m_InpTheme.Margins.tY * 2};

		int fixBarY = 0;
		if (props.iFlags & Anchor::VerticalCenter) fixBarY = (int)props.Dimensions.tHeight / 2;
		if (props.iFlags & Anchor::Top) fixBarY = (int)props.Dimensions.tHeight;

		//Fill everything up
		m_State = WidgetMouseState::Normal;
		m_Background = new Background(window, props, { theme.Base[0].Tint, theme.Base[0].BorderColor, theme.BorderSize });
		m_BottomBar = new Background(window, { {props.Position.tX, props.Position.tY-fixBarY}, {props.Dimensions.tWidth, (size_t)theme.iBarHeight}, (props.iFlags & 0b00011100) | Anchor::Bottom }, { theme.Bar, {0,0,0,0}, {0,0} });
		m_DefaultText = new Text(window, textFixedProps, theme.DefaultText, defaultText);
		m_Text = new Text(window, textFixedProps, theme.Text);

		m_Selection = new Rectangle2D(window, { {0,0}, {0,0} }, theme.Selection);
		m_Selection->SetVisibility(false);
		m_Prompt = new Rectangle2D(
			window,
			//{ "Prompt", textFixedProps.Position, {2, props.Dimensions.tHeight-2*m_Margins.tY}, Anchor::Left | Anchor::Bottom},
			{ {textFixedProps.Position.tX + theme.Margins.tX, textFixedProps.Position.tY + theme.Margins.tY}, {2, (size_t)(props.Dimensions.tHeight-theme.Margins.tY*2)}, Anchor::Left | Anchor::Bottom },
			theme.Prompt
		);

		if (!defaultText.empty()) {
			m_Text->SetVisibility(false);
			m_DefaultText->SetVisibility(true);
		}
		else {
			m_Text->SetVisibility(true);
			m_DefaultText->SetVisibility(false);
		}
		m_Prompt->SetVisibility(false);
		m_BottomBar->SetVisibility(true);

		EnableWidget();
		CalcAnchorOffsets();
	}

	SimpleInput::SimpleInput(Window* window, const Widget2DProps& props, const Theme& theme, std::wstring defaultText) :
		SimpleInput(window, props, theme.SimpleInput, defaultText)
	{
	}

	SimpleInput::SimpleInput(const SimpleInput& copy):
		Widget2D(copy)
	{
		m_InpTheme = copy.m_InpTheme;
		m_iTicks = copy.m_iTicks;
		m_bDrag = copy.m_bDrag;
		m_iDragStart = copy.m_iDragStart;
		m_iDragEnd = copy.m_iDragEnd;
		m_Enter = copy.m_Enter;
		m_iPromptIdx = copy.m_iPromptIdx;
		m_fPromptPos = copy.m_fPromptPos;
		m_bActive = copy.m_bActive;
		m_AnchorOffsets = copy.m_AnchorOffsets;
		m_Callback = nullptr;
		m_FilterFunc = nullptr;

		m_Background = new Background(*copy.m_Background);
		m_BottomBar = new Background(*copy.m_BottomBar);
		m_DefaultText = new Text(*copy.m_DefaultText);
		m_Text = new Text(*copy.m_Text);
		m_Selection = new Rectangle2D(*copy.m_Selection);
		m_Prompt = new Rectangle2D(*copy.m_Prompt);

		EnableWidget();
		CalcAnchorOffsets();
	}

	SimpleInput::~SimpleInput()
	{
		DisableWidget();
		delete m_Background;
		delete m_Text;
		delete m_Prompt;
		delete m_Selection;
		delete m_DefaultText;
		delete m_BottomBar;
	}

	SimpleInput& SimpleInput::operator=(SimpleInput rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	void swap(SimpleInput& first, SimpleInput& second) noexcept
	{
		using std::swap;
		swap(first.m_InpTheme, second.m_InpTheme);
		swap(first.m_iTicks, second.m_iTicks);
		swap(first.m_AnchorOffsets, second.m_AnchorOffsets);
		swap(first.m_bDrag, second.m_bDrag);
		swap(first.m_iDragStart, second.m_iDragStart);
		swap(first.m_iDragEnd, second.m_iDragEnd);
		swap(first.m_Enter, second.m_Enter);
		swap(first.m_State, second.m_State);
		//swap(first.m_Callback, second.m_Callback);
		swap(first.m_iPromptIdx, second.m_iPromptIdx);
		swap(first.m_fPromptPos, second.m_fPromptPos);
		swap(first.m_bActive, second.m_bActive);

		swap(*(first.m_Background), *(second.m_Background));
		swap(*(first.m_Text), *(second.m_Text)); //Hmmmm, should i rly swap this if position is swapped? 
		swap(*(first.m_Prompt), *(second.m_Prompt));
		swap(*(first.m_Selection), *(second.m_Selection));
		swap(*(first.m_DefaultText), *(second.m_DefaultText));
		swap(*(first.m_BottomBar), *(second.m_BottomBar));

		swap(static_cast<Widget2D&>(first), static_cast<Widget2D&>(second));
	}

	SimpleInput* SimpleInput::Clone() const
	{
		return new SimpleInput(*this);
	}

	//Setters
	void SimpleInput::SetColor(WidgetMouseState forState, Color color)
	{
		m_InpTheme.Base[forState].Tint = color;
		if (forState == m_State) m_Background->SetColor(m_InpTheme.Base[forState].Tint);
	}

	void SimpleInput::SetColor(Color normal, Color hover, Color press)
	{
		m_InpTheme.Base[WidgetMouseState::Normal].Tint = normal;
		m_InpTheme.Base[WidgetMouseState::Hover].Tint = hover;
		m_InpTheme.Base[WidgetMouseState::Press].Tint = press;
		m_Background->SetColor(m_InpTheme.Base[m_State].Tint);
	}

	void SimpleInput::SetBorderColor(WidgetMouseState forState, Color color)
	{
		m_InpTheme.Base[forState].BorderColor = color;
		if (forState == m_State) m_Background->SetBorderColor(m_InpTheme.Base[forState].BorderColor);
	}

	void SimpleInput::SetBorderColor(Color normal, Color hover, Color press)
	{
		m_InpTheme.Base[WidgetMouseState::Normal].BorderColor = normal;
		m_InpTheme.Base[WidgetMouseState::Hover].BorderColor = hover;
		m_InpTheme.Base[WidgetMouseState::Press].BorderColor = press;
		m_Background->SetBorderColor(m_InpTheme.Base[m_State].BorderColor);
	}

	void SimpleInput::SetBoderSize(Dimension<int> newBorderSize)
	{
		m_Background->SetBorderSize(newBorderSize);
		m_InpTheme.BorderSize = newBorderSize;
	}

	void SimpleInput::SetFontSize(int newSize)
	{
		m_Text->SetFontSize(newSize);
		m_InpTheme.Text.iFontSize = newSize;
		m_InpTheme.DefaultText.iFontSize = newSize;
	}

	void SimpleInput::SetText(std::wstring newText)
	{
		m_Text->SetText(newText);
		if (!newText.empty())
		{
			m_DefaultText->SetVisibility(false);
			m_Text->SetVisibility(true);
		} else {
			m_DefaultText->SetVisibility(true);
			m_Text->SetVisibility(false);
		}
	}

	void SimpleInput::SetFont(Font* newFont)
	{
		m_Text->SetFont(newFont);
		m_InpTheme.DefaultText.UsedFont = newFont;
		m_InpTheme.Text.UsedFont = newFont;
	}

	void SimpleInput::SetFontColor(Color newColor)
	{
		m_Text->SetColor(newColor);
		m_InpTheme.DefaultText.Tint = newColor;
		m_InpTheme.Text.Tint = newColor;
	}

	void SimpleInput::SetFlags(int newFlags)
	{
		Widget2D::SetFlags(newFlags);
		m_Background->SetFlags(newFlags);
		m_BottomBar->SetFlags(newFlags);
		int fixBarY = 0;
		if (m_Widget2DProps.iFlags & Anchor::VerticalCenter) fixBarY = (int)m_Widget2DProps.Dimensions.tHeight / 2;
		if (m_Widget2DProps.iFlags & Anchor::Top) fixBarY = (int)m_Widget2DProps.Dimensions.tHeight;
		m_BottomBar->SetPosition({m_Widget2DProps.Position.tX, m_Widget2DProps.Position.tY + fixBarY});
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
		m_BottomBar->UnregisterEvent(eventType);
	}

	//Getters
	Color SimpleInput::GetColor(WidgetMouseState forState) const
	{
		return m_InpTheme.Base[forState].Tint;
	}

	Color SimpleInput::GetBorderColor(WidgetMouseState forState) const
	{
		return m_InpTheme.Base[forState].BorderColor;
	}

	const Dimension<int>& SimpleInput::GetBoderSize() const
	{
		return m_Background->GetBorderSize();
	}

	int SimpleInput::GetFontSize() const
	{
		return m_Text->GetFontSize();
	}

	std::wstring SimpleInput::GetText() const
	{
		return m_Text->GetText();
	}

	std::wstring SimpleInput::GetTextWhenEnter()
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
		}
		else if (m_Widget2DProps.iFlags & Anchor::Right) {
			xFix = -(w - x);
		}

		m_Prompt->SetResizeProportions(xFix, y, 0, h);
		m_Selection->SetResizeProportions(xFix, y, 0, h);
		m_BottomBar->SetResizeProportions(x, y, w, h);
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

		Point<int> diff = { newPosition.tX - m_Background->GetPosition().tX - 1, newPosition.tY - m_Background->GetPosition().tY - 1 };
		m_Background->SetPosition(newPosition);

		diff.tX += 1;
		diff.tY += 1;

		m_Text->ChangePosition(diff);
		m_DefaultText->ChangePosition(diff);
		m_Prompt->ChangePosition(diff);
		m_Selection->ChangePosition(diff);
		m_BottomBar->ChangePosition(diff);
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
		m_BottomBar->ChangePosition(positionChange);
	}

	void SimpleInput::SetVisibility(bool mode)
	{
		if (mode == false) {
			m_Text->SetVisibility(mode);
			m_DefaultText->SetVisibility(mode);
		}
		else {
			if (!m_DefaultText->GetText().empty() && m_Text->GetText().empty()) {
				m_Text->SetVisibility(false);
				m_DefaultText->SetVisibility(true);
			}
			else {
				m_Text->SetVisibility(true);
				m_DefaultText->SetVisibility(false);
			}
		}
		m_Background->SetVisibility(mode);
		m_Prompt->SetVisibility(false);
		m_Selection->SetVisibility(mode);
		m_BottomBar->SetVisibility(mode);
		m_Widget2DProps.bVisibility = mode;

	}

	bool SimpleInput::GetVisibility() const
	{
		return m_Widget2DProps.bVisibility;
	}

	void SimpleInput::SetDefaultTextColor(Color newColor)
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


	void SimpleInput::SetCallback(Callback function)
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
		return m_State;
	}

	void SimpleInput::SetTextMargin(Point<int> newMargins)
	{
		m_InpTheme.Margins = newMargins;
	}

	const Point<int>& SimpleInput::GetTextMargin() const
	{
		return m_InpTheme.Margins;
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
		m_BottomBar->EnableWidget();
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
		m_BottomBar->DisableWidget();
	}

	void SimpleInput::Render()
	{
		m_Background->Render();
		m_Selection->Render();
		m_Text->Render();
		m_DefaultText->Render();
		m_Prompt->Render();
		m_BottomBar->Render();
	}

	void SimpleInput::CalcAnchorOffsets()
	{
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			m_AnchorOffsets.tX = 0;
		}
		else if (m_Widget2DProps.iFlags & Anchor::Right) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth);
		}
		else if (m_Widget2DProps.iFlags & Anchor::HorizontalCenter) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth / 2);// 0;
		}

		if (m_Widget2DProps.iFlags & Anchor::Bottom) {
			m_AnchorOffsets.tY = 0;
		}
		else if (m_Widget2DProps.iFlags & Anchor::Top) {
			m_AnchorOffsets.tY = -((int)m_Widget2DProps.Dimensions.tHeight);
		}
		else if (m_Widget2DProps.iFlags & Anchor::VerticalCenter) {
			m_AnchorOffsets.tY = -(int)(m_Widget2DProps.Dimensions.tHeight) / 2;  //0
		}

		m_Text->SetPosition(Point<int>({
			(int)(m_Widget2DProps.Position.tX + m_InpTheme.Margins.tX + m_AnchorOffsets.tX),
			(int)(m_Widget2DProps.Position.tY + m_InpTheme.Margins.tY + m_AnchorOffsets.tY)
			}));

		m_DefaultText->SetPosition(Point<int>({
			(int)(m_Widget2DProps.Position.tX + m_InpTheme.Margins.tX + m_AnchorOffsets.tX),
			(int)(m_Widget2DProps.Position.tY + m_InpTheme.Margins.tY + m_AnchorOffsets.tY)
			}));
	}

	void SimpleInput::UpdateSelection() 
	{
		//Update selection graphics
		float selectStartPos = m_Text->GetPromptPosition(std::min(m_iDragStart, m_iDragEnd));
		float selectEndPos = m_Text->GetPromptPosition(std::max(m_iDragStart, m_iDragEnd));

		int newXPos = 0;
		size_t newWidth = 0;
		if (selectStartPos >= -m_Text->GetDisplayOffset().tX && selectEndPos <= -m_Text->GetDisplayOffset().tX + m_Widget2DProps.Dimensions.tWidth) {
			newXPos = (int)(m_Widget2DProps.Position.tX + (selectStartPos + m_Text->GetDisplayOffset().tX) + m_InpTheme.Margins.tX + m_AnchorOffsets.tX);
			newWidth = (size_t)(selectEndPos - selectStartPos);
		}
		else if (selectStartPos <= -m_Text->GetDisplayOffset().tX) {
			newXPos = m_Widget2DProps.Position.tX + m_InpTheme.Margins.tX + m_AnchorOffsets.tX;
			newWidth = (size_t)(selectEndPos + m_Text->GetDisplayOffset().tX);
		}
		if (newWidth > m_Widget2DProps.Dimensions.tWidth - m_InpTheme.Margins.tX * 2) newWidth = m_Widget2DProps.Dimensions.tWidth - m_InpTheme.Margins.tX * 2;

		if (newXPos == 0 && newWidth == 0) {
			newXPos = m_Prompt->GetPosition().tX;// -m_Widget2DProps.Position.tX;
			int diff = newXPos - m_Widget2DProps.Position.tX - m_InpTheme.Margins.tX;
			newWidth = (m_Widget2DProps.Dimensions.tWidth - m_InpTheme.Margins.tX * 2) - diff + m_AnchorOffsets.tX;
		}

		m_Selection->SetPosition({ newXPos, m_Widget2DProps.Position.tY + m_InpTheme.Margins.tY + m_AnchorOffsets.tY });
		m_Selection->Resize({ newWidth, (size_t)(m_Widget2DProps.Dimensions.tHeight - m_InpTheme.Margins.tY * 2) });
		m_Selection->SetVisibility(true);
	}

	//Events
	bool SimpleInput::OnWindowTick(Event* ev)
	{
		++m_iTicks;
		if (m_State == WidgetMouseState::Press && m_iTicks % 120 == 0 && GetVisibility() == true) {
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
			}
			else {
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
			if (m_Callback != nullptr) m_Callback(ev);
			return true;
		}
		return false;
	}


	bool SimpleInput::OnNewKey(Event* ev)
	{
		if (GetVisibility() == false) return false;
		if (m_State == WidgetMouseState::Press) {

			KbKeyAppearEvent* kie = (KbKeyAppearEvent*)ev;
			int k = kie->GetKey();
			if (k == 67 && kie->GetState() > 0 && kie->GetMods() == 2) { //Ctrl+C
				int start = std::min(m_iDragStart, m_iDragEnd);
				int	length = std::max(m_iDragEnd, m_iDragStart) - std::min(m_iDragEnd, m_iDragStart);
				if (length <= 0) return true;
				std::wstring tempWStr = m_Text->GetText().substr(start, length);
				std::string temp = utf8_encode(tempWStr);
				glfwSetClipboardString(m_ParentWindow->GetTopWindow()->GetGLFWwindow(), temp.c_str());
			}
			else if (k == 86 && kie->GetState() > 0 && kie->GetMods() == 2) {	//Ctrl+V
				auto data = glfwGetClipboardString(m_ParentWindow->GetTopWindow()->GetGLFWwindow());
				if (data == nullptr) return true;
				std::string toPaste(data);
				std::wstring temp = utf8_decode(toPaste);

				if (m_iDragStart == m_iDragEnd) {				//Just paste					
					std::wstring curText = m_Text->GetText();
					curText.insert(m_iPromptIdx, temp);
					m_Text->SetText(curText);
					m_iPromptIdx += (int)temp.size();
					SetPromptPos(m_iPromptIdx);
				}
				else {	//Paste with erase
					std::wstring curText = m_Text->GetText();

					//Erase
					int charStart = std::min(m_iDragStart, m_iDragEnd);
					int charLength = std::max(m_iDragEnd, m_iDragStart) - std::min(m_iDragEnd, m_iDragStart);
					curText.erase(charStart, charLength);
					//Insert
					curText.insert(charStart, temp);
					m_Text->SetText(curText);
					//Update
					m_iPromptIdx = charStart + (int)temp.size();

					SetPromptPos(m_iPromptIdx);

					//Disable m_Selection
					m_Selection->SetVisibility(false);
					m_iDragStart = m_iDragEnd = -1;

				}

			}
			else if (k == 88 && kie->GetState() > 0 && kie->GetMods() == 2) {	//Ctrl+X
				//Copy
				int start = std::min(m_iDragStart, m_iDragEnd);
				int	length = std::max(m_iDragEnd, m_iDragStart) - std::min(m_iDragEnd, m_iDragStart);
				if (length <= 0) return true;
				std::wstring tempWStr = m_Text->GetText().substr(start, length);
				std::string temp = utf8_encode(tempWStr);
				glfwSetClipboardString(m_ParentWindow->GetTopWindow()->GetGLFWwindow(), temp.c_str());

				//Erase
				std::wstring curText = m_Text->GetText();
				curText.erase(start, length);
				m_Text->SetText(curText);
				//Update
				m_iPromptIdx = start;

				SetPromptPos(m_iPromptIdx);

				//Disable m_Selection
				m_Selection->SetVisibility(false);
				m_iDragStart = m_iDragEnd = -1;
			}
			else if (k == 65 && kie->GetState() > 0 && kie->GetMods() == 2) {	//Ctrl+A
				m_iDragEnd = (int)m_Text->GetTextRef().size();
				m_iDragStart = 0;
				UpdateSelection();
			}
			else if (k == 259 && kie->GetState() > 0) { //Backspace

				if (std::wstring textTemp = m_Text->GetText(); m_iPromptIdx <= (int)textTemp.size() && m_iPromptIdx >= 0) {
					//No selection
					if (m_iDragStart == m_iDragEnd) {
						if (m_iPromptIdx == 0) return true;
						textTemp.erase(m_iPromptIdx - 1, 1);
						m_Text->SetText(textTemp);
						SetPromptPos(m_iPromptIdx - 1);
					}
					else {
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

			}
			else if (k == 263 && kie->GetState() > 0) { //Left
			 //Update prompt & offsets
				if (m_iPromptIdx - 1 >= 0) {
					SetPromptPos(m_iPromptIdx - 1);
				}

				m_Selection->SetVisibility(false);
				m_iDragEnd = m_iDragStart = -1;
			}
			else if (k == 262 && kie->GetState() > 0) { //Right
			 //Update prompt & offsets
				if (m_iPromptIdx + 1 <= (int)m_Text->GetText().size()) {
					SetPromptPos(m_iPromptIdx + 1);
				}

				m_Selection->SetVisibility(false);
				m_iDragEnd = m_iDragStart = -1;
			}

			//Enter state update
			if (k == 257 && kie->GetState() > 0) {
				m_Enter = true;
			}
			else {
				m_Enter = false;
			}

			if (m_Callback != nullptr) m_Callback(ev);
			return true;
		}
		return false;
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
				m_Background->SetColor(m_InpTheme.Base[WidgetMouseState::Hover].Tint);
				m_Background->SetBorderColor(m_InpTheme.Base[WidgetMouseState::Hover].BorderColor);
				m_State = WidgetMouseState::Hover;
			}
			bReturn = true;
		}
		else {
			if (m_State == WidgetMouseState::Hover) {
				m_Background->SetColor(m_InpTheme.Base[WidgetMouseState::Normal].Tint);
				m_Background->SetBorderColor(m_InpTheme.Base[WidgetMouseState::Normal].BorderColor);
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
				if (tempIdx + 1 > textSize) tempIdx = textSize;
				//Do the magic
				SetPromptPos(tempIdx);
				m_iDragEnd = m_iPromptIdx;
			}
			else if (pos.tX <= m_Widget2DProps.Position.tX) {
				--tempIdx;
				if (tempIdx < 0) tempIdx = 0;
				SetPromptPos(tempIdx);
				m_iDragEnd = m_iPromptIdx;
			}
			else {
				std::tie(m_iPromptIdx, m_fPromptPos) = m_Text->GetPromptPosition({ pos.tX + -1 * (int)fOffset - m_Widget2DProps.Position.tX - m_InpTheme.Margins.tX, pos.tY + m_InpTheme.Margins.tY });
				SetPromptPos(m_iPromptIdx);
				m_iDragEnd = m_iPromptIdx;
			}

			//Update selection graphics
			UpdateSelection();

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
		if (absPromptPos >= absOffset && absPromptPos <= absOffset + width) {
			m_Prompt->SetPosition({ absPromptPos - absOffset + m_Widget2DProps.Position.tX + m_InpTheme.Margins.tX + m_AnchorOffsets.tX, m_Widget2DProps.Position.tY + m_InpTheme.Margins.tY + m_AnchorOffsets.tY });
		}
		else if (absPromptPos < absOffset) {
			//Position outside widget (to the left)
			m_Prompt->SetPosition({ m_Widget2DProps.Position.tX + m_InpTheme.Margins.tX + m_AnchorOffsets.tX, m_Widget2DProps.Position.tY + m_InpTheme.Margins.tY + m_AnchorOffsets.tY });
			m_Text->SetDisplayOffset({ -(float)absPromptPos, 0 });
		}
		else if (absPromptPos > absOffset + width) {
			//Position outside widget (to the right)
			m_Prompt->SetPosition({ (int)m_Widget2DProps.Position.tX + (int)m_Widget2DProps.Dimensions.tWidth - m_InpTheme.Margins.tX + m_AnchorOffsets.tX, m_Widget2DProps.Position.tY + m_InpTheme.Margins.tY + m_AnchorOffsets.tY });
			m_Text->SetDisplayOffset({ -(float)absPromptPos + width, (float)0 });
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
				std::tie(m_iPromptIdx, m_fPromptPos) = m_Text->GetPromptPosition({ pos.tX + -1 * (int)fOffset - m_Widget2DProps.Position.tX - m_InpTheme.Margins.tX, pos.tY + m_InpTheme.Margins.tY });
				SetPromptPos(m_iPromptIdx); //remove that if rollback

				//Update main graphics & state
				if (m_State != WidgetMouseState::Press) {
					m_Background->SetColor(m_InpTheme.Base[WidgetMouseState::Press].Tint);
					m_Background->SetBorderColor(m_InpTheme.Base[WidgetMouseState::Press].BorderColor);
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
			}
			else {
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
			m_Background->SetColor(m_InpTheme.Base[WidgetMouseState::Normal].Tint);
			m_Background->SetBorderColor(m_InpTheme.Base[WidgetMouseState::Normal].BorderColor);
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

	bool CustomFilter(KbCharAppearEvent* ev, std::string customChars)
	{
		if(customChars.find(ev->GetCodePoint()) != std::string::npos) return true;
		return false;
	}

	//------------------------------------------------------------------------

	VScroller::VScroller(
		Window* window,
		const Widget2DProps& scrollerProps,
		VScrollerTheme theme,
		AreaWidget* area
	) :
		Widget2D(window, scrollerProps),
		m_Theme(theme),
		m_Callback(nullptr),
		m_AnchorOffsets{ 0,0 },
		m_iContentHeight(0),
		m_SmartXPlace(0),
		m_Rows(0)
	{
		CalculateAnchorFix();
		m_Background = new Background(window, scrollerProps, { m_Theme.Base.Tint, m_Theme.Base.BorderColor, m_Theme.BaseBorderSize });
		m_Button = new SimpleButton<Background>(
			window,
			{ {scrollerProps.Position.tX, scrollerProps.Position.tY+ m_iAnchorFix}, {scrollerProps.Dimensions.tWidth, 0}, Anchor::Left | Anchor::Bottom },
			{ m_Theme.Slider[0], m_Theme.Slider[1], m_Theme.Slider[2], m_Theme.SliderBorderSize}
		);
		m_Button->SetBypassEventReturn(true);
		m_iBtnSave = scrollerProps.Position;
		m_DragData[2] = { 0,0 };
		m_iDisplayLine = (int)m_Widget2DProps.Dimensions.tHeight;

		ConnectArea(area);

		EnableWidget();
		CalcAnchorOffsets();
	}

	VScroller::VScroller(Window* window, const Widget2DProps& scrollerProps, Theme theme, AreaWidget* area):
		VScroller(window, scrollerProps, theme.VScroller, area)
	{
	}

	VScroller::VScroller(const VScroller& copy):
		Widget2D(copy)
	{
		m_Theme = copy.m_Theme;
		m_AnchorOffsets = copy.m_AnchorOffsets;
		m_iContentHeight = copy.m_iContentHeight;
		m_SmartXPlace = copy.m_SmartXPlace;
		m_Rows = copy.m_Rows;
		m_iBtnSave = copy.m_iBtnSave;
		m_iDisplayLine = copy.m_iDisplayLine;
		m_iDisplayLineBackup = copy.m_iDisplayLineBackup;
		memcpy(m_DragData, copy.m_DragData, 3);
		m_InsertOffs = copy.m_InsertOffs;
		m_iAnchorFix = copy.m_iAnchorFix;

		m_Area = nullptr;
		m_Callback = nullptr;

		m_Background = new Background(*copy.m_Background);
		m_Button = new SimpleButton<Background>(*copy.m_Button);

		EnableWidget();
		CalcAnchorOffsets();
	}


	VScroller::~VScroller()
	{
		DisableWidget();
		delete m_Background;
		delete m_Button;
	}

	VScroller& VScroller::operator=(VScroller rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	void swap(VScroller& first, VScroller& second) noexcept
	{
		using std::swap;
		swap(first.m_Theme, second.m_Theme);
		swap(first.m_AnchorOffsets, second.m_AnchorOffsets);
		swap(first.m_iContentHeight, second.m_iContentHeight);
		swap(first.m_SmartXPlace, second.m_SmartXPlace);
		swap(first.m_Rows, second.m_Rows);
		swap(first.m_iBtnSave, second.m_iBtnSave);
		swap(first.m_iDisplayLine, second.m_iDisplayLine);
		swap(first.m_iDisplayLineBackup, second.m_iDisplayLineBackup);
		swap(first.m_InsertOffs, second.m_InsertOffs);
		swap(first.m_iAnchorFix, second.m_iAnchorFix);
		std::swap_ranges(first.m_DragData, first.m_DragData + 3, second.m_DragData);

		swap(first.m_Background, second.m_Background);
		swap(first.m_Button, second.m_Button);
	
		swap(static_cast<Widget2D&>(first), static_cast<Widget2D&>(second));
	}

	VScroller* VScroller::Clone() const
	{
		return new VScroller(*this);
	}

	void VScroller::CalculateAnchorFix()
	{
		m_iAnchorFix = 0;
		if (m_Widget2DProps.iFlags & Anchor::Top)
		{
			m_iAnchorFix = -(int)m_Widget2DProps.Dimensions.tHeight;
		}
		else if (m_Widget2DProps.iFlags & Anchor::VerticalCenter)
		{
			m_iAnchorFix = -(int)m_Widget2DProps.Dimensions.tHeight/2;
		}
	}

	void VScroller::ConnectArea(AreaWidget* area)
	{
		m_Area = area;
	}

	void VScroller::DisconnectArea()
	{
		m_Area = nullptr;
	}


	void VScroller::SetCallback(Callback function)
	{
		m_Callback = function;
	}

	void VScroller::ResetCallback()
	{
		m_Callback = nullptr;
	}

	void  VScroller::SetFlags(int newFlags)
	{
		Widget2D::SetFlags(newFlags);
		m_Background->SetFlags(newFlags);
		m_Button->SetFlags(newFlags);
		CalcAnchorOffsets();
		UpdateScrollerDim();
	}

	void VScroller::CalcAnchorOffsets()
	{
		if (m_Widget2DProps.iFlags & Anchor::Left) {
			m_AnchorOffsets.tX = 0;
		}
		else if (m_Widget2DProps.iFlags & Anchor::Right) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth);
		}
		else if (m_Widget2DProps.iFlags & Anchor::HorizontalCenter) {
			m_AnchorOffsets.tX = -(int)(m_Widget2DProps.Dimensions.tWidth / 2);
		}

		if (m_Widget2DProps.iFlags & Anchor::Bottom) {
			m_AnchorOffsets.tY = 0;
		}
		else if (m_Widget2DProps.iFlags & Anchor::Top) {
			m_AnchorOffsets.tY = -((int)m_Widget2DProps.Dimensions.tHeight);
		}
		else if (m_Widget2DProps.iFlags & Anchor::VerticalCenter) {
			m_AnchorOffsets.tY = -(int)(m_Widget2DProps.Dimensions.tHeight) / 2;
		}
	}

	void VScroller::SetResizeProportions(int x, int y, int w, int h)
	{
		m_Background->SetResizeProportions(x, y, 0, h);
		//m_Button->SetResizeProportions(0, 0, 0, -h);
		Resizable2D::SetResizeProportions(x, y, 0, h);
	}

	void VScroller::SetPosition(Point<int> newPosition)
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

	void VScroller::ChangePosition(Point<int> positionChange)
	{
		m_Widget2DProps.Position += positionChange;
		m_ResizableProps.BasePosition += positionChange;
		m_iBtnSave += positionChange;
		m_Background->ChangePosition(positionChange);
		m_Button->ChangePosition(positionChange);
	}

	void VScroller::SetVisibility(bool mode)
	{
		m_Background->SetVisibility(mode);
		m_Button->SetVisibility(mode);
		m_Widget2DProps.bVisibility = mode;
		for (auto widget : m_ConnectedWidgets) {
			widget.first->SetVisibility(mode);
		}
	}

	bool VScroller::GetVisibility() const
	{
		return m_Widget2DProps.bVisibility;
	}


	void VScroller::EnableWidget()
	{
		ANGE_NULLPTR_TEST(m_ParentWindow, "m_ParentWindow == nullptr!");
		ANGE_ZERO_TEST(m_ParentWindow->IfOpen(), "m_ParentWindow == 0! (window is closed)");

		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseMove, I_BIND(VScroller, OnMouseMove)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseEnter, I_BIND(VScroller, OnMouseEnter)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseClick, I_BIND(VScroller, OnMouseClick)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::MouseScroll, I_BIND(VScroller, OnMouseScroll)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowResize, I_BIND(VScroller, OnWindowResize)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::WindowClose, I_BIND(VScroller, OnWindowClose)));
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::DrawableInvokeRender, I_BIND(VScroller, OnDrawableInvokeRender)));

		m_Background->EnableWidget();
		m_Button->EnableWidget();

		m_Button->UnregisterEvent(EventType::DrawableInvokeRender);
		m_Background->UnregisterEvent(EventType::DrawableInvokeRender);
	}

	void VScroller::DisableWidget()
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


	void VScroller::Render()
	{
		if (m_Area == nullptr) return;
		m_Background->Render();
		m_Button->Render();
	}


	void  VScroller::SetForegroundColor(WidgetMouseState forState, Color color)
	{
		m_Theme.Slider[forState].Tint = color;
		m_Button->SetColor(forState, m_Theme.Slider[forState].Tint);
	}

	void  VScroller::SetForegroundColors(Color normal, Color hover, Color press)
	{
		m_Theme.Slider[WidgetMouseState::Normal].Tint = normal;
		m_Theme.Slider[WidgetMouseState::Hover].Tint = hover;
		m_Theme.Slider[WidgetMouseState::Press].Tint = press;
		m_Button->SetColor(normal, hover, press);
	}

	void  VScroller::SetBackgroundColor(Color color)
	{
		m_Theme.Base.Tint = color;
		m_Background->SetColor(color);
	}

	const Color VScroller::GetForegroundColor(WidgetMouseState forState) const
	{
		return m_Theme.Slider[forState].Tint;
	}

	const Color VScroller::GetBackgroundColor() const
	{
		return m_Theme.Base.Tint;
	}

	//Update scroller btn 
	void VScroller::UpdateScrollerDim() {
		if (m_Area == nullptr) return;
		if (m_iContentHeight > (int)m_Area->m_Widget2DProps.Dimensions.tHeight) {
			size_t oldHeight = m_Button->GetDimension().tHeight;
			float contentRatio = (float)(m_iContentHeight) / m_Widget2DProps.Dimensions.tHeight;// (int)m_Area.Dimensions.tHeight;
			size_t newHeight = (size_t)((float)1 / contentRatio * m_Widget2DProps.Dimensions.tHeight);// m_Area.Dimensions.tHeight;
			m_Button->Resize({ (size_t)m_Button->GetDimension().tWidth, newHeight });
		}
		else {
			m_Button->Resize({ (size_t)m_Button->GetDimension().tWidth, 0 });
		}
		CalculateAnchorFix();
		//if button is invisible, make bg also invisible
		if (m_Widget2DProps.iFlags & ScrollerFlags::AutoDisable) {
			if ((int)m_Button->GetDimension().tHeight <= 0)
			{
				m_Background->SetVisibility(false);
			}
			else {
				m_Background->SetVisibility(true);
			}
		}
	}

	float VScroller::GetOffset()
	{
		int maxYPos = (int)m_Widget2DProps.Dimensions.tHeight - (int)m_Button->GetDimension().tHeight;
		int base = (int)m_Widget2DProps.Position.tY + m_iAnchorFix;
		int real = m_Button->GetPosition().tY - base;
		return (float)real / maxYPos;
	}

	void VScroller::SetOffset(float offset) {

		if (offset > 1.0f) offset = 1.0f;
		if (offset < 0.0f) offset = 0.0f;

		//calculate real offset, & update elements
		float baseYPosition = m_iContentHeight * offset;

		//calculate new scroller position
		int maxYPos = (int)m_Widget2DProps.Dimensions.tHeight - (int)m_Button->GetDimension().tHeight;
		int newYPosRel = (int)(offset * maxYPos);
		m_Button->SetPosition({ m_iBtnSave.tX + m_AnchorOffsets.tX, (int)(newYPosRel + m_iBtnSave.tY + m_iAnchorFix) });

		//Shift content
		float contentOffset = (1.0f - offset)*(m_iContentHeight - m_Widget2DProps.Dimensions.tHeight);
		for (auto widget : m_ConnectedWidgets) {
			widget.first->SetPosition({ widget.second.tX, widget.second.tY + (int)contentOffset });
		}
	}

	void VScroller::Clear()
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

	void VScroller::SetInsertOffsets(Point<int> offset)
	{
		m_InsertOffs = offset;
		RecalculatePositions(0);
	}

	void VScroller::RecalculatePositions(size_t f)
	{
		if (m_Area == nullptr) return;

		//Clear data
		m_DragData[2] = { 0,0 };
		m_iContentHeight = 0;
		m_SmartXPlace = 0;
		m_Rows = 0;
		
		m_iDisplayLine -= (int)f;

		int applyYOffset = m_InsertOffs.tY;

		if (m_Widget2DProps.iFlags & ScrollerFlags::SmartPlacement) {
			for (ConnectedWidget& widget : m_ConnectedWidgets) {
				Point<int> pushPos = { 0,0 };

				//Check if we are in maximum width range (of scroller area)
				if (m_SmartXPlace + (int)widget.first->GetDimension().tWidth > (int)m_Area->m_Widget2DProps.Dimensions.tWidth) {
					//Perform adding to new row
					++m_Rows;
					m_iContentHeight += (int)widget.first->GetDimension().tHeight + applyYOffset;


					//Fix x position
					pushPos.tX = m_Area->m_Widget2DProps.Position.tX;
					m_SmartXPlace = 0;

					//Fix y position
					pushPos.tY = m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight - m_iContentHeight + (int)widget.first->GetDimension().tHeight;

				}
				else {
					//Perform adding to the same row
					pushPos.tX = m_Area->m_Widget2DProps.Position.tX + m_SmartXPlace;
					if (m_Rows == 0) {
						m_iContentHeight = (int)widget.first->GetDimension().tHeight;
						pushPos.tY = m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight;
					}
					else {
						pushPos.tY = m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight - m_iContentHeight + (int)widget.first->GetDimension().tHeight;
					}

				}

				//Inc m_SmartXPlace
				m_SmartXPlace += (int)widget.first->GetDimension().tWidth + m_InsertOffs.tX;

				//Fix anchor for content
				auto fix = CalculateAreaAnchor();
				pushPos += fix;

				//Add widget to scroller
				widget.first->SetPosition(pushPos);
				widget.second = { pushPos.tX, pushPos.tY };
			}
		}
		else {
			for (ConnectedWidget& widget : m_ConnectedWidgets) {
				Point<int> pushPos = { 0,0 };

				//Perform standard adding (create next row)
				pushPos = { m_Area->m_Widget2DProps.Position.tX, -m_iContentHeight + m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight };
				m_iContentHeight += (int)widget.first->GetDimension().tHeight + applyYOffset;

				//Fix anchor for content
				auto fix = CalculateAreaAnchor();
				pushPos += fix;

				//Add widget to scroller
				widget.first->SetPosition(pushPos);
				widget.second = { pushPos.tX, pushPos.tY };
			}
		}

		//if (m_iContentHeight > (int)m_Area->m_Widget2DProps.Dimensions.tHeight) {
			UpdateScrollerDim();
			int max = (m_iContentHeight - (int)m_Widget2DProps.Dimensions.tHeight);
			int curLine = (m_iDisplayLine - (int)m_Widget2DProps.Dimensions.tHeight);
			SetOffset(1.0f - ((float)curLine / max));
		//}
	}

	void VScroller::PushBack(Widget2D* widget)
	{
		widget->SetVisibility(false);
		if (m_Area == nullptr) return;
		widget->SetVisibility(true);

		Point<int> pushPos = { 0,0 };

		//Get y offset value
		int applyYOffset = m_InsertOffs.tY;

		//Perform smart adding
		if (m_Widget2DProps.iFlags & ScrollerFlags::SmartPlacement) {

			if (!m_ConnectedWidgets.empty() && m_ConnectedWidgets.back().first->GetDimension().tHeight != widget->GetDimension().tHeight) {
				ANGE_WARNING("VScroller row for SmartPlacement should have a constant height. PushBack() abroted.");
				return;
			}

			//Check if we are in maximum width range (of scroller area)
			if (m_SmartXPlace + (int)widget->GetDimension().tWidth > (int)m_Area->m_Widget2DProps.Dimensions.tWidth) {
				//Perform adding to new row
				++m_Rows;
				m_iContentHeight += (int)widget->GetDimension().tHeight + applyYOffset;


				//Fix x position
				pushPos.tX = m_Area->m_Widget2DProps.Position.tX;
				m_SmartXPlace = 0;

				//Fix y position
				pushPos.tY = m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight - m_iContentHeight + (int)widget->GetDimension().tHeight;

			}
			else {
				//Perform adding to the same row
				pushPos.tX = m_Area->m_Widget2DProps.Position.tX + m_SmartXPlace;
				if (m_Rows == 0) {
					m_iContentHeight = (int)widget->GetDimension().tHeight;
					pushPos.tY = m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight;
				}
				else {
					pushPos.tY = m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight - m_iContentHeight + (int)widget->GetDimension().tHeight;
				}

			}

			//Inc m_SmartXPlace
			m_SmartXPlace += (int)widget->GetDimension().tWidth + m_InsertOffs.tX;

		}
		else {
			//Perform standard adding (create next row)
			pushPos = { m_Area->m_Widget2DProps.Position.tX, -m_iContentHeight + m_Area->m_Widget2DProps.Position.tY + (int)m_Area->m_Widget2DProps.Dimensions.tHeight };
			m_iContentHeight += (int)widget->GetDimension().tHeight + applyYOffset;
		}

		//Fix anchor for content
		auto fix = CalculateAreaAnchor();
		pushPos += fix;

		//Add widget to scroller
		widget->SetPosition(pushPos);
		widget->SetFlags(Anchor::Left | Anchor::Top);

		widget->UnregisterEvent(EventType::DrawableInvokeRender);
		widget->UnregisterEvent(EventType::WindowResize);

		std::cout << "Pushed: " << pushPos.ToString() << std::endl;

		m_ConnectedWidgets.push_back(std::pair<Widget2D*, Point<int>>(widget, pushPos)); //+ (int)widget->GetDimension().tHeight

		if (m_iContentHeight > (int)m_Area->m_Widget2DProps.Dimensions.tHeight) {
			UpdateScrollerDim();
			int max = (m_iContentHeight - (int)m_Widget2DProps.Dimensions.tHeight);
			int curLine = (m_iDisplayLine - (int)m_Widget2DProps.Dimensions.tHeight);
			SetOffset(1.0f - ((float)curLine / max));
		}

	}

	Point<int> VScroller::CalculateAreaAnchor()
	{
		int yFix = 0;
		int xFix = 0;
		if (m_Area->m_Widget2DProps.iFlags & Anchor::Top) {
			xFix -= (int)m_Area->m_Widget2DProps.Dimensions.tHeight;
		}
		else if (m_Area->m_Widget2DProps.iFlags & Anchor::VerticalCenter) {
			yFix -= (int)m_Area->m_Widget2DProps.Dimensions.tHeight / 2;
		}
		if (m_Area->m_Widget2DProps.iFlags & Anchor::Right) {
			xFix -= (int)m_Area->m_Widget2DProps.Dimensions.tWidth;
		}
		else if (m_Area->m_Widget2DProps.iFlags & Anchor::HorizontalCenter) {
			xFix -= (int)m_Area->m_Widget2DProps.Dimensions.tWidth / 2;
		}
		return Point<int>{xFix, yFix};
	}

	bool VScroller::OnDrawableInvokeRender(Event* ev)
	{	
		if (m_Area == nullptr) return false;
		Render();
		GLint scissorBackup[4];
		glGetIntegerv(GL_SCISSOR_BOX, &scissorBackup[0]);

		//Calculate scissor fix
		auto fix = CalculateAreaAnchor();

		glScissor(m_Area->m_Widget2DProps.Position.tX+fix.tX, m_Area->m_Widget2DProps.Position.tY+fix.tY, (GLsizei)m_Area->m_Widget2DProps.Dimensions.tWidth, (GLsizei)m_Area->m_Widget2DProps.Dimensions.tHeight);
		for (auto w : m_ConnectedWidgets) {
			w.first->Render();
		}
		glScissor(scissorBackup[0], scissorBackup[1], scissorBackup[2], scissorBackup[3]);

		return false;
	}

	bool VScroller::OnMouseMove(Event* ev)
	{

		Point<int> pos2 = m_Button->GetPosition();
		Dimension<size_t> dim = m_Button->GetDimension();

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
			}
			else if (offset + m_Button->GetDimension().tHeight > m_Widget2DProps.Position.tY + m_Widget2DProps.Dimensions.tHeight + m_AnchorOffsets.tY) {
				offset = m_Widget2DProps.Position.tY + (int)m_Widget2DProps.Dimensions.tHeight - (int)m_Button->GetDimension().tHeight + +m_AnchorOffsets.tY;
			}

			int maximum = (int)m_Widget2DProps.Dimensions.tHeight - (int)m_Button->GetDimension().tHeight;
			int current = offset - (m_Widget2DProps.Position.tY + m_AnchorOffsets.tY);
			float ratio = 1.0f - (float)current / maximum;
			int contentOffset = (int)(ratio * (m_iContentHeight - m_Widget2DProps.Dimensions.tHeight));

			//Update
			m_Button->SetPosition({ m_iBtnSave.tX + m_AnchorOffsets.tX, offset + m_iAnchorFix });
			m_iDisplayLine = contentOffset + (int)m_Widget2DProps.Dimensions.tHeight;
			SetOffset(1.0f - ratio);	
		}

		if (CheckCoords(pos)) {
			if (m_Callback != nullptr) return m_Callback(ev);
			return true;
		}

		return false;
	}

	bool VScroller::OnMouseClick(Event* ev)
	{
		if (GetVisibility() == false) return false;
		MouseClickEvent* mce = (MouseClickEvent*)ev;
		Point<int> pos = mce->GetPosition() - m_AnchorOffsets;
		if (CheckCoords(pos)) {
			if (mce->GetAction() == 1) {
				m_DragData[0] = pos;
				m_DragData[2] = m_Button->GetPosition();
				m_iDisplayLineBackup = m_iDisplayLine;
			}
			if (m_Callback != nullptr) return m_Callback(ev);
			return true;
		}
		return false;
	}

	bool VScroller::OnWindowResize(Event* ev)
	{

		m_Area->OnWindowResize(ev);

		Dimension<size_t> diff = m_Widget2DProps.Dimensions;
		WindowResizeEvent* wre = (WindowResizeEvent*)ev;
		if (m_Widget2DProps.iFlags & ResizePolicy::AutoFill) {
			m_Widget2DProps.Position = Point<int>({ 0, 0 });
			m_Widget2DProps.Dimensions = wre->GetDimension();
		}
		else {
			CalcResizeData(wre->GetDimension(), m_Widget2DProps.Dimensions, m_Widget2DProps.Position);
		}
		diff = diff - m_Widget2DProps.Dimensions;

		//Update
		m_iBtnSave = m_Widget2DProps.Position;

		CalcAnchorOffsets();
		m_Widget2DProps.bIfChanged = true;

		RecalculatePositions(diff.tHeight);

		return false;
	}

	bool VScroller::OnMouseScroll(Event* ev)
	{
		MouseScrollEvent* mse = (MouseScrollEvent*)ev;
		if (m_Button->GetDimension().tHeight == 0) return false;

		auto pos = mse->GetPosition() - m_AnchorOffsets;
		auto pos2 = mse->GetPosition() - CalculateAreaAnchor();

		if ( CheckCoords(pos) || m_Area->CheckCoords(pos2)) {

			Point<double> offsets = mse->GetScrollOffsets();
			m_iDisplayLine = m_iDisplayLine - 24 * (int)offsets.tY;
			if (m_iDisplayLine < (int)m_Widget2DProps.Dimensions.tHeight) m_iDisplayLine = (int)m_Widget2DProps.Dimensions.tHeight;
			if (m_iDisplayLine > m_iContentHeight) m_iDisplayLine = m_iContentHeight;

			int max = (m_iContentHeight - (int)m_Widget2DProps.Dimensions.tHeight);
			int curLine = (m_iDisplayLine - (int)m_Widget2DProps.Dimensions.tHeight);
			SetOffset(1.0f - ((float)curLine / max));
			if (m_Callback != nullptr) return m_Callback(ev);
			return true;
		}
		return false;
	}


	bool VScroller::OnMouseEnter(Event* ev)
	{
		return false;
	}

	bool VScroller::OnWindowClose(Event* ev)
	{
		DisableWidget();
		return false;
	}

	//------------------------------------------------------------------------------------------------------------------


	CustomWidget::CustomWidget(Window* window, const Widget2DProps& props) :
		Widget2D(window, props)
	{
		m_WidgetType = WidgetType::Custom;
		m_LastInsertionPos = -1;
	}

	CustomWidget::CustomWidget(const CustomWidget& copy):
		Widget2D(copy)
	{
		//Plain data copy
		m_LastInsertionPos = copy.m_LastInsertionPos;
		
		//Recreate stored objects
		std::list<std::pair<Widget2D*, Widget2D*>> internalWindows; //First old, second new
		std::list<Widget2D*> connectedToInternalWindow;

		for (auto part : copy.m_Components) {
			Widget2D* widget = part.second->Clone();
			m_Components.insert(std::pair<int, Widget2D*>(part.first, widget));
		
			//Update internal windows list
			if (widget->GetWidgetType() == WidgetType::Window){
				internalWindows.push_back(std::pair<Widget2D*, Widget2D*>(part.second, widget));
			}

			//Update connection list
			if (widget->GetParentWindow() != m_ParentWindow){
				connectedToInternalWindow.push_back(widget);
			}
		}

		//Fix connections
		for (auto toConnect : connectedToInternalWindow)
		{
			Widget2D* trueParent = toConnect->GetParentWindow();
			for (auto newWindow : internalWindows)
			{
				if (trueParent == newWindow.first)
				{
					//Connect widget to that window
					toConnect->ChangeParent((Window*)newWindow.second);
				}
			}
		}
	}


	CustomWidget::~CustomWidget()
	{
		for (auto it : m_Components)
		{
			delete it.second;
		}
		m_Components.clear();
	}

	CustomWidget& CustomWidget::operator=(CustomWidget rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	void swap(CustomWidget& first, CustomWidget& second) noexcept
	{
		using std::swap;
		swap(first.m_LastInsertionPos, second.m_LastInsertionPos);
		swap(first.m_Components, second.m_Components);
		swap(static_cast<Widget2D&>(first), static_cast<Widget2D&>(second));
	}

	CustomWidget* CustomWidget::Clone() const
	{
		return new CustomWidget(*this);
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
			std::cout << it.first << std::endl;
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
			}
			else if (newFlags & Anchor::Right) {
				position.tX = m_Widget2DProps.Position.tX + ((int)m_Widget2DProps.Dimensions.tWidth);
			}
			else if (newFlags & Anchor::HorizontalCenter) {
				position.tX = m_Widget2DProps.Position.tX + (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			}
		}
		else if (oldFlags & Anchor::Right) {
			if (newFlags & Anchor::Left) {
				position.tX = m_Widget2DProps.Position.tX - ((int)m_Widget2DProps.Dimensions.tWidth);
			}
			else if (newFlags & Anchor::Right) {
				position.tX = m_Widget2DProps.Position.tX;
			}
			else if (newFlags & Anchor::HorizontalCenter) {
				position.tX = m_Widget2DProps.Position.tX - (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			}
		}
		else if (oldFlags & Anchor::HorizontalCenter) {
			if (newFlags & Anchor::Left) {
				position.tX = m_Widget2DProps.Position.tX - (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			}
			else if (newFlags & Anchor::Right) {
				position.tX = m_Widget2DProps.Position.tX + (int)(m_Widget2DProps.Dimensions.tWidth) / 2;
			}
			else if (newFlags & Anchor::HorizontalCenter) {
				position.tX = m_Widget2DProps.Position.tX;
			}
		}

		if (oldFlags & Anchor::Bottom) {
			if (newFlags & Anchor::Bottom) {
				position.tY = m_Widget2DProps.Position.tY;
			}
			else if (newFlags & Anchor::Top) {
				position.tY = m_Widget2DProps.Position.tY + ((int)m_Widget2DProps.Dimensions.tHeight);
			}
			else if (newFlags & Anchor::VerticalCenter) {
				position.tY = m_Widget2DProps.Position.tY + (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			}
		}
		else if (oldFlags & Anchor::Top) {
			if (newFlags & Anchor::Bottom) {
				position.tY = m_Widget2DProps.Position.tY - ((int)m_Widget2DProps.Dimensions.tHeight);
			}
			else if (newFlags & Anchor::Top) {
				position.tY = m_Widget2DProps.Position.tY;
			}
			else if (newFlags & Anchor::VerticalCenter) {
				position.tY = m_Widget2DProps.Position.tY - (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			}
		}
		else if (oldFlags & Anchor::VerticalCenter) {
			if (newFlags & Anchor::Bottom) {
				position.tY = m_Widget2DProps.Position.tY - (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			}
			else if (newFlags & Anchor::Top) {
				position.tY = m_Widget2DProps.Position.tY + (int)(m_Widget2DProps.Dimensions.tHeight) / 2;
			}
			else if (newFlags & Anchor::VerticalCenter) {
				position.tY = m_Widget2DProps.Position.tY;
			}
		}
	}

	void CustomWidget::SetFlags(int flags)
	{
		for (auto it : m_Components) {
			//Set flags without changing anchor
			int filteredFlags = (flags  & AnchorMask) | (it.second->GetFlags() & AnchorBits);
			it.second->SetFlags(filteredFlags);

			//Recalculate position if there is anchor change
			Point<int> position = it.second->GetPosition();
			TranslateAnchor(position,flags, it.second->GetFlags());
			it.second->SetPosition(position);
		}
	}


	void  CustomWidget::SetResizeProportions(int x, int y, int w, int h)
	{
		for (auto it : m_Components) {
			it.second->SetResizeProportions(x, y, 0, 0);
		}
	}

	Widget2D* CustomWidget::GetComponent(int idx)
	{
		if (auto it = m_Components.find(idx); it != m_Components.end()) {
			return it->second;
		}
		return nullptr;
	}

	bool CustomWidget::AddComponent(int idx, Widget2D* widget)
	{
		if (widget != nullptr) {
			auto ret = m_Components.insert(std::pair<int, Widget2D*>(idx, widget));
			if(ret.second == true) m_LastInsertionPos = idx;
			return ret.second;
		}
		return false;
	}

	int CustomWidget::AddComponent(Widget2D* widget)
	{
		if (widget != nullptr) {
			auto ret = m_Components.insert(std::pair<int, Widget2D*>(m_LastInsertionPos+1, widget));
			if (ret.second == true) m_LastInsertionPos += 1;
			return m_LastInsertionPos;
		}
		return std::numeric_limits<int>::min();
	}

	size_t CustomWidget::ComponentAmount()
	{
		return m_Components.size();
	}

	//------------------------------------------------------------------------------------------------------------------


	ProgressBar::ProgressBar(Window* window, Widget2DProps props, ProgressBarTheme theme, std::wstring defText, float maxValue) :
		CustomWidget(window, props)
	{
		m_fMaxValue = maxValue;
		m_fObservedValue = nullptr;
		m_wsBaseText = defText;

		AddComponent(PG_BACKGROUND, new Background(window, props, theme.Base));

		auto pro = props;
		TranslateAnchor(pro.Position, pro.iFlags, Anchor::Left | Anchor::Bottom);
		pro.Position += {(int)theme.Base.BorderSize.tWidth, (int)theme.Base.BorderSize.tHeight};
		pro.Dimensions = { 0, props.Dimensions.tHeight - theme.Base.BorderSize.tHeight * 2 };

		AddComponent(PG_FILL, new Background(window, { pro.Position, pro.Dimensions, Anchor::Left | Anchor::Bottom }, theme.Fill));

		if (theme.TextTh.UsedFont != nullptr)
		{
			props = m_Widget2DProps;
			Point<int> poss = props.Position;
			TranslateAnchor(props.Position, props.iFlags, Anchor::VerticalCenter | Anchor::HorizontalCenter);
			props.iFlags = Anchor::VerticalCenter || Anchor::HorizontalCenter;

			AddComponent(PG_TEXT, new Text(window, props, theme.TextTh));
		}

		EnableWidget();
	}

	ProgressBar::ProgressBar(Window* window, Widget2DProps props, Theme theme, std::wstring defText, float maxValue) :
		ProgressBar(window, props, theme.ProgressBar, defText, maxValue)
	{
	}

	ProgressBar::ProgressBar(const ProgressBar& copy) :
		CustomWidget(copy)
	{
		m_Callback = nullptr;
		m_fObservedValue = nullptr;
		m_wsBaseText = copy.m_wsBaseText;
		m_fMaxValue = copy.m_fMaxValue;
	}

	ProgressBar& ProgressBar::operator=(ProgressBar rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	void swap(ProgressBar& first, ProgressBar& second) noexcept
	{
		using std::swap;
		swap(first.m_wsBaseText, second.m_wsBaseText);
		swap(first.m_fMaxValue, second.m_fMaxValue);
		swap(static_cast<CustomWidget&>(first), static_cast<CustomWidget&>(second));
	}

	ProgressBar* ProgressBar::Clone()
	{
		return new ProgressBar(*this);
	}

	void ProgressBar::EnableWidget()
	{
		m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::Tick, I_BIND(ProgressBar, OnWindowTick)));
		CustomWidget::EnableWidget();
	}

	bool ProgressBar::OnWindowTick(Event* ev)
	{
		if (m_Widget2DProps.iFlags & AutoUpdate) Update();
		return false;
	}

	void ProgressBar::SetToObserve(float* toObserve)
	{
		m_fObservedValue = toObserve;
		Update();
	}

	void ProgressBar::SetValue(float newValue)
	{
		if (m_fObservedValue != nullptr) {
			*m_fObservedValue = newValue;
			Update();
		}
	}

	void ProgressBar::Update()
	{
		Background* m_Bg = ((Background*)GetComponent(PG_BACKGROUND));
		Background* m_FillBg = ((Background*)GetComponent(PG_FILL));
		Text* m_Info = ((Text*)GetComponent(PG_TEXT));
		float value = 0.0f;
		if (m_fObservedValue != nullptr) value = *m_fObservedValue;

		//Calculate stuff
		int max = m_Widget2DProps.Dimensions.tWidth - m_Bg->GetBorderSize().tWidth * 2;
		float ratio = (value / m_fMaxValue);
		if (ratio < 0.0f) ratio = 0.0f;
		if (ratio > 1.0f) ratio = 1.0f;
		int width = ratio * max;

		//Update UI
		m_FillBg->Resize({ (size_t)width, m_FillBg->GetDimension().tHeight });
		if (m_Widget2DProps.iFlags & PrecentageInfo && m_Info != nullptr) {
			std::wstring updatedText = m_wsBaseText + std::to_wstring((int)(ratio * 100)) + L"%";
			m_Info->SetText(updatedText);
		}

		if (m_Callback != nullptr) {
			if (m_Widget2DProps.iFlags & InvokeCallback || (m_Widget2DProps.iFlags & InvokeCallbackOnDone && ratio == 1.0f)) {
				ProgressBarUpdateEvent* pbue = new ProgressBarUpdateEvent(ratio);
				m_Callback(pbue);
				delete pbue;
			}
		}
	}

	void ProgressBar::SetCallback(Callback cbFunc)
	{
		m_Callback = cbFunc;
	}

	void ProgressBar::ResetCallback()
	{
		m_Callback = nullptr;
	}

	void ProgressBar::ChangeFillColor(Color newColor)
	{
		((Text*)GetComponent(PG_TEXT))->SetColor(newColor);
	}


	//--------------------------------------------------------------------------------------------------------------------


	BasicItem::BasicItem(Window* window, Widget2DProps props, ContextMenu* cm) :
		CustomWidget(window, props)
	{
		m_ParentWidget = cm;
	}

	BasicItem::~BasicItem()
	{
	}

	ContextMenu* BasicItem::GetParentWidget()
	{
		return m_ParentWidget;
	}




	ContextMenuItem::ContextMenuItem(Window* window, Widget2DProps props, ContextMenu* cm, SimpleButtonTheme btnTheme) :
		BasicItem(window, props, cm)
	{
		auto btn = new SimpleButton<Background>(window, props, btnTheme);
		btn->SetCallback([this](Event* ev)->bool {
			if (ev->GetEventType() == EventType::MouseClick)
			{
				MouseClickEvent* mce = (MouseClickEvent*)ev;
				if (mce->GetButton() == 0 && mce->GetAction() == 0) {
					this->GetParentWidget()->DisableWidget();
					if(m_Callback != nullptr) m_Callback(ev);
					((SimpleButton<Background>*)GetComponent(0))->SetState(WidgetMouseState::Normal);
				}
			}
			return true;
		});

		AddComponent(0, btn);
	}

	void ContextMenuItem::SetText(TextTheme theme, std::wstring text)
	{
		if (Widget2D* check = GetComponent(1); check != nullptr) {
			delete check;
			m_Components.erase(1);
		}

		auto dim = m_Widget2DProps.Dimensions - Dimension<size_t>{28, 0};
		Point<int> pos = m_Widget2DProps.Position + Point<int>{28, (int)m_Widget2DProps.Dimensions.tHeight / 2};

		Text* textWid = new Text(
			m_ParentWindow,
			{ pos, dim, Anchor::Left | Anchor::VerticalCenter },
			theme,
			text
		);
		AddComponent(1, textWid);
	}

	void ContextMenuItem::SetImage(ImageTheme theme, Texture* texture)
	{
		if (Widget2D* check = GetComponent(2); check != nullptr) {
			delete check;
			m_Components.erase(2);
		}

		if (texture == nullptr) return;

		auto dim = Dimension<size_t>(
			{ (size_t)(m_Widget2DProps.Dimensions.tHeight*0.85f),
			(size_t)(m_Widget2DProps.Dimensions.tHeight*0.85f) }
		);

		auto pos = m_Widget2DProps.Position + Point<int>{5, (int)m_Widget2DProps.Dimensions.tHeight / 2};
		Image* img = new Image(
			m_ParentWindow,
			{ pos, dim, Anchor::Left | Anchor::VerticalCenter },
			theme,
			texture
		);
		AddComponent(2, img);
	}

	void ContextMenuItem::SetCallback(Callback cbFunc)
	{
		m_Callback = cbFunc;
	}

	void ContextMenuItem::ResetCallback()
	{
		m_Callback = nullptr;
	}

	Callback ContextMenuItem::GetCallbackFunc()
	{
		return m_Callback;
	}



	DividerItem::DividerItem(Window* window, Widget2DProps props, ContextMenu* cm, Color tint) :
		BasicItem(window, props, cm)
	{
		auto divBg = new Background(window, props, {tint, {0,0,0,0}, {0,0}});
		AddComponent(0, divBg);
	}


	ContextMenu::ContextMenu(Window* window, Dimension<size_t> dimension, ContextMenuTheme theme) :
		CustomWidget(window, { {0,0}, dimension })
	{
		m_Theme = theme;
		m_TotalHeight = 0;
		m_ResizableProps.BaseDimension = dimension;
		SetFlags(Anchor::Left | Anchor::Bottom);
		SetupButton();
	}

	ContextMenu::~ContextMenu()
	{
	}

	ContextMenuItem* ContextMenu::AddItem(std::wstring text, Texture * texture)
	{

		//Add bg if not present yet
		if (ComponentAmount() == 1) {
			auto bg = new Background(m_ParentWindow, m_Widget2DProps, m_Theme.Base);
			AddComponent(0, bg);
		}

		//Add item
		auto pos = m_Widget2DProps.Position;
		TranslateAnchor(pos, m_Widget2DProps.iFlags, Anchor::Left | Anchor::Bottom);
		pos += Point<int>({ 1, -m_TotalHeight - m_Theme.iRow + 1 });
		std::cout << pos.ToString() << std::endl;
		auto bi = new ContextMenuItem(
			m_ParentWindow,
			{ pos, {m_Widget2DProps.Dimensions.tWidth - 2, (size_t)m_Theme.iRow - 2}, Anchor::Left | Anchor::Bottom },
			this,
			m_Theme.Item
		);

		bi->SetText(m_Theme.Item.TextTh, text);
		if (texture != nullptr) bi->SetImage(m_Theme.Img, texture);
		AddComponent(bi);
		
		m_TotalHeight += m_Theme.iRow;
		Resize(m_Widget2DProps.Dimensions + Dimension<size_t>{0, (size_t)m_Theme.iRow}, m_Theme.iRow);
		
		return bi;
	}

	void ContextMenu::AddDivider(Color dividerColor)
	{
		//Add bg if not present yet
		if (ComponentAmount() == 1) AddComponent(0, new Background(m_ParentWindow, m_Widget2DProps, m_Theme.Base));
		
		//Add item
		auto pos = m_Widget2DProps.Position;
		TranslateAnchor(pos, m_Widget2DProps.iFlags, Anchor::Left | Anchor::Bottom);
		pos += Point<int>({ 1 + 28, -m_TotalHeight - 1 });

		auto divider = new DividerItem(
			m_ParentWindow,
			{ pos, {m_Widget2DProps.Dimensions.tWidth - 2 - 28 - 4, 1}, Anchor::Left | Anchor::Bottom },
			this,
			dividerColor
		);

		AddComponent(divider);
		m_TotalHeight += 1;
		Resize(m_Widget2DProps.Dimensions + Dimension<size_t>{0, 1}, 1);
	}

	void ContextMenu::SetPosition(Point<int> newPos)
	{
		Dimension<size_t> dim = m_ParentWindow->GetPhysicalWindowDim();
		if (newPos.tX + (int)m_Widget2DProps.Dimensions.tWidth > (int)dim.tWidth) {
			newPos.tX = newPos.tX - (int)m_Widget2DProps.Dimensions.tWidth;
			if (newPos.tX < 0) ANGE_WARNING("[ContextMenu] Cannot fit menu into the window boundaries.");
		}
		if (newPos.tY - (int)m_Widget2DProps.Dimensions.tHeight < 0) {
			newPos.tY = newPos.tY + (int)m_Widget2DProps.Dimensions.tHeight;
			if (newPos.tY > (int)m_Widget2DProps.Dimensions.tHeight) ANGE_WARNING("[ContextMenu] Cannot fit menu into the window boundaries.");
		}
		CustomWidget::SetPosition(newPos);
		m_Button->SetPosition({0, 0});
	}

	void ContextMenu::Resize(Dimension<size_t> newSize, int heightShift)
	{
		m_ResizableProps.BaseDimension = newSize;
		m_Widget2DProps.Dimensions = newSize;
		auto bg = (Background*)(GetComponent(0));
		bg->ChangePosition({ 0, -heightShift });
		bg->Resize(newSize);
		m_Widget2DProps.bIfChanged = true;
	}

	void ContextMenu::SetupButton()
	{
		//Create global button
		Window* topWindow = m_ParentWindow->GetTopWindow();
		Dimension<size_t> dim = m_ParentWindow->GetDimension();
		m_Button = new SimpleButton<Background>(
			topWindow,
			Widget2DProps({ {0,0}, dim, Anchor::Left | Anchor::Bottom }),
			SimpleButtonTheme(Color(0, 0, 0, 0), Color(0, 0, 0, 0), Color(0, 0, 0, 0), { 0,0 }, { 0, {0,0,0,0}, nullptr })
		);
		m_Button->DisableWidget();
		m_Button->SetResizeProportions(0, 0, 100, 100);
		m_Button->SetBypassEventReturn(true);
		m_Button->SetCallback([this](Event*ev) {
			if (ev->GetEventType() == EventType::MouseClick) {
				
				MouseClickEvent* mce = (MouseClickEvent*)ev;
				auto pos = mce->GetPosition();
				
				if (pos.tX < m_Widget2DProps.Position.tX || pos.tX > m_Widget2DProps.Position.tX + m_Widget2DProps.Dimensions.tWidth || 
					pos.tY > m_Widget2DProps.Position.tY || pos.tY < m_Widget2DProps.Position.tY - m_Widget2DProps.Dimensions.tHeight) {

					this->DisableWidget();
					return true;
				}
			}
			return false;
		});
		AddComponent(-1, m_Button);
	}
	



}
