#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Modules/Window.h"
#include "Ange/Core/Modules/Graphics/Widgets.h"
#include "Ange/Core/Modules/Graphics/BasicWidgets.h"
#include "Ange/Core/Modules/Graphics/Texture.h"
#include "Ange/Core/Modules/Graphics/Font.h"
#include "Ange/Core/Event.h"
#include "Ange/Core/Task.h"

namespace Ange {

	//-----------------------------------------------------------------------
	//Enums
	//-----------------------------------------------------------------------


	enum WidgetMouseState
	{
		Normal = 0,
		Hover = 1,
		Press = 2
	};

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	class SimpleButton : public Widget2D
	{
		friend class ContextMenuItem;
	public:
		SimpleButton(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const BackgroundProps& backgroundProps = BackgroundProps(),
			const TextProps& textProps = TextProps()
		);
		SimpleButton(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const ImageProps& imageProps = ImageProps(),
			const TextProps& textProps = TextProps()
		);
		virtual ~SimpleButton();

		//Setters
		void SetColor(WidgetMouseState forState, Color color);
		void SetColor(Color normal, Color hover, Color press);
		void SetBorderColor(WidgetMouseState forState, Color color);
		void SetBorderColor(Color normal, Color hover, Color press);
		void SetBoderSize(Dimension<int> newBorderSize);
		void SetFontSize(int newSize);
		void SetText(std::wstring newText);
		void SetFont(Font* newFont);
		void SetFontColor(Color newColor);
		void SetFlags(int newFlags) override;
		void Resize(Dimension<size_t> newDim);
		void SetBypassEventReturn(bool mode);

		void UnregisterEvent(EventType eventType) override;

		//Getters
		const Color GetColor(WidgetMouseState forState) const;
		const Color GetBorderColor(WidgetMouseState forState) const;
		const Dimension<int> GetBoderSize() const;
		const int GetFontSize() const;
		std::wstring GetText() const;
		Color GetFontColor() const;
		Font* GetUsedFont() const;
		const Image* GetImage() const;
		const Background* GetBackground() const;

		//Managing button
		void SetCallback(Callback function);
		void ResetCallback();
		WidgetMouseState GetState();
		bool GetDragInfo();

		//Derived
		void SetResizeProportions(int x, int y, int w, int h) override;
		void SetPosition(Point<int> newPosition) override;
		void ChangePosition(Point<int> positionChange) override;
		void SetVisibility(bool mode) override;
		bool GetVisibility() const override;

		void EnableWidget() override;	//Should bind and pass data to m_Bindings
		void DisableWidget() override;	//Should unbind m_InternalBinding from m_ParentWindow
		void Render() override;

	private:
		//Events handlers
		bool OnMouseMove(Event* ev);
		bool OnMouseClick(Event* ev);
		bool OnMouseEnter(Event* ev);
		bool OnWindowResize(Event* ev);
		bool OnWindowClose(Event* ev);
		bool OnWindowTick(Event* ev);

		//Helper functions
		void CalcAnchorOffsets();

		//Vars
		int m_iTicks;
		bool m_bDrag;
		bool m_bBypassEventsReturn;
		Point<int> m_AnchorOffsets;
		WidgetMouseState m_State;
		FrontWidget m_FrontWidget;
		Text* m_Text;
		Color m_BgColors[3];
		Color m_BorderColors[3];
		std::list<BindListIterator> m_Bindings;
		Callback m_Callback;
	};

	//------------------------------------------------------------------

	std::string utf8_encode(const std::wstring &wstr);
	std::wstring utf8_decode(const std::string &str);

	class SimpleInput : public Widget2D
	{
	public:
		SimpleInput(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const BackgroundProps& rectProps = BackgroundProps(),
			const TextProps& defaultTextProps = TextProps(),
			const TextProps& textProps = TextProps()
		);
		virtual ~SimpleInput();

		//Setters
		void SetColor(WidgetMouseState forState, Color color);
		void SetColor(Color normal, Color hover, Color press);
		void SetBorderColor(WidgetMouseState forState, Color color);
		void SetBorderColor(Color normal, Color hover, Color press);
		void SetBoderSize(Dimension<int> newBorderSize);
		void SetFontSize(int newSize);
		void SetText(std::wstring newText);
		void SetFont(Font* newFont);
		void SetFontColor(Color newColor);
		void SetFlags(int newFlags) override;
		void SetTextMargin(Point<int> newMargins);
		void SetPromptPos(int pos);
		void SetDefaultTextColor(Color newColor);
		void SetDefaultText(std::wstring newDefaultText);

		void UnregisterEvent(EventType eventType) override;

		//Getters
		const Color GetColor(WidgetMouseState forState) const;
		const Color GetBorderColor(WidgetMouseState forState) const;
		const Dimension<int>& GetBoderSize() const;
		const int GetFontSize() const;
		std::wstring GetText() const;
		Color GetFontColor() const;
		Font* GetUsedFont() const;
		const Point<int>& GetTextMargin() const;
		int GetPromptPos() const;
		Color GetDefaultTextColor() const;
		std::wstring GetDefaultText() const;
		const std::wstring& GetTextRef() const;
		
		std::wstring GetTexWhenEnter();


		//Managing input
		void SetCallback(Callback function);
		void ResetCallback();

		void SetFilter(std::function<bool(KbCharAppearEvent*)> function);
		void ResetFilter();

		WidgetMouseState GetState();

		//Helper functions
		void CalcAnchorOffsets();

		//Derived
		void SetResizeProportions(int x, int y, int w, int h) override;

		void SetPosition(Point<int> newPosition) override;
		void ChangePosition(Point<int> positionChange) override;
		void SetVisibility(bool mode) override;
		bool GetVisibility() const override;

		void EnableWidget() override;	//Should bind and pass data to m_Bindings
		void DisableWidget() override;	//Should unbind m_InternalBinding from m_ParentWindow
		void Render() override;

	private:
		//Events handlers
		bool OnMouseMove(Event* ev);
		bool OnMouseClick(Event* ev);
		bool OnMouseEnter(Event* ev);
		bool OnWindowResize(Event* ev);
		bool OnWindowClose(Event* ev);
		bool OnWindowTick(Event* ev);
		bool OnNewChar(Event* ev);
		bool OnNewKey(Event* ev);

		//Vars
		bool m_Enter;
		int m_iTicks;
		int m_iPromptIdx;
		float m_fPromptPos;
		bool m_bActive;
		bool m_bDrag;
		int m_iDragStart, m_iDragEnd;
		Point<int> m_AnchorOffsets;
		WidgetMouseState m_State;
		Background* m_Background;
		Rectangle2D* m_Prompt;
		Rectangle2D* m_Selection;
		Text* m_Text;
		Text* m_DefaultText;
		Color m_BgColors[3];
		Color m_BorderColors[3];
		Point<int> m_Margins;
		Callback m_Callback;
		std::function<bool(KbCharAppearEvent*)> m_FilterFunc;
	};


	bool IntNumericFilter(KbCharAppearEvent* ev);
	bool FloatNumericFilter(KbCharAppearEvent* ev);
	bool AlphabeticFilter(KbCharAppearEvent* ev);
	bool AlphaNumericFilter(KbCharAppearEvent* ev);


	//---------------------------------------------------------------------

	enum ScrollerFlags 
	{
		Default = 0,
		SmartPlacement = 1
	};

	using ConnectedWidget = std::pair< Widget2D*, Point<int> >;

	class VerticalScroller : public Widget2D
	{
	public:
		VerticalScroller(
			Window* window,
			const Widget2DProps& props,
			const Point<int>& scrollerAreaPos,
			const Dimension<size_t>& scrollerAreaDim, 
			const BackgroundProps& rectFgProps,
			const BackgroundProps& rectBgProps
		);
		virtual ~VerticalScroller();

		//Setters
		void SetForegroundColor(WidgetMouseState forState, Color color);
		void SetForegroundColors(Color normal, Color hover, Color press);
		void SetBackgroundColor(Color color);
		void SetFlags(int newFlags) override;

		//Getters
		const Color GetForegroundColor(WidgetMouseState forState) const;
		const Color GetBackgroundColor() const;

		//Helper functions
		void CalcAnchorOffsets();

		//Managing scroller
		void Clear();
		void PushBack(Widget2D* widget, int flags = 0);
		void SetInsertOffsets(Point<int> offset = {0,0});
		//void Insert(Widget2D* widget);
		//void PushFront(Widget2D* widget);
		//void PopBack();
		//void PopFront();
		//void Erase(int idx);

		void SetCallback(Callback function);
		void ResetCallback();

		/*!
		0.0f - 1.0f
		*/
		void SetOffset(float offset);

		//Derived
		void SetResizeProportions(int x, int y, int w, int h) override;

		void SetPosition(Point<int> newPosition) override;
		void ChangePosition(Point<int> positionChange) override;
		void SetVisibility(bool mode) override;
		bool GetVisibility() const override;

		void EnableWidget() override;
		void DisableWidget() override;
		void Render() override;

	private:
		//Events handlers
		bool OnMouseMove(Event* ev);
		bool OnMouseClick(Event* ev);
		bool OnMouseEnter(Event* ev);
		bool OnWindowResize(Event* ev);
		bool OnWindowClose(Event* ev);
		bool OnDrawableInvokeRender(Event* ev);

		void UpdateScrollerDim();

		//Adv pushing
		int m_SmartXPlace;
		int m_Rows;

		//Vars
		Point<int> m_InsertOffs;
		Point<int> m_AnchorOffsets;
		Background* m_Background;
		SimpleButton* m_Button;
		Color m_BgColor;
		Color m_FgColors[3];
		std::list<BindListIterator> m_Bindings;
		Callback m_Callback;

		Point<int> m_iBtnSave;
		Point<int> m_DragData[3]; //Drag start, Drag end, Button position
		Point<int> m_ScrAreaPos;
		Dimension<size_t> m_ScrAreaDim;
		std::list<ConnectedWidget> m_ConnectedWidgets;
		int m_iContentHeight;
		int m_iDisplayLine, m_iDisplayLineBackup;
	};


	//---------------------------------------------------------------------

	class CustomWidget : public Widget2D
	{
	public:
		CustomWidget(Window* window, const Widget2DProps& props);
		CustomWidget(const CustomWidget& copy);
		virtual ~CustomWidget();

		Widget2D* GetComponent(int idx);
		size_t ComponentAmount();

		//Derived
		void SetResizeProportions(int x, int y, int w, int h) override;
		void SetPosition(Point<int> newPosition) override;
		void ChangePosition(Point<int> positionChange) override;
		void SetVisibility(bool mode) override;
		bool GetVisibility() const override;

		void UnregisterEvent(EventType ev) override;
		void EnableWidget() override;
		void DisableWidget() override;
		void Render() override;
	
	protected:
		void TranslateAnchor(Point<int>& position, int oldFlags, int newFlags);
		void AddComponent(int idx, Widget2D* widget);
		int AddComponent(Widget2D* widget);

		std::map<int, Widget2D*> m_Components;
		int m_LastInsertionPos;
	};

	//---------------------------------------------------------------------
	
	class ContextMenu;

	class BasicItem : public CustomWidget
	{
	public:
		BasicItem(Window* window, Widget2DProps props, ContextMenu* cm);
		~BasicItem();

		ContextMenu* GetParentWidget();

	private:

		void Cleanup();

		ContextMenu* m_ParentWidget;
	};

	//---------------------------------------------------------------------

	class ContextMenuItem : public BasicItem
	{
	public:
		ContextMenuItem(Window* window, Widget2DProps props, ContextMenu* cm, BackgroundProps bg);
	
		void SetText(TextProps props);

		void SetImage(ImageProps props);

		void SetCallback(Callback cbFunc);
		void ResetCallback();
		Callback GetCallbackFunc();

	private:
		Callback m_Callback;
	};

	//---------------------------------------------------------------------

	class DividerItem : public BasicItem
	{
	public:

		DividerItem(Window* window, Widget2DProps props, ContextMenu* cm, BackgroundProps bg);
	};

	//---------------------------------------------------------------------

	class ContextMenu : public CustomWidget
	{
	public:
		ContextMenu(Window* window, Dimension<size_t> dimension, BackgroundProps bgTheme, Color rowBg, int rowHeight);

		~ContextMenu();

		void Clean();

		ContextMenuItem* AddItem(TextProps textProps, ImageProps imageProps = ImageProps());

		void AddDivider(Color dividerColor);

		void SetPosition(Point<int> newPos) override;

	private:

		void Resize(Dimension<size_t> newSize, int heightShift);

		BackgroundProps m_BgProps;
		Color m_RowBg;
		int m_RowHeight;
		int m_TotalHeight;
	};


	//---------------------------------------------------------------------

	class MenuItem : public SimpleButton
	{
	public:
		MenuItem(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const BackgroundProps& backgroundProps = BackgroundProps(),
			const TextProps& textProps = TextProps()
		) :
			SimpleButton(window, props, backgroundProps, textProps)
		{
		
		};
		virtual ~MenuItem(){}

	};


	class AppMenu : public CustomWidget
	{
	public:
		AppMenu(Window* window, BackgroundProps buttonTheme, TextProps textProps, int height, int contextMenuWidth = 120):
			CustomWidget(window, { {0,0}, {0,0}, Anchor::Left|Anchor::Bottom })
		{
			//Bar creation
			Point<int> pos = { -1, (int)window->GetDimension().tHeight - height };
			Dimension<size_t> dim = { window->GetDimension().tWidth + 2, (size_t)height+1};
			
			auto bg = new Background(window, { pos, dim, Anchor::Left | Anchor::Bottom }, buttonTheme);
			AddComponent(0, bg);
			SetResizeProportions(0, 100, 100, 0);

			//Allocate contextmenu
			m_Cm = new ContextMenu(
				window,
				{ (size_t)contextMenuWidth, 0 },
				buttonTheme,
				Color(255, 255, 255, 255),
				24
			);

			MenuItem* mi = new MenuItem(
				window,
				Widget2DProps(pos + Point<int>{0,1}, { 40, (size_t)height }, Anchor::Left | Anchor::Bottom),
				{ buttonTheme.BaseColor, {0,0,0,0}, {0,0} },
				textProps
			);
			mi->SetColor(WidgetMouseState::Hover, Color(255, 0, 0, 255));
			mi->SetResizeProportions(0, 100, 0, 0);
			AddComponent(mi);
		}

		~AppMenu()
		{
			delete m_Cm;
		}

		void SetResizeProportions(int x, int y, int w, int h) override
		{
			for (auto it : m_Components)
			{
				it.second->SetResizeProportions(x, y, w, h);
			}
			Widget2D::SetResizeProportions(x, y, w, h);
		}

	private:
		ContextMenu* m_Cm;
	};


	//----------------------------------------------------------

	enum ProgressBarFlags
	{
		AutoUpdate = 1 << 14,
		InvokeCallback = 1<< 15,
		InvokeCallbackOnDone = 1<< 16,
		PrecentageInfo = 1 << 17
	};

	class ProgressBar : CustomWidget
	{
	public:

		ProgressBar(Window* window, Widget2DProps props, BackgroundProps bgProps, Color fillColor, TextProps textProps, float maxValue) :
			CustomWidget(window, props)
		{
			m_fMaxValue = maxValue;
			m_fObservedValue = nullptr;
			m_wsBaseText = textProps.wsText;
			m_Bg = new Background(window, props, bgProps);
			props.Position += {(int)bgProps.BorderSize.tWidth, (int)bgProps.BorderSize.tHeight};
			props.Dimensions = { 0, props.Dimensions.tHeight - bgProps.BorderSize.tHeight*2 };
			m_FillBg = new Background(window, props, { fillColor, {0,0,0,0}, {0,0} });

			m_Info = nullptr;
			if (textProps.UsedFont != nullptr)
			{
				props = m_Widget2DProps;
				props.Position = {props.Position.tX+(int)props.Dimensions.tWidth/2, props.Position.tY+(int)props.Dimensions.tHeight/2};
				props.iFlags = Anchor::VerticalCenter || Anchor::HorizontalCenter;
				m_Info = new Text(window, props, textProps);
			}

			EnableWidget();
		}

		~ProgressBar()
		{
			for (auto it : m_Components)
			{
				delete it.second;
			}
			m_Components.clear();
		}

		void EnableWidget()
		{
			m_Bindings.push_back(m_ParentWindow->BindEvent(EventType::Tick, I_BIND(ProgressBar, OnWindowTick)));
			CustomWidget::EnableWidget();
		}

		bool OnWindowTick(Event* ev)
		{
			if(m_Widget2DProps.iFlags & AutoUpdate) Update();
			return false;
		}


		void SetToObserve(float* toObserve)
		{
			m_fObservedValue = toObserve;
			Update();
		}

		void SetValue(float newValue)
		{
			*m_fObservedValue = newValue;
			Update();
		}

		void Update()
		{
			//Calculate stuff
			int max = m_Widget2DProps.Dimensions.tWidth - m_Bg->GetBoderSize().tWidth*2;
			float ratio = (*m_fObservedValue / m_fMaxValue);
			if (ratio < 0.0f) ratio = 0.0f;
			if (ratio > 1.0f) ratio = 1.0f;
			int width = ratio*max;

			//Update UI
			m_FillBg->Resize({(size_t)width, m_FillBg->GetDimension().tHeight});
			if (m_Widget2DProps.iFlags & PrecentageInfo && m_Info != nullptr){
				std::wstring updatedText = m_wsBaseText + std::to_wstring((int)(ratio * 100)) + L"%";
				m_Info->SetText(updatedText);
			}

			if (m_Callback != nullptr) {
				if (m_Widget2DProps.iFlags & InvokeCallback || (m_Widget2DProps.iFlags & InvokeCallbackOnDone && ratio == 1.0f) ) {
					ProgressBarUpdateEvent* pbue = new ProgressBarUpdateEvent(ratio);
					m_Callback(pbue);
					delete pbue;
				}
			}
		}

		void SetCallback(Callback cbFunc)
		{
			m_Callback = cbFunc;
		}
		
		void ResetCallback()
		{
			m_Callback = nullptr;
		}

	private:
		Background* m_Bg;
		Background* m_FillBg;
		Text* m_Info;
		Callback m_Callback;
		std::wstring m_wsBaseText;
		float* m_fObservedValue;
		float m_fMaxValue;
	};
}
