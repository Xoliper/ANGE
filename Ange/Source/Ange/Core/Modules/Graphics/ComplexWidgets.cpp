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
		m_bBypassEventsReturn(false)
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
		swap(first.m_State, second.m_State);
		//wap(first.m_Callback, second.m_Callback);
		swap(*(first.m_FrontWidget), *(second.m_FrontWidget));
		swap(*(first.m_Text), *(second.m_Text)); //Hmmmm, should i rly swap this if position is swapped? 
		swap(static_cast<Widget2D&>(first), static_cast<Widget2D&>(second));
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
		if (m_bDrag == true) return false; //TODO: Check this later.
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
		m_BottomBar = new Background(window, { {props.Position.tX, props.Position.tY-fixBarY}, {props.Dimensions.tWidth, (size_t)theme.iBarHeight}, (props.iFlags & 0b11100000) | Anchor::Bottom }, { theme.Bar, {0,0,0,0}, {0,0} });
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
	std::string utf8_encode(const std::wstring& wstr) { return ""; }
	std::wstring utf8_decode(const std::string& str) { return L""; }
#endif


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
			m_iDisplayLine = m_iDisplayLine - 24 * offsets.tY;
			if (m_iDisplayLine < m_Widget2DProps.Dimensions.tHeight) m_iDisplayLine = m_Widget2DProps.Dimensions.tHeight;
			if (m_iDisplayLine > m_iContentHeight) m_iDisplayLine = m_iContentHeight;

			int max = (m_iContentHeight - (int)m_Widget2DProps.Dimensions.tHeight);
			int curLine = (m_iDisplayLine - (int)m_Widget2DProps.Dimensions.tHeight);
			SetOffset(1.0f - ((float)curLine / max));
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




}
