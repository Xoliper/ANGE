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
		void SetColor(WidgetMouseState forState, Color& color);
		void SetColor(Color& normal, Color& hover, Color& press);
		void SetBorderColor(WidgetMouseState forState, Color& color);
		void SetBorderColor(Color& normal, Color& hover, Color& press);
		void SetBoderSize(Dimension<int> newBorderSize);
		void SetFontSize(int newSize);
		void SetText(std::wstring newText);
		void SetFont(Font* newFont);
		void SetFontColor(Color& newColor);
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
		void SetCallback(std::function<bool(Event*)> function);
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
		std::function<bool(Event*)> m_Callback;
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
		void SetColor(WidgetMouseState forState, Color& color);
		void SetColor(Color& normal, Color& hover, Color& press);
		void SetBorderColor(WidgetMouseState forState, Color& color);
		void SetBorderColor(Color& normal, Color& hover, Color& press);
		void SetBoderSize(Dimension<int> newBorderSize);
		void SetFontSize(int newSize);
		void SetText(std::wstring newText);
		void SetFont(Font* newFont);
		void SetFontColor(Color& newColor);
		void SetFlags(int newFlags) override;
		void SetTextMargin(Point<int> newMargins);
		void SetPromptPos(int pos);
		void SetDefaultTextColor(Color& newColor);
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
		void SetCallback(std::function<bool(Event*)> function);
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
		std::list<BindListIterator> m_Bindings;
		std::function<bool(Event*)> m_Callback;
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
		void SetForegroundColor(WidgetMouseState forState, Color& color);
		void SetForegroundColors(Color& normal, Color& hover, Color& press);
		void SetBackgroundColor(Color& color);
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

		void SetCallback(std::function<bool(Event*)> function);
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
		std::function<bool(Event*)> m_Callback;

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

		std::map<int, Widget2D*> m_Components;
	};

	//---------------------------------------------------------------------
	

	class BasicItem : public CustomWidget
	{
	public:
		BasicItem(Window* window, const Widget2DProps& props, BackgroundProps bg) :
			CustomWidget(window, props)
		{
			auto btn = new SimpleButton(window, props, bg);
			if (bg.BaseColor.GetBrightness() > 128){
				btn->SetColor(WidgetMouseState::Hover, bg.BaseColor - Color(64, 64, 64, 0));
			} else {
				btn->SetColor(WidgetMouseState::Hover, bg.BaseColor + Color(64, 64, 64, 0));
			}
			AddComponent(0, btn);
		}


		void SetText(TextProps props)
		{
			if (Widget2D* check = GetComponent(1); check != nullptr) delete check;

			auto dim = m_Widget2DProps.Dimensions - Dimension<size_t>{34, 0};
			Point<int> pos = m_Widget2DProps.Position + Point<int>{34, (int)m_Widget2DProps.Dimensions.tHeight / 2};

			Text* text = new Text(
				m_ParentWindow,
				{ pos, dim, Anchor::Left | Anchor::VerticalCenter },
				props
			);
			AddComponent(1, text);
		}
		
		void SetImage(ImageProps props)
		{
			if (Widget2D* check = GetComponent(2); check != nullptr) delete check;
			
			auto dim = Dimension<size_t>(
				{(size_t)(m_Widget2DProps.Dimensions.tHeight*0.92f),
				(size_t)(m_Widget2DProps.Dimensions.tHeight*0.92f)}
			);

			auto pos = m_Widget2DProps.Position + Point<int>{2, (int)m_Widget2DProps.Dimensions.tHeight / 2};
			Image* img = new Image(
				m_ParentWindow,
				{ pos, dim, Anchor::Left | Anchor::VerticalCenter },
				props
			);
			AddComponent(2, img);
		}

		~BasicItem()
		{
			Cleanup();
		}

	private:

		void Cleanup()
		{
			for (auto it : m_Components)
			{
				delete it.second;
			}
		}
	};

	class ContextMenu : public CustomWidget
	{
	public:
		ContextMenu(Window* window, const Widget2DProps& props, BackgroundProps bgTheme, Color rowBg, int rowHeight) :
			CustomWidget(window, props)
		{
			m_BgProps = bgTheme;
			m_RowHeight = rowHeight;
			m_RowBg = rowBg;
			AddComponent(0, new Background(window, m_Widget2DProps, m_BgProps));
		}

		~ContextMenu()
		{
			for (auto it : m_Components)
			{
				delete it.second;
			}
		}

		void AddItem(TextProps textProps, ImageProps imageProps = ImageProps())
		{
			auto pos = m_Widget2DProps.Position;
			TranslateAnchor(pos, m_Widget2DProps.iFlags, Anchor::Left | Anchor::Bottom);
			pos += Point<int>({ 1, -(int)m_Items.size()*m_RowHeight - m_RowHeight + 1 });
			auto bi = new BasicItem(
				m_ParentWindow,
				{pos, {m_Widget2DProps.Dimensions.tWidth-2, (size_t)m_RowHeight-2}, Anchor::Left | Anchor::Bottom },
				{ m_RowBg, {0,0,0,0}, {0,0} }
			);
			bi->SetText(textProps);
			if(imageProps.ImageTexture != nullptr) bi->SetImage(imageProps);
			m_Items.push_back(bi);
			Resize(m_Widget2DProps.Dimensions + Dimension<size_t>{0,(size_t)m_RowHeight});
			
		}

	private:

		void Resize(Dimension<size_t> newSize)
		{
			m_ResizableProps.BaseDimension = newSize;
			m_Widget2DProps.Dimensions = newSize;
			auto bg = (Background*)(GetComponent(0));
			bg->Resize(newSize);
			bg->ChangePosition({ 0, -m_RowHeight });
			m_Widget2DProps.bIfChanged = true;
		}


		std::list<BasicItem*> m_Items;
		BackgroundProps m_BgProps;
		Color m_RowBg;
		int m_RowHeight;
	};






	class Menu : public CustomWidget
	{
	public:
		Menu();
		~Menu();

		void AddItem();

	private:

	};

}
