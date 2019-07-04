#pragma once

#include <thread>
#include <list>
#include <functional>
#include "Primitives.h"

namespace Ange {

	//------------------------------------------------------------------------------------------------------
	//Forward declarations
	//------------------------------------------------------------------------------------------------------

	class Window;

	//------------------------------------------------------------------------------------------------------
	//Defines
	//------------------------------------------------------------------------------------------------------

	#define I_BIND(className, functionName) std::bind(&className::functionName, this, std::placeholders::_1)

	//------------------------------------------------------------------------------------------------------
	//Enums
	//------------------------------------------------------------------------------------------------------

	/*!
	Defines the type of event. It can be read from events using the <GetEventType> function.
	*/
	enum class EventType
	{
		Default = 0, All = 1,
		WindowInvokeOperateFunc = 2, WindowClose = 3, WindowResize = 4,
		WindowFocus = 5, WindowIconify = 6, WindowMove = 7,
		MouseEnter = 8, MouseMove = 9, MouseClick = 10, MouseScroll = 11,
		KbCharAppear = 12, KbKeyAppear = 13,
		DrawableInvokeRender = 14,
		Tick = 16
	};

	//------------------------------------------------------------------------------------------------------
	//Classes
	//------------------------------------------------------------------------------------------------------

	/*!
	Basic interface for events. Objects of this type are processed by Window class objects.
	*/
	class Event
	{
	public:

		/*!
		The function creates a clone of the object. It is needed in the process of sending events to child windows.
		Typical implementation: "return new ImplementedEventName (* this);"
		*/
		virtual Event* Clone()  const = 0;

		/*!
		Virtual destructor.
		*/
		virtual ~Event();

		/*!
		Returns the type of event.
		*/
		EventType GetEventType() const;

		/*!
		Allows you to set the type of event from outside. Note - wrong setting of this parameter may cause memory
		corruption violation because events are often cast from the base class to the correct one.
		*/
		void SetEventType(EventType eventType);

		/*!
		Returns a flag informing about the event being handled. Returns true if the event has been processed and
		false if it is still waiting to be processed.
		*/
		bool WasProcessed() const;

		/*!
		Allows you to set the event processing flag.
		*/
		void SetAsProcessed();

		/*!
		Returns the handle (pointer) to the window that created the event.
		*/
		Window* GetOwner();

		/*!
		Sets the parent window for the event.
		*/
		void SetOwner(Window* owner);

	protected:

		/*Describes the type of event.*/
		EventType m_Type = EventType::Default;

		/*The state flag of serving the event. If it is equal to "true" the event has been handled.*/
		bool m_bProcessed = false;

		/*Stores the pointer to the Window that created the event.*/
		Window* m_OwnedBy = nullptr;
	};

	//------------------------------------------------------------------------------

	/*!
	An event that occurs when the logic inside the engine needs to be updated.
	*/
	class TickEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		TickEvent();

		/*!
		Implementation of the event cloning function.
		*/
		TickEvent* Clone() const;
	};

	//------------------------------------------------------------------------------

	/*!
	An event which causes Widgets registered in the parent window to draw.
	*/
	class DrawableInvokeRender : public Event
	{
	public:
		/*!
		Default constructor.
		*/
		DrawableInvokeRender();

		/*!
		Implementation of the event cloning function.
		*/
		DrawableInvokeRender* Clone() const;
	};

	//------------------------------------------------------------------------------

	/*!
	An event raised when the parent window captures a key. The keycode is encoded and can be treated as
	a UTF-32 character.
	*/
	class KbCharAppearEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		KbCharAppearEvent(Window* owner, unsigned int codepoint, int mods);

		/*!
		Implementation of the event cloning function.
		*/
		KbCharAppearEvent* Clone() const;

		/*!
		Returns encoded key codepoint.
		*/
		unsigned int GetCodePoint() const;

		/*!
		Returns key modifier.
		*/
		int GetMods() const;

	protected:

		/*Stores data regarding encoded key codepoint.*/
		unsigned int m_uiCodePoint;

		/*Stores data regarding key modifiers.*/
		int m_iMods;
	};

	//------------------------------------------------------------------------------

	/*!
	An event raised when the parent window captures a key. The character is not encoded. An event used
	when handling special keys.
	*/
	class KbKeyAppearEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		KbKeyAppearEvent(Window* owner, int key, int scancode, int action, int mods);

		/*!
		Implementation of the event cloning function.
		*/
		KbKeyAppearEvent* Clone() const;

		/*!
		Returns key.
		*/
		int GetKey() const;

		/*!
		Returns key statce. The "0" is pressed, "1" is released.
		*/
		int GetState() const;

		/*!
		Returns key scancode.
		*/
		int GetScanCode() const;

		/*!
		Returns modifier.
		*/
		int GetMods() const;

	protected:

		/*Stores data regarding key.*/
		int m_iKey;

		/*Stores data regarding key scancode.*/
		int m_iScanCode;

		/*Stores data regarding key state (pressed or released).*/
		int m_iAction;

		/*Stores data regarding key modifiers.*/
		int m_iMods;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the user mouse enters or exits the parent window area.
	*/
	class MouseEnterEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		MouseEnterEvent(bool state);

		/*!
		Implementation of the event cloning function.
		*/
		MouseEnterEvent* Clone() const;

		/*!
		Returns information about whether the mouse enters or exits the parent window area.
		*/
		bool GetState() const;

	protected:

		/*Stores data regarding enter/exit state.*/
		bool m_bState;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the user mouse moves. Mouse coordinates are calculated for a specific
	window.
	*/
	class MouseMoveEvent : public Event
	{
		friend class Window;
	public:

		/*!
		Default constructor.
		*/
		MouseMoveEvent(int xpos, int ypos);

		/*!
		Implementation of the event cloning function.
		*/
		MouseMoveEvent* Clone() const;

		/*!
		Returns information about mouse position, relative to the parent window area.
		*/
		Point<int>& GetPosition();

		/*!
		Returns information about whether the move was made inside the widget (true).
		*/
		bool GetCheckStatus();

		/*!
		Sets the flag which informs whether the move was made inside the widget.
		*/
		void SetCheckStatus(bool status);

	protected:

		/*Stores data regarding mouse position.*/
		Point<int> m_stPosition;
		bool m_bInside;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the user click on mouse. Mouse coordinates are calculated for a specific
	window.
	*/
	class MouseClickEvent : public Event
	{
		friend class Window;
	public:

		/*!
		Default constructor.
		*/
		MouseClickEvent(int button, int action, int mods, int xpos, int ypos);

		/*!
		Implementation of the event cloning function.
		*/
		MouseClickEvent* Clone() const;

		/*!
		Returns mouse click button (eg. 0 -> left, 1 -> right).
		*/
		int GetButton() const;

		/*!
		Returns mouse click state (eg. 0 -> release, 1 -> press).
		*/
		int GetAction() const;

		/*!
		Returns information about mouse position, relative to the parent window area.
		*/
		Point<int>& GetPosition();

	protected:

		/*Stores data regarding mouse click button.*/
		int m_iButton;

		/*Stores data regarding mouse click state.*/
		int m_iAction;

		/*Stores data regarding mouse click modifier.*/
		int m_iMods;

		/*Stores data regarding mouse (click) position.*/
		Point<int> m_stPosition;
	};

	//-----------------------------------------------------------------------------------------------------

		/*!
	The event is generated when the parent window changes its size.
	*/
	class WindowResizeEvent : public Event
	{
		friend class Window;
	public:

		/*!
		Default constructor.
		*/
		WindowResizeEvent(size_t width, size_t height);

		/*!
		Implementation of the event cloning function.
		*/
		WindowResizeEvent* Clone() const;

		/*!
		Returns information about new window dimensions.
		*/
		Dimension<size_t>& GetDimension();

	protected:
		/*Stores data regarding new window dimensions.*/
		Dimension<size_t> m_stDimension;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the top window moves.
	*/
	class WindowMoveEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		WindowMoveEvent(int xpos, int ypos);

		/*!
		Implementation of the event cloning function.
		*/
		WindowMoveEvent* Clone() const;

		/*!
		Returns information about new window position.
		*/
		Point<int>& GetPosition();

	protected:

		/*Stores data regarding new window position.*/
		Point<int> m_stPosition;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the parent window closes.
	*/
	class WindowCloseEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		WindowCloseEvent(Window* owner);

		/*!
		Implementation of the event cloning function.
		*/
		WindowCloseEvent* Clone() const;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	Starts the event processing in the child window.
	*/
	class WindowInvokeOperateFuncEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		WindowInvokeOperateFuncEvent(Window* owner);

		/*!
		Implementation of the event cloning function.
		*/
		WindowInvokeOperateFuncEvent* Clone() const;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the parent window is minimized/maximized.
	*/
	class WindowIconifyEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		WindowIconifyEvent(int iconify);

		/*!
		Implementation of the event cloning function.
		*/
		WindowIconifyEvent* Clone() const;

		/*!
		Returns window iconify state (eg. 0 -> minimize, 1 -> maximize).
		*/
		int GetState();

	protected:

		/*Stores data regarding window iconify state.*/
		int m_iIconify;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the top window obtains or loses focus.
	*/
	class WindowFocusEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		WindowFocusEvent(int focus);

		/*!
		Implementation of the event cloning function.
		*/
		WindowFocusEvent* Clone() const;

		/*!
		Returns window focus state (eg. 0 -> lost focus, 1 -> get focus).
		*/
		int GetState();

	protected:

		/*Stores data regarding window focus state.*/
		int m_iFocus;
	};

	//-----------------------------------------------------------------------------------------------------

	/*!
	The event is generated when the user sroll on mouse.
	*/
	class MouseScrollEvent : public Event
	{
	public:

		/*!
		Default constructor.
		*/
		MouseScrollEvent(double xoffset, double yoffset);

		/*!
		Implementation of the event cloning function.
		*/
		MouseScrollEvent* Clone() const;

		/*!
		Returns information about mouse scroll offsets.
		*/
		Point<double> GetScrollOffsets() const;

	protected:
		/*Stores data regarding mouse scroll offsets.*/
		Point<double> m_stScroll;
	};

	//-----------------------------------------------------------------------------------------------------

	using BindListIterator = std::list < std::pair<EventType, std::function<bool(Event*)> > >::iterator;
	using BindListPair = std::pair<EventType, std::function<bool(Event*)> >;

	/*!
	The base class that provides the Window class functions for binding events, adding or removing them from
	the window, and function that control the flow of events inside the window that will delegate events to
	specific widgets.
	*/
	class EventDispatcher
	{
	public:

		/*!
		Default constructor.
		*/
		EventDispatcher();

		/*!
		Removes all events and bindings that are stored internally. It simply execute the Cleanup() function.
		*/
		~EventDispatcher();

		/*!
		Adds an event to the window event queue.
		*/
		void RaiseEvent(Event * event);

		/*!
		Binds an event to a specific function. It allows you to bind multiple functions to one type of event.
		Returns the iterator needed to unbound a function. The function is added to the end of the processing queue.
		*/
		BindListIterator BindEvent(EventType eventType, std::function<bool(Event*)> function);

		/*!
		It de-binds a previously bound function.
		*/
		void UnbindEvent(BindListIterator it);

	protected:

		/*!
		Delegate events to specific widgets. Events are processed in FIFO queue.
		*/
		void DispatchEvents(Window* currentWindow);

		/*!
		Removes all events and bindings that are stored internally.
		*/
		void Cleanup();

		/*Responsible for the mode of iterating through the binded functions. If true FIFO, if false->FILO.*/
		bool m_bFifoDrawable;

		/*Flag monitoring changes occurring in the list of bound functions.*/
		bool m_bBindingsChangeFlag;

		/*List of Events.*/
		std::list<Event*> m_Events;

		/*List of bound functions.*/
		std::list<BindListPair> m_FunctionBindings;

		/*Copy of m_FunctionBindings list.*/
		std::list<BindListPair> m_FunctionBindingsCpy;

	};

}
