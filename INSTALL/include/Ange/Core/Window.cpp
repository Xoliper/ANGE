#include "Apch.h"
#include "Window.h"
#include "Ange/Core/Logger.h"

namespace Ange {

	//Static initialization -----------------------------------------

	bool Window::s_GLFWInitialized = false;

	//Constructors & Destructor -----------------------------------------

	Window::Window(Window* parent, std::string windowName, const Widget2DProps& props) noexcept : Widget2D(parent, props)
	{
		m_GLFWWindow = nullptr;
		m_ShaderManager = nullptr;
		m_World = nullptr;
		m_Callback = nullptr;
		m_WidgetType = WidgetType::Window;
		m_WindowProps.m_WindowTitle = windowName;
		SetFlags(props.iFlags);
	}

	Window::Window(const Window & copy) :
		Widget2D(copy)
	{
		m_GLFWWindow = nullptr;
		m_ShaderManager = nullptr;
		m_World = nullptr;
		m_LastMoveEvent = nullptr;
		m_Callback = nullptr;
		m_Events.clear();
		m_FunctionBindings.clear();
		m_FunctionBindingsCpy.clear();
		m_bFifoDrawable = copy.m_bFifoDrawable;
		m_bBindingsChangeFlag = copy.m_bBindingsChangeFlag;
		m_WindowProps = copy.m_WindowProps;
		m_WindowProps.m_WindowTitle = copy.m_WindowProps.m_WindowTitle + " - copy";
		if (m_WindowProps.bState == true) {
			m_WindowProps.bState = false;
			Init();
			std::cout << "einit" << std::endl;
		}
		SetMinMaxDimensions(m_ResizableProps.iMinWidth, m_ResizableProps.iMinHeight, m_ResizableProps.iMaxWidth, m_ResizableProps.iMaxHeight);
	}


	Window::~Window() noexcept
	{
		Cleanup();
	}

	Window& Window::operator=(Window rhs)
	{
		swap(*this, rhs);
		return *this;
	}

	Window* Window::Clone() const
	{
		return new Window(*this);
	}

	void swap(Window& first, Window& second) noexcept
	{
		using std::swap;
		swap(*first.m_World, *second.m_World);
		swap(first.m_Widget2DProps, second.m_Widget2DProps);
		swap(first.m_ResizableProps, second.m_ResizableProps);
		
		first.SetWindowSize({ first.m_Widget2DProps.Dimensions.tWidth, first.m_Widget2DProps.Dimensions.tHeight });
		second.SetWindowSize({ second.m_Widget2DProps.Dimensions.tWidth, second.m_Widget2DProps.Dimensions.tHeight });

		first.SetMinMaxDimensions(first.m_ResizableProps.iMinWidth, first.m_ResizableProps.iMinHeight, first.m_ResizableProps.iMaxWidth, first.m_ResizableProps.iMaxHeight);
		second.SetMinMaxDimensions(second.m_ResizableProps.iMinWidth, second.m_ResizableProps.iMinHeight, second.m_ResizableProps.iMaxWidth, second.m_ResizableProps.iMaxHeight);
	}

	//Access methods -------------------------------------------------

	const WindowType & Window::GetWindowType() const noexcept
	{
		return m_WindowType;
	}

	bool Window::GetFocusStatus() const noexcept
	{
		return m_WindowProps.bFocus;
	}

	const Dimension<size_t> Window::GetPhysicalWindowDim() const noexcept
	{
		if (m_ParentWindow != nullptr) {
			Window* temp = m_ParentWindow;
			while (temp->m_ParentWindow != nullptr) {
				temp = temp->m_ParentWindow;
			}
			return temp->GetDimension();
		}
		return GetDimension();
	}

	const Point<int> Window::GetRealPosition() const noexcept
	{
		Point<int> realPosition = GetPosition();
		if (m_ParentWindow != nullptr) {
			Window* temp = m_ParentWindow;
			while (temp->m_ParentWindow != nullptr) {
				realPosition += temp->GetPosition();
				temp = temp->m_ParentWindow;
			}
			return realPosition;
		}
		return  realPosition;
	}

	ShaderManager* Window::GetShaderManager() const noexcept
	{
		ANGE_NULLPTR_TEST(m_ShaderManager, "m_ShaderManager == nullptr!");
		return m_ShaderManager;
	}

	World* Window::GetWorld() noexcept
	{
		ANGE_NULLPTR_TEST(m_World, "m_World == nullptr!");
		return m_World;
	}

	Window* Window::GetTopWindow() noexcept
	{
		if (m_ParentWindow != nullptr) {
			Window* temp = m_ParentWindow;
			while (temp->m_ParentWindow != nullptr) {
				temp = temp->m_ParentWindow;
			}
			return temp;
		}
		return this;
	}

	GLFWwindow* Window::GetGLFWwindow() const noexcept
	{
		return m_GLFWWindow;
	}

	void Window::SetWindowSize(Dimension<size_t> newSize) noexcept
	{
		if (m_WindowType == WindowType::Parent) {
			ANGE_NULLPTR_TEST(m_GLFWWindow, "m_GLFWWindow == nullptr!");
			glfwSetWindowSize(m_GLFWWindow, (int)newSize.tWidth, (int)newSize.tHeight);
		} else {
			RaiseEvent(new WindowResizeEvent(newSize.tWidth, newSize.tHeight));
		}
		if (newSize.tHeight < m_ResizableProps.iMinHeight || newSize.tHeight > m_ResizableProps.iMaxHeight)
			ANGE_WARNING("[SetWindowSize] Cannot set new widget height because the new size is outside the limits (set by SetMinMaxDimensions()).");

		if (newSize.tWidth < m_ResizableProps.iMinWidth || newSize.tWidth > m_ResizableProps.iMaxWidth)
			ANGE_WARNING("[SetWindowSize] Cannot set new widget width because the new size is outside the limits (set by SetMinMaxDimensions()).");
	};

	void Window::SetPosition(Point<int> newPosition) noexcept
	{
		m_Widget2DProps.Position = newPosition;
		if (m_WindowType == WindowType::Parent) {
			ANGE_NULLPTR_TEST(m_GLFWWindow, "m_GLFWWindow == nullptr!");
			glfwSetWindowPos(m_GLFWWindow, newPosition.tX, newPosition.tY);
		}
	};

	void Window::ChangePosition(Point<int> positionChange) noexcept
	{
		m_Widget2DProps.Position += positionChange;
		if (m_WindowType == WindowType::Parent) {
			ANGE_NULLPTR_TEST(m_GLFWWindow, "m_GLFWWindow == nullptr!");
			glfwSetWindowPos(m_GLFWWindow, m_Widget2DProps.Position.tX, m_Widget2DProps.Position.tY);
		}
	};

	void Window::SetVisibility(bool mode) noexcept
	{
		m_Widget2DProps.bVisibility = mode;
		if (m_WindowType == WindowType::Parent) {
			if (mode == false) {
				glfwIconifyWindow(m_GLFWWindow);
			}
			else {
				glfwRestoreWindow(m_GLFWWindow);
			}
		}
	};

	bool Window::GetVisibility() const noexcept
	{
		return m_Widget2DProps.bVisibility;
	}

	void Window::SetWindowTitle(std::string newTitle) noexcept
	{
		m_WindowProps.m_WindowTitle = newTitle;
		if (m_WindowType == WindowType::Parent) {
			ANGE_NULLPTR_TEST(m_GLFWWindow, "m_GLFWWindow == nullptr!");
			glfwSetWindowTitle(m_GLFWWindow, newTitle.c_str());
		}
	}

	void Window::SetMinMaxDimensions(int iMinWidth, int iMinHeight, int iMaxWidth, int iMaxHeight) noexcept
	{
		if (GetWindowType() == WindowType::Parent) {
			if (m_GLFWWindow == nullptr)
			{
				ANGE_WARNING("Using 'SetWindowMinimumAndMaximumSize' on uninitialized Window!");
				return;
			}
			glfwSetWindowSizeLimits(m_GLFWWindow, iMinWidth, iMinHeight, iMaxWidth, iMaxHeight);
		}
		Resizable2D::SetMinMaxDimensions(iMinWidth, iMinHeight, iMaxWidth, iMaxHeight);
	}

	std::string Window::GetWindowName()
	{
		return m_WindowProps.m_WindowTitle;
	}

	void Window::SetCallback(Callback callback) noexcept
	{
		m_Callback = callback;
	}

	
	void Window::ResetCallback() noexcept
	{
		m_Callback = nullptr;
	}

	void Window::SetFlags(int newFlags) noexcept
	{
		Widget2D::SetFlags(newFlags);
		if (m_Widget2DProps.iFlags & WindowFlags::FifoDrawable) {
			m_bFifoDrawable = true;
		}
	}


	void Window::EnableWidget() noexcept
	{
		if (GetWindowType() == WindowType::Parent){
			//Setup Window Callbacks
			//-------------------------------------------------------------------------------------------------------
			glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* window, int width, int height)
			{
				//Update window dimension
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				angeWindow.m_Widget2DProps.Dimensions.Set(width, height);
				//Send Event to subscribers

				ANGE_EVENT("WindowResize on [%s] raised! (%ux%u)", angeWindow.GetWindowName().c_str(), width, height);
				Event* ev = new WindowResizeEvent(width, height);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if(ret) angeWindow.RaiseEvent(ev);
				} else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* window)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				//angeWindow.Close();
				ANGE_EVENT("WindowClose on [%s] raised!", angeWindow.GetWindowName().c_str());
				Event* ev = new WindowCloseEvent(nullptr);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetWindowPosCallback(m_GLFWWindow, [](GLFWwindow* window, int xpos, int ypos)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				angeWindow.m_Widget2DProps.Position.Set(xpos, ypos);
				ANGE_EVENT("WindowMove on [%s] raised! (%ux%u)", angeWindow.GetWindowName().c_str(), xpos, ypos);
				Event* ev = new WindowMoveEvent(xpos, ypos);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetWindowIconifyCallback(m_GLFWWindow, [](GLFWwindow* window, int iconify)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("WindowIconify on [%s] raised! (%s)", angeWindow.GetWindowName().c_str(), iconify ? "True" : "False");
				Event* ev = new WindowIconifyEvent(iconify);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetWindowFocusCallback(m_GLFWWindow, [](GLFWwindow* window, int focused)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("WindowFocus on [%s] raised! (%s)", angeWindow.GetWindowName().c_str(), focused ? "True" : "False");
				Event* ev = new WindowFocusEvent(focused);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetWindowRefreshCallback(m_GLFWWindow, [](GLFWwindow* window)
			{
				Window* angeWindow = (Window*)glfwGetWindowUserPointer(window);
				int width, height;
				glfwGetWindowSize(window, &width, &height);

				//Do the resize thing
				angeWindow->m_Widget2DProps.Dimensions.Set(width, height);
				glViewport(0, 0, (int)angeWindow->m_Widget2DProps.Dimensions.tWidth, (int)angeWindow->m_Widget2DProps.Dimensions.tHeight);
				glScissor(0, 0, (int)angeWindow->m_Widget2DProps.Dimensions.tWidth, (int)angeWindow->m_Widget2DProps.Dimensions.tHeight);
				angeWindow->m_World->Resize({(size_t)width,(size_t)height});

				//Add some events to current event list.
				angeWindow->RaiseEvent(new WindowResizeEvent(width,height));
				if (angeWindow->m_Widget2DProps.iFlags & WindowFlags::ChildAutoOperate) {
					angeWindow->m_Events.push_back(new WindowInvokeOperateFuncEvent(angeWindow));
				}
				if (angeWindow->m_Widget2DProps.iFlags & WindowFlags::AutoInvokeRender) {
					angeWindow->m_Events.push_front(new DrawableInvokeRender());
				}

				//Do some "retarded" Operate() functionality.
				angeWindow->MakeCurrent();
				angeWindow->ClearScene();
				angeWindow->DispatchEvents(angeWindow);
				glfwSwapBuffers(window);
			});


			//Setup Mouse Callbacks
			//-------------------------------------------------------------------------------------------------------
			glfwSetCursorEnterCallback(m_GLFWWindow, [](GLFWwindow* window, int entered)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("MouseEnter on [%s] raised! (%s)", angeWindow.GetWindowName().c_str(), entered ? "True" : "False");
				Event* ev = new MouseEnterEvent(entered);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* window, double  xpos, double  ypos)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("MouseMove on [%s] raised! (%dx%d)", angeWindow.GetWindowName().c_str(), static_cast<int>(xpos), static_cast<int>(angeWindow.GetDimension().tHeight - ypos));
				Event* ev = new MouseMoveEvent(static_cast<int>(xpos), static_cast<int>(angeWindow.GetDimension().tHeight - ypos));
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("MouseClick on [%s] raised! [%u] [%s] [%u], (%u x %u)", angeWindow.GetWindowName().c_str(), button, action ? "Pressed" : "Released", mods, static_cast<int>(xpos), static_cast<int>(angeWindow.GetDimension().tHeight - ypos));
				Event*ev = new MouseClickEvent(button, action, mods, static_cast<int>(xpos), static_cast<int>(angeWindow.GetDimension().tHeight - ypos));
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("MouseScroll on [%s] raised! (%fx%f)", angeWindow.GetWindowName().c_str(), xoffset, yoffset);
				Event* ev = new MouseScrollEvent(xoffset, yoffset, static_cast<int>(xpos), static_cast<int>(angeWindow.GetDimension().tHeight - ypos));
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});


			//Setup Keyboard Callbacks
			//-------------------------------------------------------------------------------------------------------
			glfwSetCharModsCallback(m_GLFWWindow, [](GLFWwindow* window, unsigned int codepoint, int mods)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("KbChar on [%s] raised! [%u] - > [%c], [%u]", angeWindow.GetWindowName().c_str(), codepoint, static_cast<char>(codepoint), mods);
				Event* ev = new KbCharAppearEvent((Window*)glfwGetWindowUserPointer(window), codepoint, mods);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});

			glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Window& angeWindow = *(Window*)glfwGetWindowUserPointer(window);
				ANGE_EVENT("KbKey on [%s] raised! [%u] - > [%c], [%u]", angeWindow.GetWindowName().c_str(), scancode, static_cast<char>(scancode), mods);
				Event* ev = new KbKeyAppearEvent((Window*)glfwGetWindowUserPointer(window), key, scancode, action, mods);
				if (angeWindow.m_Callback != nullptr) {
					bool ret = angeWindow.m_Callback(ev);
					if (ret) angeWindow.RaiseEvent(ev);
				}
				else {
					angeWindow.RaiseEvent(ev);
				}
			});


			//Register functions
			m_InternalBindings.push_back(BindEvent(Ange::EventType::WindowResize, I_BIND(Window, OnWindowResize)));
			m_InternalBindings.push_back(BindEvent(Ange::EventType::WindowIconify, I_BIND(Window, OnWindowIconify)));
			m_InternalBindings.push_back(BindEvent(Ange::EventType::WindowMove, I_BIND(Window, OnWindowMove)));
			m_InternalBindings.push_back(BindEvent(Ange::EventType::WindowFocus, I_BIND(Window, OnWindowFocusChange)));
			m_InternalBindings.push_back(BindEvent(Ange::EventType::WindowClose, I_BIND(Window, OnWindowClose)));
			glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR_DISABLED, true);

		} else {
			//Register functions
			m_ExternalBindings.push_back(m_ParentWindow->BindEvent(Ange::EventType::WindowClose, I_BIND(Window, OnWindowClose)));
			m_ExternalBindings.push_back(m_ParentWindow->BindEvent(Ange::EventType::All, I_BIND(Window, SubWindowAlterEvent)));
			m_InternalBindings.push_back(BindEvent(Ange::EventType::WindowClose, I_BIND(Window, OnWindowClose)));

		}


	}

	void Window::DisableWidget() noexcept
	{
		//Notice: This will not remove m_FunctionBindings from others Widgets!

		if (GetWindowType() == WindowType::Parent) {
			//Remove callbacks
			glfwSetWindowSizeCallback(m_GLFWWindow, nullptr);
			glfwSetWindowCloseCallback(m_GLFWWindow, nullptr);
			glfwSetWindowPosCallback(m_GLFWWindow, nullptr);
			glfwSetWindowIconifyCallback(m_GLFWWindow, nullptr);
			glfwSetWindowFocusCallback(m_GLFWWindow, nullptr);
			glfwSetCursorEnterCallback(m_GLFWWindow, nullptr);
			glfwSetCursorPosCallback(m_GLFWWindow, nullptr);
			glfwSetMouseButtonCallback(m_GLFWWindow, nullptr);
			glfwSetScrollCallback(m_GLFWWindow, nullptr);
			glfwSetCharModsCallback(m_GLFWWindow, nullptr);
			//Remove Bindings
			for (auto binding : m_InternalBindings)
			{
				UnbindEvent(binding);
			}
			m_InternalBindings.clear();

			if (m_ParentWindow != nullptr) {
				for (auto binding : m_ExternalBindings)
				{
					m_ParentWindow->UnbindEvent(binding);
				}
				m_ExternalBindings.clear();
			}

		} else {
			//Remove Bindings
			for (auto binding : m_InternalBindings)
			{
				UnbindEvent(binding);
			}
			m_InternalBindings.clear();

			if (m_ParentWindow != nullptr) {
				for (auto binding : m_ExternalBindings)
				{
					m_ParentWindow->UnbindEvent(binding);
				}
				m_ExternalBindings.clear();
			}

		}
	}

	void Window::Render() noexcept
	{
		m_Events.push_front(new DrawableInvokeRender());
	}

	void Window::UnregisterEvent(EventType eventType)
	{
		for (std::list<BindListIterator>::iterator it = m_InternalBindings.begin(); it != m_InternalBindings.end(); it++)
		{
			if ((*it)->first == eventType) {
				UnbindEvent((*it));
				it = m_InternalBindings.erase(it);
			}
		}
	}


	//On<Action> Events handler -------------------------------------------------

	bool Window::OnWindowResize(Event* event) noexcept //Child should alter
	{
		WindowResizeEvent* ev = static_cast<WindowResizeEvent*>(event);
		m_Widget2DProps.Dimensions.Set(ev->GetDimension().tWidth, ev->GetDimension().tHeight);
		glViewport(0, 0, (int)m_Widget2DProps.Dimensions.tWidth, (int)m_Widget2DProps.Dimensions.tHeight);
		ANGE_NULLPTR_TEST(m_World, "m_World == nullptr!");
		m_World->Resize(ev->GetDimension());
		return false;
	}

	bool Window::OnWindowIconify(Event* event) noexcept
	{
		WindowIconifyEvent* ev = static_cast<WindowIconifyEvent*>(event);
		m_WindowProps.bIconify = ev->GetState();
		return false;
	}

	bool Window::OnWindowMove(Event* event) noexcept
	{
		WindowMoveEvent* ev = static_cast<WindowMoveEvent*>(event);
		m_Widget2DProps.Position.Set(ev->GetPosition().tX, ev->GetPosition().tY);
		return false;
	}

	bool Window::OnWindowFocusChange(Event* event) noexcept
	{
		WindowFocusEvent* ev = static_cast<WindowFocusEvent*>(event);
		m_WindowProps.bFocus = ev->GetState();
		return false;
	}

	bool Window::SubWindowAlterEvent(Event* event) noexcept
	{
		if (event->GetEventType() == EventType::MouseClick) {
			//If action is false (Release mouse button) then this event is passed on
			//If action is true and position is within subwindow coords
			//then position is converted to subwindow coords and passed on
			MouseClickEvent* mce = static_cast<MouseClickEvent*>(event);
			if (mce->GetAction() == false){
				MouseClickEvent* clone = mce->Clone();
				AlterCoords(clone->GetPosition());
				clone->SetOwner(this);
				RaiseEvent(clone);
			} else {
				auto position = mce->GetPosition();
				if (CheckCoords(position)) {
					MouseClickEvent* clone = mce->Clone();
					AlterCoords(clone->GetPosition());
					clone->SetOwner(this);
					RaiseEvent(clone);
				}
			}
		} else if (event->GetEventType() == EventType::MouseMove) {
			//Position is converted to subwindow coords.
			//If position is out of subwindow coords then this event is not propagated.
			MouseMoveEvent* mme = static_cast<MouseMoveEvent*>(event);
			auto position = mme->GetPosition();
			if (CheckCoords(position)) {
				if (m_WindowProps.bEnter == false){
					m_WindowProps.bEnter = true;
					RaiseEvent(new MouseEnterEvent(true));
				}
			} else {
				if (m_WindowProps.bEnter == true) {
					m_WindowProps.bEnter = false;
					RaiseEvent(new MouseEnterEvent(false));
				}
			}
			MouseMoveEvent* clone = mme->Clone();
			AlterCoords(clone->GetPosition());
			RaiseEvent(clone);
		} else if (event->GetEventType() == EventType::WindowResize) {
			//Passing on for now
			WindowResizeEvent* wre = (WindowResizeEvent*)event->Clone();
			CalcResizeData(wre->GetDimension(), m_Widget2DProps.Dimensions, m_Widget2DProps.Position);
			wre->GetDimension() = m_Widget2DProps.Dimensions;
			RaiseEvent(wre);
		} else if (event->GetEventType() == EventType::WindowInvokeOperateFunc) {
			Operate();
		} else if(event->GetEventType() == EventType::WindowClose) {
			//Eat this scumbag!!!
		} else {
			//Copy every other m_Events
			auto copy = event->Clone();
			copy->SetOwner(this);
			RaiseEvent(copy);
		}
		return false;
	}


	bool Window::OnWindowClose(Event* event) noexcept
	{
		if (event->GetOwner() == this) return false; //Prevent infinite loop
		Cleanup();
		return false;
	}

	void Window::Close() noexcept
	{
		Cleanup();
	}

	void Window::Cleanup() noexcept
	{
		//Prevent double delete;
		if (m_WindowProps.bState == false) return;
		//Create new copy (prevents ONP when wnd is deleted without using Operate() even once
		//This also prevents ONP when Cleanup raised by Event not Close() or Destructor
		auto cpy = m_FunctionBindings;
		//Unbind ourself listeners
		DisableWidget();
		//Invoke WindowCloseEvent on listeners
		WindowCloseEvent* wce = new WindowCloseEvent(this);
		for (auto i : cpy)
		{
			if (i.first == EventType::WindowClose || i.first == EventType::All) i.second(wce);
		}
		delete wce;

		//Cleanup ourself
		if (m_WindowType == WindowType::Parent) {
			ANGE_INFO("Closing window [%s]...", m_WindowProps.m_WindowTitle.c_str());
		}
		else {
			ANGE_INFO("Closing child window [%s]...", m_WindowProps.m_WindowTitle.c_str());
		}

		//Delete Shadermanager & World if parent
		if (m_WindowType == WindowType::Parent) {
			delete m_ShaderManager;
			delete m_World;
		}

		//Delete window if parent
		if (m_GLFWWindow != nullptr) {
			glfwDestroyWindow(m_GLFWWindow);
			m_GLFWWindow = nullptr;
		}

		//Reset vars
		m_WindowProps.bState = false;
		m_ShaderManager = nullptr;
		m_World = nullptr;
	}

	//Other methods -------------------------------------------------


	bool Window::Operate() noexcept
	{
		auto top = GetTopWindow();
		glViewport(0, 0, (int)top->GetDimension().tWidth, (int)top->GetDimension().tHeight);

		//Set scissor to ourself
		if(m_ParentWindow != nullptr){
			glScissor(GetRealPosition().tX, GetRealPosition().tY, (int)GetDimension().tWidth, (int)GetDimension().tHeight);
		} else {
			glScissor(0, 0, (int)GetDimension().tWidth, (int)GetDimension().tHeight);
		}


		//Check flags
		if (m_Widget2DProps.iFlags & WindowFlags::ChildAutoOperate) {
			m_Events.push_back(new WindowInvokeOperateFuncEvent(this));
		}
		if (m_Widget2DProps.iFlags & WindowFlags::AutoInvokeRender) {
			m_Events.push_front(new DrawableInvokeRender());
		}

		//Handling m_Events
		DispatchEvents(this);

		//Set scissor back to parent rect
		if (m_ParentWindow != nullptr) {
			if (m_ParentWindow->GetWindowType() == WindowType::Child) {
				glScissor(m_ParentWindow->GetRealPosition().tX, m_ParentWindow->GetRealPosition().tY, (int)m_ParentWindow->GetDimension().tWidth, (int)m_ParentWindow->GetDimension().tHeight);
			} else {
				glScissor(0, 0, (int)m_ParentWindow->GetDimension().tWidth, (int)m_ParentWindow->GetDimension().tHeight); //TODO: check this? double?
			}
		} else {
			glScissor(0, 0, (int)GetDimension().tWidth, (int)GetDimension().tHeight);
		}

		//Do GLFW stuff
		if (IfOpen()) {
			if (m_WindowType == WindowType::Parent) {
				glfwSwapBuffers(m_GLFWWindow);
				glfwPollEvents();
			}
			return true;
		}
		return false;
	}

	void Window::ClearScene() noexcept
	{
		auto top = GetTopWindow();
		glViewport(0, 0, (int)top->GetDimension().tWidth, (int)top->GetDimension().tHeight);
		glScissor(0, 0, (int)top->GetDimension().tWidth, (int)top->GetDimension().tHeight);

		if (IfOpen() && m_WindowType == WindowType::Parent) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	void Window::SetVSync(int value) const noexcept
	{
		if (m_WindowType == WindowType::Child) {
			ANGE_INFO("SetVSync used on Child window [%s] - this operation have no effect...", m_WindowProps.m_WindowTitle.c_str());
		}
		glfwSwapInterval(value);
	}

	bool Window::IfOpen() const noexcept
	{
		return m_WindowProps.bState;
	}

	void Window::MakeCurrent() noexcept
	{
		if (m_WindowType == WindowType::Child) {
			ANGE_INFO("MakeCurrent used on Child window [%s] - this operation have no effect...", m_WindowProps.m_WindowTitle.c_str());
		}
		glfwMakeContextCurrent(m_GLFWWindow);
	}

	void Window::SetClearColor(Color newColor) noexcept
	{
		if (m_WindowType == WindowType::Child) {
			ANGE_INFO("SetClearColor used on Child window [%s] - this operation have no effect...", m_WindowProps.m_WindowTitle.c_str());
		}
		glClearColor(newColor.r, newColor.g, newColor.b, newColor.a);
	}

	void Window::InitParent(const Widget2DProps& props) noexcept
	{
		if (m_WindowProps.bState != 0)
		{
			ANGE_ERROR("You should not use the 'Init' function more than once per window object.");
			return;
		}

		//Create window
		m_Widget2DProps = props;
		m_WindowType = WindowType::Parent;
		m_ResizableProps.BaseDimension = m_Widget2DProps.Dimensions;
		m_ResizableProps.ParentBaseDimension = m_Widget2DProps.Dimensions;
		m_ResizableProps.BasePosition = m_Widget2DProps.Position;


		ANGE_INFO("Creating window [%s] (%ux%u)  --OpenGL", m_WindowProps.m_WindowTitle.c_str(), m_Widget2DProps.Dimensions.tWidth, m_Widget2DProps.Dimensions.tHeight);
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			ANGE_ASSERT(success, "Could not initialize GLFW!");
			if (success > 0) {
				s_GLFWInitialized = true;
			} else {
				s_GLFWInitialized = false;
			}
		}

		glfwWindowHint(GLFW_VISIBLE, 0);
		//glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_NONE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		//Check if borderless
		if (m_Widget2DProps.iFlags & WindowFlags::Borderless) {
			glfwWindowHint(GLFW_DECORATED, false);
			RaiseEvent(new WindowResizeEvent(m_Widget2DProps.Dimensions.tWidth, m_Widget2DProps.Dimensions.tHeight));
		}

		m_GLFWWindow = glfwCreateWindow(static_cast<int>(m_Widget2DProps.Dimensions.tWidth), static_cast<int>(m_Widget2DProps.Dimensions.tHeight), m_WindowProps.m_WindowTitle.c_str(), nullptr, nullptr);
		if (m_GLFWWindow != nullptr)
		{
			glfwSetWindowPos(m_GLFWWindow, m_Widget2DProps.Position.tX, m_Widget2DProps.Position.tY);
			glfwSetWindowUserPointer(m_GLFWWindow, this);
			glfwMakeContextCurrent(m_GLFWWindow);
			SetVSync(true);
			m_WindowProps.bState = true;
			ANGE_SUCCESS("Window [%s] (%ux%u) was created!", m_WindowProps.m_WindowTitle.c_str(), m_Widget2DProps.Dimensions.tWidth, m_Widget2DProps.Dimensions.tHeight);
		}
		else {
			ANGE_ERROR("Window [%s] (%ux%u) could not be created!", m_WindowProps.m_WindowTitle.c_str(), m_Widget2DProps.Dimensions.tWidth, m_Widget2DProps.Dimensions.tHeight);
			return;
		}


		/*glload::LoadTest tst = glload::LoadFunctions();
		if (!tst) {
			ANGE_FATAL("Failed to initialize [%u] OpenGL functions.", tst.GetNumMissing());
			return;
		}*/
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			ANGE_FATAL("Failed to initialize OpenGL functions [GLAD].");
			return;
		}


		//Load default shaders
		ANGE_INFO("Loading shaders... [%s] (%dx%d)", m_WindowProps.m_WindowTitle.c_str(), m_Widget2DProps.Dimensions.tWidth, m_Widget2DProps.Dimensions.tHeight);
		m_ShaderManager = new ShaderManager();

		//Update World data
		m_World = new World(props.Dimensions);

		//Enable Widget
		EnableWidget();

		//Show window
		glfwShowWindow(m_GLFWWindow);

		//Setup some OGL vars
		SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_SCISSOR_TEST);
	}

	void Window::InitChild(const Widget2DProps& props) noexcept
	{
		if (m_ParentWindow == nullptr){
			ANGE_ERROR("Creating child window without parent.");
			return;
		} else {
			m_WindowType = WindowType::Child;
		}

		//Copy window props
		m_WindowProps.bState = true;

		m_World = m_ParentWindow->m_World;
		m_ShaderManager = m_ParentWindow->m_ShaderManager;

		m_ResizableProps.BaseDimension = m_Widget2DProps.Dimensions;
		m_ResizableProps.ParentBaseDimension = m_ParentWindow->m_Widget2DProps.Dimensions;
		m_ResizableProps.BasePosition = m_Widget2DProps.Position;

		//Enable widget
		EnableWidget();
	}

	void Window::Init() noexcept
	{
		if (m_ParentWindow == nullptr){
			InitParent(m_Widget2DProps);
		} else {
			InitChild(m_Widget2DProps);
		}
	}

}
