#include "Apch.h"
#include "Event.h"
#include "Widgets.h"
#include "Ange/Core/Window.h"

namespace Ange {

	enum EventHandle operator | (enum EventHandle obj, enum EventHandle obj2)
	{
		return (EventHandle)((int)obj | (int)obj2);
	}


		EventType Event::GetEventType() const
		{
			return m_Type;
		};

		void Event::SetEventType(EventType eventType)
		{
			m_Type = eventType;
		}

		bool Event::WasProcessed() const
		{
			return m_bProcessed;
		};

		void Event::SetAsProcessed()
		{
			m_bProcessed = true;
		}

		Window* Event::GetOwner()
		{
			return m_OwnedBy;
		}


		void Event::SetOwner(Window* owner)
		{
			m_OwnedBy = owner;
		}

		Event::~Event()
		{
		}

	//------------------------------------------------------------------------------

		TickEvent::TickEvent()
		{
			Event::SetEventType(EventType::Tick);
		}

		TickEvent* TickEvent::Clone() const
		{
			return new TickEvent(*this);
		}

	//------------------------------------------------------------------------------

		DrawableInvokeRender::DrawableInvokeRender()
		{
			Event::SetEventType(EventType::DrawableInvokeRender);
		}

		DrawableInvokeRender* DrawableInvokeRender::Clone() const
		{
			return new DrawableInvokeRender(*this);
		}

	//------------------------------------------------------------------------------


		KbCharAppearEvent::KbCharAppearEvent(Window* owner, unsigned int codepoint, int mods)
		{
			Event::SetEventType(EventType::KbCharAppear);
			m_uiCodePoint = codepoint;
			m_iMods = mods;
			m_OwnedBy = owner;
		}

		KbCharAppearEvent* KbCharAppearEvent::Clone() const
		{
			return new KbCharAppearEvent(*this);
		}

		 unsigned int KbCharAppearEvent::GetCodePoint() const
		{
			return m_uiCodePoint;
		}

		 int KbCharAppearEvent::GetMods() const
		{
			return m_iMods;
		}

	//------------------------------------------------------------------------------

		KbKeyAppearEvent::KbKeyAppearEvent(Window* owner, int key, int scancode, int action, int mods)
		{
			Event::SetEventType(EventType::KbKeyAppear);
			m_iKey = key;
			m_iScanCode = scancode;
			m_iAction = action;
			m_iMods = mods;
			m_OwnedBy = owner;
		}

		KbKeyAppearEvent* KbKeyAppearEvent::Clone() const
		{
			return new KbKeyAppearEvent(*this);
		}

		 int KbKeyAppearEvent::GetKey() const
		{
			return m_iKey;
		}

		 int KbKeyAppearEvent::GetState() const
		{
			return m_iAction;
		}

		 int KbKeyAppearEvent::GetScanCode() const
		{
			return m_iScanCode;
		}

		 int KbKeyAppearEvent::GetMods() const
		{
			return m_iMods;
		}


	//-----------------------------------------------------------------------------------------------------

		MouseEnterEvent::MouseEnterEvent(bool state)
		{
			Event::SetEventType(EventType::MouseEnter);
			m_bState = state;
		}

		MouseEnterEvent* MouseEnterEvent::Clone() const
		{
			return new MouseEnterEvent(*this);
		}

		 bool MouseEnterEvent::GetState() const
		{
			return m_bState;
		}

	//-----------------------------------------------------------------------------------------------------

		MouseMoveEvent::MouseMoveEvent(int xpos, int ypos)
		{
			Event::SetEventType(EventType::MouseMove);
			m_stPosition.Set(xpos, ypos);
			m_bInside = true;
		}

		MouseMoveEvent* MouseMoveEvent::Clone() const
		{
			return new MouseMoveEvent(*this);
		}

		Point<int>& MouseMoveEvent::GetPosition()
		{
			return m_stPosition;
		}

		bool MouseMoveEvent::GetCheckStatus()
		{
			return m_bInside;
		}

		void MouseMoveEvent::SetCheckStatus(bool status)
		{
			m_bInside = status;
		}

	//-----------------------------------------------------------------------------------------------------

		MouseClickEvent::MouseClickEvent(int button, int action, int mods, int xpos, int ypos)
			: m_iButton(button), m_iAction(action), m_iMods(mods)
		{
			Event::SetEventType(EventType::MouseClick);
			m_stPosition.Set(xpos, ypos);
		}

		MouseClickEvent* MouseClickEvent::Clone() const
		{
			return new MouseClickEvent(*this);
		}

		 int MouseClickEvent::GetButton() const
		{
			return m_iButton;
		}

		 int MouseClickEvent::GetAction() const {
			return m_iAction;
		}

		Point<int>& MouseClickEvent::GetPosition()
		{
			return m_stPosition;
		}

	//-----------------------------------------------------------------------------------------------------

		WindowResizeEvent::WindowResizeEvent(size_t width, size_t height)
		{
			Event::SetEventType(EventType::WindowResize);
			m_stDimension.Set(width, height);
		}

		WindowResizeEvent* WindowResizeEvent::Clone() const
		{
			return new WindowResizeEvent(*this);
		}

		Dimension<size_t>& WindowResizeEvent::GetDimension()
		{
			return m_stDimension;
		}

	//-----------------------------------------------------------------------------------------------------

		WindowMoveEvent::WindowMoveEvent(int xpos, int ypos)
		{
			Event::SetEventType(EventType::WindowMove);
			m_stPosition.Set(xpos, ypos);
		}

		WindowMoveEvent* WindowMoveEvent::Clone() const
		{
			return new WindowMoveEvent(*this);
		}

		Point<int>& WindowMoveEvent::GetPosition()
		{
			return m_stPosition;
		}

	//-----------------------------------------------------------------------------------------------------

		WindowCloseEvent::WindowCloseEvent(Window* owner)
		{
			Event::SetEventType(EventType::WindowClose);
			m_OwnedBy = owner;
		}

		WindowCloseEvent* WindowCloseEvent::Clone() const
		{
			return new WindowCloseEvent(*this);
		}

	//-----------------------------------------------------------------------------------------------------

		WindowInvokeOperateFuncEvent::WindowInvokeOperateFuncEvent(Window* owner)
		{
			Event::SetEventType(EventType::WindowInvokeOperateFunc);
			m_OwnedBy = owner;
		}

		WindowInvokeOperateFuncEvent* WindowInvokeOperateFuncEvent::Clone() const
		{
			return new WindowInvokeOperateFuncEvent(*this);
		}

	//-----------------------------------------------------------------------------------------------------

		WindowIconifyEvent::WindowIconifyEvent(int iconify)
		{
			Event::SetEventType(EventType::WindowIconify);
			m_iIconify = iconify;
		}

		WindowIconifyEvent* WindowIconifyEvent::Clone() const
		{
			return new WindowIconifyEvent(*this);
		}

		int WindowIconifyEvent::GetState()
		{
			return m_iIconify;
		}

	//-----------------------------------------------------------------------------------------------------

		WindowFocusEvent::WindowFocusEvent(int focus)
		{
			Event::SetEventType(EventType::WindowFocus);
			m_iFocus = focus;
		}

		WindowFocusEvent* WindowFocusEvent::Clone() const
		{
			return new WindowFocusEvent(*this);
		}

		int WindowFocusEvent::GetState()
		{
			return m_iFocus;
		}

	//-----------------------------------------------------------------------------------------------------

		MouseScrollEvent::MouseScrollEvent(double xoffset, double yoffset, int xpos, int ypos)
		{
			Event::SetEventType(EventType::MouseScroll);
			m_stScroll.Set(xoffset, yoffset);
			m_stPosition.Set(xpos, ypos);
		}

		MouseScrollEvent* MouseScrollEvent::Clone() const
		{
			return new MouseScrollEvent(*this);
		}

		 Point<double> MouseScrollEvent::GetScrollOffsets() const
		{
			return m_stScroll;
		}

		 Point<int> MouseScrollEvent::GetPosition() const
		 {
			 return m_stPosition;
		 }

	//-----------------------------------------------------------------------------------------------------

		 ProgressBarUpdateEvent::ProgressBarUpdateEvent(float ratio)
		 {
			 Event::SetEventType(EventType::ProgressBarUpdate);
			 m_fRatio = ratio;
		 }

		 ProgressBarUpdateEvent* ProgressBarUpdateEvent::Clone() const
		 {
			 return new ProgressBarUpdateEvent(*this);
		 }

		 float ProgressBarUpdateEvent::GetRatio() const
		 {
			 return m_fRatio;
		 }

	//-----------------------------------------------------------------------------------------------------

		 CheckboxChange::CheckboxChange(Widget2D* widget, bool state)
		 {
			 Event::SetEventType(EventType::CheckboxChange);
			 m_Widget = widget;
			 m_State = state;
		 }

		 CheckboxChange*  CheckboxChange::Clone() const
		 {
			 return new CheckboxChange(*this);
		 }

		 bool  CheckboxChange::GetState()
		 {
			 return m_State;
		 }

		 Widget2D*  CheckboxChange::GetWidget()
		 {
			 return m_Widget;
		 }

	//-----------------------------------------------------------------------------------------------------

		 RatioChange::RatioChange(Widget2D* widget, int selected)
		 {
			 Event::SetEventType(EventType::RatioChange);
			 m_Widget = widget;
			 m_Selected = selected;
		 }

		 RatioChange*  RatioChange::Clone() const
		 {
			 return new RatioChange(*this);
		 }

		 int  RatioChange::GetSelectedFieldId()
		 {
			 return m_Selected;
		 }

		 Widget2D*  RatioChange::GetWidget()
		 {
			 return m_Widget;
		 }

	//-----------------------------------------------------------------------------------------------------

		EventDispatcher::EventDispatcher()
		{
			m_bFifoDrawable = false;
			m_bBindingsChangeFlag = true;
			/*for (int i = 0; i < EVENT_TYPES; i++)
			{
				m_FrameEvents[i] = nullptr;
			}*/
			m_LastMoveEvent = nullptr;
		}

		EventDispatcher::~EventDispatcher()
		{
			Cleanup();
		}

		void EventDispatcher::RaiseEvent(Event * event)
		{
			/*int type = (int)event->GetEventType();
			if ((type >= 2 && type <= 5) || type == 14)
			{
				if(m_FrameEvents[type] != nullptr){
					delete m_FrameEvents[type];
				}
				m_FrameEvents[type] = event;
			}*/
			if (event->GetEventType() == EventType::WindowResize) {
				if (m_LastMoveEvent != nullptr) {
					delete m_LastMoveEvent;
				}
				m_LastMoveEvent = event;
			} else {
				m_Events.push_back(event);
			}
		}

		BindListIterator EventDispatcher::BindEvent(EventType eventType, std::function<EventHandle(Event*)> function)
		{
			m_bBindingsChangeFlag = true;
			m_FunctionBindings.push_back(BindListPair(eventType, function));
			return --m_FunctionBindings.end();
		}

		void EventDispatcher::UnbindEvent(BindListIterator it)
		{
			m_bBindingsChangeFlag = true;
			m_FunctionBindings.erase(it);
		}

		void EventDispatcher::DispatchEvents(Window* currentWindow)
		{
			///Checks whether new binding has been added and possibly creates a new copy of the binding list.
			///This allows for dynamic changes of bindings while going through this list.
			if (m_bBindingsChangeFlag) {
				m_FunctionBindingsCpy = m_FunctionBindings;	///A copy is done here.
				m_bBindingsChangeFlag = false;
			}

			///Add tick event (Should be done with cooperation with Timer). This allows the functions that need
			///to perform operations at specific intervals to do the job correctly.

			 // - Measure time
			static double limitFPS = 1.0 / 8.0;
			m_CurrTime = glfwGetTime();
			m_DeltaTime += (m_CurrTime - m_LastTime) / limitFPS;
			m_LastTime = m_CurrTime;

			if (m_DeltaTime >= 1.0) {
				m_Events.push_front(new TickEvent());
				m_DeltaTime--;
			}

			bool close = false;
			bool alreadyDrawn = false;

			///First - handle window resize
			if (m_LastMoveEvent != nullptr) {
				for (auto it = m_FunctionBindingsCpy.rbegin(); it != m_FunctionBindingsCpy.rend(); it++) {
					if (it->first == EventType::WindowResize || it->first == EventType::All) {
						///Execute
						if ((it->second(m_LastMoveEvent) & EventHandle::DontPass) > 0) break;
					}
				}
				delete m_LastMoveEvent;
				m_LastMoveEvent = nullptr;
			}

			for (auto event : m_Events) {

				///If last Event was WindowClose type then, skip all next events.
				if (close == false) {
					if (event->GetEventType() == EventType::WindowClose) {
						close = true;
					}

					///Prevents drawing widgets more than once per cycle.
					bool drawableEval = event->GetEventType() == EventType::DrawableInvokeRender;
					if (drawableEval == true && alreadyDrawn == true) {
						delete event;
						continue;
					}
					else if (drawableEval == true && alreadyDrawn == false) {
						alreadyDrawn = true;
					}

					///Performing events on binded functions.
					if (m_bFifoDrawable == true && drawableEval) {
						for (auto it = m_FunctionBindingsCpy.begin(); it != m_FunctionBindingsCpy.end(); it++) {
							if (it->first == event->GetEventType() || it->first == EventType::All) {
								///Execute
								if((it->second(event) & EventHandle::DontPass) > 0) break;
							}
						}
					}
					else {
						for (auto it = m_FunctionBindingsCpy.rbegin(); it != m_FunctionBindingsCpy.rend(); it++) {
							if (it->first == event->GetEventType() || it->first == EventType::All) {
								///Execute
								if((it->second(event) & EventHandle::DontPass) > 0) break;
							}
						}
					}


				}
				delete event;
			}
			m_Events.clear();
		}

		void EventDispatcher::Cleanup()
		{
			//Delete m_Events belong to this window
			for (auto event : m_Events) {
				delete event;
			}
			m_Events.clear();

			//Remove m_FunctionBindings
			m_FunctionBindings.clear();
		}

}
