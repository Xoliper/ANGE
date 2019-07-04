#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Modules/Window.h"
#include "Ange/Core/Modules/Graphics/Widgets.h"
#include "Ange/Core/Modules/Graphics/BasicWidgets.h"
#include "Ange/Core/Modules/Graphics/Texture.h"
#include "Ange/Core/Modules/Graphics/Font.h"
#include "Ange/Core/Event.h"

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
			const BackgroundProps& rectProps = BackgroundProps(),
			const TextProps& textProps = TextProps()
		);
		SimpleButton(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const ImageProps& spriteProps = ImageProps(),
			const TextProps& textProps = TextProps()
		);
		~SimpleButton();

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
		Background* m_Background;
		Image* m_Image;
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
		~SimpleInput();

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
		~VerticalScroller();

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
		CustomWidget();
		CustomWidget(Window* window, const Widget2DProps& props);
		CustomWidget(const CustomWidget& copy);
		~CustomWidget();

		Widget2D* GetComponent(int idx)
		{
			if (auto it = m_Components.find(idx); it != m_Components.end()) {
				return it->second;
			}
			return nullptr;
		}

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

		std::map<int, Widget2D*> m_Components;
	};

}
