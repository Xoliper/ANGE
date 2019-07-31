#pragma once

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Window.h"
#include "Ange/Core/Widgets.h"
#include "Ange/Core/BasicWidgets.h"
#include "Ange/Core/Texture.h"
#include "Ange/Core/Font.h"
#include "Ange/Core/Event.h"
#include "Ange/Core/Task.h"

namespace Ange {

	//-----------------------------------------------------------------------
	//Enums
	//-----------------------------------------------------------------------

	/*!
	Describes the status of the widget after hovering over it with the mouse.
	*/
	enum WidgetMouseState
	{
		Normal = 0,
		Hover = 1,
		Press = 2
	};

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------
	template<class T> class SimpleButton;
	template<class T> void swap(SimpleButton<T>&, SimpleButton<T>&) noexcept;


	template<class T>
	/*!
	Basic interactive widget - displays a button that reacts to mouse clicks. It has two forms - it can
	be displayed as a rectangular area of a given color or as a picture.
	**SimpleButton<Image> button(window, {{0,0}, {100, 30}}, theme, "Btn", &image);
	**...or...
	**SimpleButton<Background> button(window, {{0,0}, {100, 30}}, theme, "Btn");
	*/
	class SimpleButton : public Widget2D
	{
		friend class ContextMenuItem;
	public:

		/*!
		Default constructor. 
		*/
		SimpleButton(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const SimpleButtonTheme& buttonTheme = SimpleButtonTheme(),
			std::wstring btnText = L"",
			Texture* imageTex = nullptr
		);

		/*!
		Delegating constructor with theme application.
		*/
		SimpleButton(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const Theme& theme = Theme(),
			std::wstring btnText = L"",
			Texture* imageTex = nullptr
		);

		/*!
		Copy constructor.
		*/
		SimpleButton(const SimpleButton& copy);

		/*!
		Destructor.
		*/
		virtual ~SimpleButton();

		/*!
		Assignment operator.
		*/
		SimpleButton& operator=(SimpleButton rhs);

		/*!
		Swap function.
		*/
		friend void swap<>(SimpleButton<T>& first, SimpleButton<T>& second) noexcept;

		/*!
		Clones the object (usefull while making copy of object that is casted to its base).
		*/
		SimpleButton* Clone() const;

		/*!
		Sets a callback that is executed every time an event is processed (inside button object).
		*/
		void SetCallback(Callback function);

		/*!
		Resets the callback to nullptr.
		*/
		void ResetCallback();

		/*!
		Returns the current status of the widget (useful when using the pooling technique).
		*/
		WidgetMouseState GetState();

		/*!
		Returns information whether the user drags the mouse over the button.
		*/
		bool GetDragInfo();


		/*!
		Sets the background color for a given widget state.
		*/
		void SetColor(WidgetMouseState forState, Color color);

		/*!
		Sets the background color for all widget states.
		*/
		void SetColor(Color normal, Color hover, Color press);

		/*!
		Sets the border color for a given widget state.
		*/
		void SetBorderColor(WidgetMouseState forState, Color color);
		
		/*!
		Sets the border color for all widget states.
		*/
		void SetBorderColor(Color normal, Color hover, Color press);

		/*!
		Sets the widget border size.
		*/
		void SetBoderSize(Dimension<int> newBorderSize);
		
		/*!
		Sets the font size for the button.
		*/
		void SetFontSize(int newSize);
		
		/*!
		Sets the button text.
		*/
		void SetText(std::wstring newText);
		
		/*!
		Sets the new font for the button.
		*/
		void SetFont(Font* newFont);
		
		/*!
		Sets the font color for all widget states.
		*/
		void SetFontColor(Color newColor);
		
		/*!
		Sets new button state (WidgetMouseState).
		*/
		void SetState(WidgetMouseState state);

		/*!
		Sets the widget flags.
		*/
		void SetFlags(int newFlags) override;
		
		/*!
		Changes the size of the button.
		*/
		void Resize(Dimension<size_t> newDim);
		
		/*!
		Sets whether to delete events that have been handled by the widget or not.
		*/
		void SetBypassEventReturn(bool mode);

		/*!
		Allows to unregister the widget from a specific event in the parent window.
		*/
		void UnregisterEvent(EventType eventType) override;

		/*!
		Returns the background color for a given widget state.
		*/
		Color GetColor(WidgetMouseState forState) const;

		/*!
		Returns the border color for a given widget state.
		*/
		Color GetBorderColor(WidgetMouseState forState) const;

		/*!
		Returns the border size of the widget.
		*/
		Dimension<int> GetBoderSize() const;

		/*!
		Returns font size used by widget to render text.
		*/
		int GetFontSize() const;

		/*!
		Returns the text displayed by the widget.
		*/
		std::wstring GetText() const;

		/*!
		Returns font color used by widget to render text.
		*/
		Color GetFontColor() const;

		/*!
		Returns pointer to the font object used by widget to render text.
		*/
		Font* GetUsedFont() const;

		/*!
		Returns pointer to the base/template widget object.
		*/
		T* GetFrontObject() const;

		//Overrides
		void SetResizeProportions(int x, int y, int w, int h) override;
		void SetPosition(Point<int> newPosition) override;
		void ChangePosition(Point<int> positionChange) override;
		void SetVisibility(bool mode) override;
		bool GetVisibility() const override;
		void EnableWidget() override;
		void DisableWidget() override;
		void Render() override;

	private:
		bool OnMouseMove(Event* ev);
		bool OnMouseClick(Event* ev);
		bool OnMouseEnter(Event* ev);
		bool OnWindowResize(Event* ev);
		bool OnWindowClose(Event* ev);
		bool OnWindowTick(Event* ev);

		/*!
		Converts the location of the widget depending on the flags set (Anchor).
		*/
		void CalcAnchorOffsets();

		/* Internal widget counter. */
		int m_iTicks;

		/* Stores information whether the user drags the mouse over the button. */
		bool m_bDrag;

		/* Stores precalculated (depending on Anchor flags) offsets.*/
		Point<int> m_AnchorOffsets;

		/* Stores pointer to the base/template widget object. */
		T* m_FrontWidget;

		/* Stores Text object. */
		Text* m_Text;

		/* Stores widget theme. */
		SimpleButtonTheme m_BtnTheme;

		/* Stores information whether delete the processed event or not. */
		bool m_bBypassEventsReturn;

		/* Stores widet state. (depends on mouse actions)*/
		WidgetMouseState m_State;

		/* Stores additional internal bindings to the parent window. */
		std::list<BindListIterator> m_Bindings;

		/* Stores callback function. */
		Callback m_Callback;
	};

	//------------------------------------------------------------------

	/*!
	Basic interactive widget - displays a input control that reacts to mouse clicks and keyboard inputs. It supports
	keyboard shortcuts like: Ctrl+A, Ctrl+X, Ctrl+C, Ctrl+V, Left and right Arrows, Backspace. 
	**SimpleInput input(window, {{0,0}, {100, 30}}, theme);
	*/
	class SimpleInput : public Widget2D
	{
	public:

		/*!
		Default constructor.
		*/
		SimpleInput(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const SimpleInputTheme& rectProps = SimpleInputTheme(),
			std::wstring defaultText = L""
		);

		/*!
		Delegating constructor with theme application.
		*/
		SimpleInput(
			Window* window,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const Theme& theme = Theme(),
			std::wstring defaultText = L""
		);

		/*!
		Copy constructor.
		*/
		SimpleInput(const SimpleInput& copy);

		/*!
		Destructor.
		*/
		virtual ~SimpleInput();

		/*!
		Assignment operator.
		*/
		SimpleInput& operator=(SimpleInput rhs);

		/*!
		Swap function.
		*/
		friend void swap(SimpleInput& first, SimpleInput& second) noexcept;

		/*!
		Clones the object (usefull while making copy of object that is casted to its base).
		*/
		SimpleInput* Clone() const;

		/*!
		Returns reference to the input text (quick access).
		*/
		const std::wstring& GetTextRef() const;

		/*!
		Returns the current input text (useful when using the pooling technique). When Enter key was not pressed
		returns empty std::wstirng -> L"".
		*/
		std::wstring GetTextWhenEnter();

		/*!
		Sets a callback that is executed every time an event is processed (inside SimpleInput object).
		*/
		void SetCallback(Callback function);

		/*!
		Resets the callback to nullptr.
		*/
		void ResetCallback();

		/*!
		Sets the filter that passes specific characters to the SimpleInput widget. There are 5 predefinied
		filters: IntNumericFilter, FloatNumericFilter, AlphabeticFilter, AlphaNumericFilter, CustomFilter.
		*/
		void SetFilter(std::function<bool(KbCharAppearEvent*)> function);

		/*!
		Resets the filter to nullptr.
		*/
		void ResetFilter();

		/*!
		Returns the current status of the widget (useful when using the pooling technique).
		*/
		WidgetMouseState GetState();


		/*!
		Sets the background color for a given widget state.
		*/
		void SetColor(WidgetMouseState forState, Color color);

		/*!
		Sets the background color for all widget states.
		*/
		void SetColor(Color normal, Color hover, Color press);

		/*!
		Sets the border color for a given widget state.
		*/
		void SetBorderColor(WidgetMouseState forState, Color color);

		/*!
		Sets the border color for all widget states.
		*/
		void SetBorderColor(Color normal, Color hover, Color press);

		/*!
		Sets the widget border size.
		*/
		void SetBoderSize(Dimension<int> newBorderSize);

		/*!
		Sets the font size for the text inside Input field.
		*/
		void SetFontSize(int newSize);

		/*!
		Sets the text inside Input field.
		*/
		void SetText(std::wstring newText);

		/*!
		Sets the new font for the input text. (Requires redrawn to show changes)
		*/
		void SetFont(Font* newFont);

		/*!
		Sets the font color for all widget states.
		*/
		void SetFontColor(Color newColor);

		/*!
		Sets the widget flags.
		*/
		void SetFlags(int newFlags) override;

		/*!
		Sets the text margins to the left bottom position of the widget.
		*/
		void SetTextMargin(Point<int> newMargins);

		/*!
		Sets the cursor/prompt in a given position.
		*/
		void SetPromptPos(int pos);

		/*!
		Sets new font color for the default text.
		*/
		void SetDefaultTextColor(Color newColor);

		/*!
		Dets new default text.
		*/
		void SetDefaultText(std::wstring newDefaultText);

		/*!
		Allows to unregister the widget from a specific event in the parent window.
		*/
		void UnregisterEvent(EventType eventType) override;

		/*!
		Returns the background color for a given widget state.
		*/
		Color GetColor(WidgetMouseState forState) const;
		
		/*!
		Returns the border color for a given widget state.
		*/
		Color GetBorderColor(WidgetMouseState forState) const;
		
		/*!
		Returns the border size of the widget.
		*/
		const Dimension<int>& GetBoderSize() const;
		
		/*!
		Returns font size used by widget to render text.
		*/
		int GetFontSize() const;
		
		/*!
		Returns the input text.
		*/
		std::wstring GetText() const;
		
		/*!
		Returns font color used by widget to render text.
		*/
		Color GetFontColor() const;
		
		/*!
		Returns pointer to the font object used by widget to render text.
		*/
		Font* GetUsedFont() const;
		
		/*!
		Returns the text margins to the left bottom position of the widget.
		*/
		const Point<int>& GetTextMargin() const;
		
		/*!
		Returns the cursor/prompt position.
		*/
		int GetPromptPos() const;
		
		/*!
		Returns the default text color.
		*/
		Color GetDefaultTextColor() const;
		
		/*!
		Returns the default text.
		*/
		std::wstring GetDefaultText() const;

		/*!
		Converts the location of the widget depending on the flags set (Anchor).
		*/
		void CalcAnchorOffsets();

		//Overrides:
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
		bool OnWindowTick(Event* ev);
		bool OnNewChar(Event* ev);
		bool OnNewKey(Event* ev);

		/*!
		Helper function. Updates the rectangular area which marks the text while selecting it via mouse.
		*/
		void UpdateSelection();

		/* Stores information whether the enter key has been pressed. */
		bool m_Enter;
		
		/* Internal widget counter. */
		int m_iTicks;
		
		/* Describes current prompt position. */
		int m_iPromptIdx;
		
		/* Describes current prompt position in 2D space (X offset)*/
		float m_fPromptPos;
		
		/* Describes whether the SimpleInput widget is active or not.  */
		bool m_bActive;
		
		/* Stores information whether the user drags the mouse over the button. */
		bool m_bDrag;
		
		/* Stores start and end position of the cursor (in characters) while user is dragging mouse. */
		int m_iDragStart, m_iDragEnd;
		
		/* Stores precalculated (depending on Anchor flags) offsets.*/
		Point<int> m_AnchorOffsets;
		
		/* Stores widet state. (depends on mouse actions and m_bActive variable)*/
		WidgetMouseState m_State;
		
		/* Represents the background of the SimpleInput widget. */
		Background* m_Background;
		
		/* Graphical indicator of SimpleInput widget. (bottom bar) */
		Rectangle2D* m_BottomBar;
		
		/* Represents the prompt/cursor in the input field. */
		Rectangle2D* m_Prompt;
		
		/* Represents the text selection. */
		Rectangle2D* m_Selection;
		
		/* Represents the text in the input field. */
		Text* m_Text;
		
		/* Represents the default text in the input field. */
		Text* m_DefaultText;
		
		/* Stores widget theme. */
		SimpleInputTheme m_InpTheme;
		
		/* Stores callback function. */
		Callback m_Callback;
		
		/* Stores filter function. */
		std::function<bool(KbCharAppearEvent*)> m_FilterFunc;
	};

	bool IntNumericFilter(KbCharAppearEvent* ev);
	bool FloatNumericFilter(KbCharAppearEvent* ev);
	bool AlphabeticFilter(KbCharAppearEvent* ev);
	bool AlphaNumericFilter(KbCharAppearEvent* ev);
	bool CustomFilter(KbCharAppearEvent* ev, std::string customChars);

	//---------------------------------------------------------------------

	using ConnectedWidget = std::pair<Widget2D*, Point<int> >;

	/*!
	Basic interactive widget - after connecting other widgets to it - it moves them according to specific rules
	to a different place and takes control over their Render() function. Displays all connected widgets in the
	specified area and the rest of the widgets are cuts out. Provides a graphical bar which allows to move
	the attached widgets. It needs to have a connected AreaWidget widget to works (see ConnectArea() function).
	*/
	class VScroller : public Widget2D
	{
	public:

		/*!
		Default constructor.
		*/
		VScroller(
			Window* window,
			const Widget2DProps& scrollerProps,
			VScrollerTheme theme,
			AreaWidget* area = nullptr
		);

		/*!
		Delegating constructor with theme application.
		*/
		VScroller(
			Window* window,
			const Widget2DProps& scrollerProps,
			Theme theme,
			AreaWidget* area = nullptr
		);

		/*!
		Copy constructor.
		*/
		VScroller(const VScroller& copy);

		/*!
		Destructor. Warning: it does NOT destroy connected widgets.
		*/
		virtual ~VScroller();

		/*!
		Assignment operator.
		*/
		VScroller& operator=(VScroller rhs);

		/*!
		Swap function.
		*/
		friend void swap(VScroller& first, VScroller& second) noexcept;

		/*!
		Clones the object (usefull while making copy of object that is casted to its base).
		*/
		VScroller* Clone() const;

		/*!
		Attaches an AreaWidget widget that specifies the space where new connected widgets will be moved. Please
		check appropriate guide for VScroller widget.
		*/
		void ConnectArea(AreaWidget* area);

		/*!
		Resets the area to nullptr. (VScroller will no longer work until new area is set)
		*/
		void DisconnectArea();

		/*!
		Removes all connected widgets from internal list without destroying them.
		*/
		void Clear();

		/*!
		Adds (connects) widget to the VScroller.
		*/
		void PushBack(Widget2D* widget);

		/*!
		Sets the margins between successive widgets.
		*/
		void SetInsertOffsets(Point<int> offset = { 0,0 });

		/*!
		Recalculates the positions of all connected widgets.
		*/
		void RecalculatePositions(size_t difference = 0);

		/*!
		Sets a callback that is executed every time an event is processed (inside button object).
		*/
		void SetCallback(Callback function);

		/*!
		Resets the callback to nullptr.
		*/
		void ResetCallback();

		/*!
		Sets the scroller value (and shifts connected widgets position).
		*/
		void SetOffset(float offset);

		/*!
		Returns the scroller value.
		*/
		float GetOffset();

		/*!
		Sets the foreground (slider) color for a given widget state.
		*/
		void SetForegroundColor(WidgetMouseState forState, Color color);
		
		/*!
		Sets the foreground (slider) color for all widget states.
		*/
		void SetForegroundColors(Color normal, Color hover, Color press);

		/*!
		Sets the widget background color.
		*/
		void SetBackgroundColor(Color color);

		/*!
		Sets the widget flags.
		*/
		void SetFlags(int newFlags) override;

		/*!
		Returns the foreground (slider) color for a given widget state.
		*/
		const Color GetForegroundColor(WidgetMouseState forState) const;
		
		/*!
		Returns the widget background color.
		*/
		const Color GetBackgroundColor() const;

		//Overrides:
		void SetResizeProportions(int x, int y, int w, int h) override;
		void SetPosition(Point<int> newPosition) override;
		void ChangePosition(Point<int> positionChange) override;
		void SetVisibility(bool mode) override;
		bool GetVisibility() const override;
		void EnableWidget() override;
		void DisableWidget() override;
		void Render() override;

		//TODO:
		//void Insert(Widget2D* widget);
		//void PushFront(Widget2D* widget);
		//void PopBack();
		//void PopFront();
		//void Erase(int idx);

	private:
		bool OnMouseMove(Event* ev);
		bool OnMouseClick(Event* ev);
		bool OnMouseEnter(Event* ev);
		bool OnMouseScroll(Event* ev);
		bool OnWindowResize(Event* ev);
		bool OnWindowClose(Event* ev);
		bool OnDrawableInvokeRender(Event* ev);

		/*!
		Converts the location of the widget depending on the flags set (Anchor).
		*/
		void CalcAnchorOffsets();

		/*!
		Updates the size of the slider.
		*/
		void UpdateScrollerDim();

		/*!
		Calculates the position change in the Y axis to fix some calculation problems. (depends on Anchor flags)
		*/
		void CalculateAnchorFix();
		
		/*!
		Helps to calculate the real position of the AreaWidget widget depending on its Anchor flags.
		*/
		Point<int> CalculateAreaAnchor();

		
		/* Stores the X location for the new widget (if SmartPlacement flag is present). */
		int m_SmartXPlace;

		/* Stores information about the number of rows added. (needed to calculate new Y position)*/
		int m_Rows;

		/* Stores data set by SetInsertOffsets function. (margins to each widget) */
		Point<int> m_InsertOffs;

		/* Stores precalculated (depending on Anchor flags) offsets.*/
		Point<int> m_AnchorOffsets;

		/* Check CalculateAnchorFix() function. */
		int m_iAnchorFix;

		/* Represents the background of the widget. */
		Background* m_Background;

		/* Represents the slider. */
		SimpleButton<Background>* m_Button;
		
		/* Stores additional internal bindings to the parent window. */
		std::list<BindListIterator> m_Bindings;
		
		/* Stores callback function. */
		Callback m_Callback;
		
		/* Stores widget theme. */
		VScrollerTheme m_Theme;

		/* Stores pointer to the AreaWidget. */
		AreaWidget* m_Area;

		/* Helper variable that stores the initial position of the slider. */
		Point<int> m_iBtnSave;

		/* Helper variables that represents the mouse click position while dragging slider. */
		Point<int> m_DragData[3]; //Drag start, Drag end, Button position
		
		/* Stores connected objects/widgets. */
		std::list<ConnectedWidget> m_ConnectedWidgets;
		
		/* Stores the actual height of the connected objects.  */
		int m_iContentHeight;
		
		/* Represents the real position for first line to be drown. (based on m_iContentHeight)  */
		int m_iDisplayLine, m_iDisplayLineBackup;
	};


	//---------------------------------------------------------------------

	/*!
	This is the base for quickly creating new widgets. It can also be used as a shell class that emulates
	other widgets in runtime.
	*/
	class CustomWidget : public Widget2D
	{
	public:

		/*!
		Default constructor.
		*/
		CustomWidget(Window* window, const Widget2DProps& props);

		/*!
		Copy constructor
		*/
		CustomWidget(const CustomWidget& copy);
		
		/*!
		Destructor. Deletes all widgets added to m_Components list (via AddComponent() function).
		*/
		virtual ~CustomWidget();

		/*!
		Assignment operator.
		*/
		CustomWidget& operator=(CustomWidget rhs);

		/*!
		Swap function.
		*/
		friend void swap(CustomWidget& first, CustomWidget& second) noexcept;

		/*!
		Clones the object (usefull while making copy of object that is casted to its base). Warning:
		Please implement this function in derived classes for compatibility reasons!
		*/
		virtual CustomWidget* Clone() const;

		/*!
		Helps to translate the anchor for internal widgets/components.
		*/
		void TranslateAnchor(Point<int>& position, int oldFlags, int newFlags);

		/*!
		Allows to easily add the widget-component to the internal list (version with explicit ID).
		*/
		bool AddComponent(int idx, Widget2D* widget);

		/*!
		Allows to easily add the widget-component to the internal list. The identifier is automatically assigned.
		If component is already present at passed ID, then addotion is canceled.
		*/
		int AddComponent(Widget2D* widget);

		/*!
		Returns (from m_Components list) widget associated with particular ID .
		*/
		Widget2D* GetComponent(int idx);

		/*!
		Returns widgets amount from m_Components list.
		*/
		size_t ComponentAmount();

		//Overrides:
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

		/* Stores widgets that are parts of implemented/target widget. */
		std::map<int, Widget2D*> m_Components;

		/* Helper variable. Stores ID of the last added widget. */
		int m_LastInsertionPos;
	};
	
	//---------------------------------------------------------------------
	

	class ContextMenu;

	class BasicItem : public CustomWidget
	{
	public:
		BasicItem(Window* window, Widget2DProps props, ContextMenu* cm);
		virtual ~BasicItem();

		ContextMenu* GetParentWidget();

	private:
		ContextMenu* m_ParentWidget;

	};

	//---------------------------------------------------------------------

	class ContextMenuItem : public BasicItem
	{
	public:
		ContextMenuItem(Window* window, Widget2DProps props, ContextMenu* cm, SimpleButtonTheme btnTheme);
	
		void SetText(TextTheme props, std::wstring text);
		void SetImage(ImageTheme props, Texture * texture = nullptr);

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

		DividerItem(Window* window, Widget2DProps props, ContextMenu* cm, Color tint);
	};

	//---------------------------------------------------------------------

	class ContextMenu : public CustomWidget
	{
	public:
		ContextMenu(Window* window, Dimension<size_t> dimension, ContextMenuTheme theme);

		~ContextMenu();


		ContextMenuItem* AddItem(std::wstring text, Texture* texture = nullptr);
		void AddDivider(Color dividerColor);

		void SetPosition(Point<int> newPos) override;

	private:

		void Resize(Dimension<size_t> newSize, int heightShift);
		void SetupButton();


		ContextMenuTheme m_Theme;
		SimpleButton<Background>* m_Button;
		int m_TotalHeight;
	};

	/*

	//---------------------------------------------------------------------

	class MenuItem : public SimpleButton
	{
	public:
		MenuItem(
			Window* window,
			int contextMenuWidth,
			const Widget2DProps& props = Widget2DProps({ 0,0 }, { 0,0 }, Anchor::Left | Anchor::Bottom | ResizePolicy::AutoFill),
			const BackgroundTheme& BackgroundTheme = BackgroundTheme(),
			const TextTheme& textProps = TextTheme(),
			Color hoverColor = {255,255,255,255}
		) :
			SimpleButton(window, props, BackgroundTheme, textProps)
		{
			m_Cm = new ContextMenu(window, { (size_t)contextMenuWidth,0 }, BackgroundTheme, BackgroundTheme.Base.Tint, 22 );
			SetCallback([this](Event* ev){
				if (ev->GetEventType() == EventType::MouseClick){
					MouseClickEvent* mce = (MouseClickEvent*)ev;
					if (mce->GetAction() == 0 && mce->GetButton() == 0) {
						m_Cm->SetPosition(this->GetPosition());
						m_Cm->EnableWidget();
					}
				}
				return true;
			});

		};

		virtual ~MenuItem()
		{
			delete m_Cm;
		}

		ContextMenu* GetContextMenu()
		{
			return m_Cm;
		}

	private:
		ContextMenu* m_Cm;

	};


	class AppMenu : public CustomWidget
	{
	public:
		AppMenu(Window* window, BackgroundTheme buttonTheme, Color itemHoverColor, int height, int contextMenuWidth = 120):
			CustomWidget(window, { {0,0}, {0,0}, Anchor::Left|Anchor::Bottom })
		{
			m_BtnPosition = 0;
			m_HoverColor = itemHoverColor;

			//Bar creation
			Point<int> pos = { -1, (int)window->GetDimension().tHeight - height };
			Dimension<size_t> dim = { window->GetDimension().tWidth + 2, (size_t)height+1};
			
			m_Bg = new Background(window, { pos, dim, Anchor::Left | Anchor::Bottom }, buttonTheme);
			AddComponent(0, m_Bg);
			SetResizeProportions(0, 100, 100, 0);

			//Allocate contextmenu
			m_Cm = new ContextMenu(
				window,
				{ (size_t)contextMenuWidth, 0 },
				buttonTheme,
				buttonTheme.Base.Tint,
				24
			);
		}

		~AppMenu()
		{
			delete m_Cm;
		}

		MenuItem* AddButton(TextTheme textProps, int width, int contextMenuWidth)
		{
			Point<int> pos = m_Bg->GetPosition();
			Dimension<size_t> dim = m_Bg->GetDimension();
			MenuItem* mi = new MenuItem(
				m_ParentWindow,
				contextMenuWidth,
				Widget2DProps(pos + Point<int>{m_BtnPosition, 1}, { (size_t)width, dim.tHeight }, Anchor::Left | Anchor::Bottom),
				{ m_Bg->GetColor(), {0,0,0,0}, {0,0} },
				textProps,
				m_HoverColor
			);
			mi->SetColor(WidgetMouseState::Hover, m_HoverColor);
			mi->SetResizeProportions(0, 100, 0, 0);
			AddComponent(mi);
			m_BtnPosition += width;
			return mi;
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
		Background* m_Bg;
		Color m_HoverColor;
		int m_BtnPosition;
	};
		*/

	//----------------------------------------------------------

	#define PG_BACKGROUND 1
	#define PG_FILL 2
	#define PG_TEXT 3

	/*!
	Interactive widget, based on CustomWidget. Displays the progress bar.
	*/
	class ProgressBar : public CustomWidget
	{
	public:

		/*!
		Default constructor (to refactor).
		*/
		ProgressBar(Window* window, Widget2DProps props, ProgressBarTheme theme, std::wstring defText, float maxValue);

		/*!
		Delegating constructor with theme application.
		*/
		ProgressBar(Window* window, Widget2DProps props, Theme theme, std::wstring defText, float maxValue);

		/*!
		Copy constructor.
		*/
		ProgressBar::ProgressBar(const ProgressBar& copy);
		/*!
		Assignment operator.
		*/
		ProgressBar& ProgressBar::operator=(ProgressBar rhs);

		/*!
		Swap function.
		*/
		friend void swap(ProgressBar& first, ProgressBar& second) noexcept;

		/*!
		Clone funciton.
		*/
		ProgressBar* Clone();

		/*!
		Sets the variable that is listened to and upon which the progress bar is updated.
		*/
		void SetToObserve(float* toObserve);

		/*!
		Sets a new value for the observed variable.
		*/
		void SetValue(float newValue);

		/*!
		Updates the widget.
		*/
		void Update();

		/*!
		Sets a callback that is executed every time an event is processed (inside ProgressBar object).
		*/
		void SetCallback(Callback cbFunc);
		
		/*!
		Resets the callback to nullptr.
		*/
		void ResetCallback();

		/*!
		Changes the fill color of the progress bar.
		*/
		void ChangeFillColor(Color newColor);


		//Overrides:
		void EnableWidget() override;
		bool OnWindowTick(Event* ev);

	private:
		
		/* Stores handle to callback function. */
		Callback m_Callback;

		/* Defines the text displayed on the progress bar. */
		std::wstring m_wsBaseText;

		/* Pointer on the observed variable. */
		float* m_fObservedValue;

		/* A variable that specifies the upper limit of the observed variable. */
		float m_fMaxValue;
	};

}
