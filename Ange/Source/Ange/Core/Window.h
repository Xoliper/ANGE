#pragma once

#include <thread>
#include <future>
#include "Ange/Core/Primitives.h"
#include "Ange/Core/Event.h"
#include "Ange/Core/Shader.h"
#include "Ange/Core/World.h"
#include "Ange/Core/Widgets.h"
#include <GLFW/glfw3.h>


namespace Ange {

	//-----------------------------------------------------------------------
	//Enums
	//-----------------------------------------------------------------------

	/*!
	Defines the window type for the "Window" widget. The window of the type "Parent" is a physical window created
	with the help of GLFW, while the "Child" type is a virtual subwindow inside the physical window.
	*/
	enum class WindowType
	{
		Parent	= 1,	/*"Physical" GLFW window.*/
		Child	= 2		/*"Virtual" subarea of window which behaves like physical window.*/
	};

	/*!
	Flags for the window widget. The "ChildAutoOperate" flag automatically performs the Operate () function on the
	sub-windows. "AutoInvokeRender" automatically executes the Render () function on the widgets attached to the window.
	And, "FifoDrawable" forces rendering widgets in the FIFO order.
	*/
	enum WindowFlags
	{
		ChildAutoOperate = 1,	/*Automatically performs Operate() function on subwindows.*/
		AutoInvokeRender = 2,	/*Automatically executes Render() function on attached widgets.*/
		FifoDrawable = 4,		/*Forces FIFO rendering.*/
		Borderless = 8			/* WIP */
	};

	//-----------------------------------------------------------------------
	//Structs
	//-----------------------------------------------------------------------

	/*!
	Contains variables describing the window state.
	*/
	struct WindowProps
	{
		/*It describes if the window is closed (false) or open (true).*/
		bool bState = false;

		/*It describes whether the window is minimized (false) or not (true).*/
		bool bIconify = false;

		/*Describes whether the window has focus or not.*/
		bool bFocus = true;

		/*Determines whether the mouse cursor is inside a window or a sub-window.*/
		bool bEnter = false;

		/*Stores the name of the window.*/
		std::string m_WindowTitle;
	};

	//-----------------------------------------------------------------------
	//Classes [In future: Interface with PIMPL for OpenGL & Vulkan & DirectX support]
	//-----------------------------------------------------------------------

	void WakeUpThread();

	/*!
	The main Ange widget. "Window" instance can create a GLFW physical window or sub-window. Requires initialization
	using the Init() method.
	*/
	class Window : public EventDispatcher, public Widget2D
	{
		friend class EventDispatcher;
		friend class ThreadScheduler;
		friend void WakeUpThread();
	public:

		/*!
		Default constructor.
		*/
		Window(
			Window* parent = nullptr,
			std::string windowName = "Ange Window",
			const Widget2DProps& props = Widget2DProps({ 0,0 }, {720, 480}, WindowFlags::FifoDrawable)
		) noexcept;

		/*!
		Copy constructor. Does NOT copy bindings, only position, dimension etc.
		*/
		Window(const Window & copy);

		/*!
		Destructor. Basicly it invokes Cleanup() method.
		*/
		virtual ~Window() noexcept;

		/*!
		Assignment operator.
		*/
		Window& operator=(Window rhs);

		/*!
		Swap function. Swaps only trivial data, such as position, dimension, m_Widget2DPorps, m_World, m_ResizableProps.
		*/
		friend void swap(Window& first, Window& second) noexcept;

		/*!
		Clone function. Does NOT copy bindings, only position, dimension etc.
		*/
		Window* Clone() const override;

		/*!
		Runs all the processes occurring in the window and allows it to operate properly. Based on the dimensions
		of the window, sets the view scissor and executes the function DispatchEvents() of the base class EventDispatcher.
		If the window is of the type "Parent", it automatically performs a buffer swap (glfwSwapBuffers()), and processes
		all events from the GLFW (glfwPollEvents()). Returns true if the window is open.
		*/
		bool Operate(int forceAutoDraw = 0) noexcept;

		/*!
		Replaces the old image with a recently rendered one.
		*/
		void Swap() noexcept;

		/*!
		It the window is of the "Parent" type, it clears previously rendered frame, and setups the background to background
		color.
		*/
		void ClearScene() noexcept;

		/*!
		Starts initialization processes for a specific window type.
		*/
		void Init() noexcept;

		/*!
		Returns value which describes if the window is closed (false) or open (true).
		*/
		bool IfOpen() const noexcept;

		/*!
		Sends a signal to the window to close. Attention, closure may not occur immediately. Because the sent event must
		be processed by the Operate() function.
		*/
		void Close() noexcept;


		/*!
		Sets vertical synchronization. Only for the "Parent" window.
		*/
		void SetVSync(int value) const noexcept;

		/*!
		Sets the window to be active for rendering (for OpenGL). Only for the "Parent" window.
		*/
		void MakeCurrent() noexcept;

		/*!
		Sets the background color. Only for the "Parent" window.
		*/
		void SetClearColor(Color newColor) noexcept;

		/*!
		Returns the actual size of the physical (top) window in pixels.
		*/
		const Dimension<size_t> GetPhysicalWindowDim()		const noexcept;

		/*!
		Returns the actual position of the physical (top) window in pixels. Can be negative.
		*/
		const Point<int> GetRealPosition()					const noexcept;

		/*!
		Returns window type (Parent/Child).
		*/
		const WindowType & GetWindowType()					const noexcept;

		/*!
		Returns pointer to the ShaderManager object from the physical (top) window. Do not delete the
		received object yourself.
		*/
		ShaderManager* GetShaderManager()					const noexcept;

		/*!
		Returns window focus status (eg. 0 -> no focus, 1 -> focused).
		*/
		bool GetFocusStatus()								const noexcept;

		/*!
		Returns pointer to the World object. Do not delete the received object yourself.
		*/
		World* GetWorld() noexcept;

		/*!
		Returns pointer to the most top window (parent/physical). You can always delete any window, in which
		case all child windows will also closed, but the remaining window objects must be deallocated manually.
		*/
		Window* GetTopWindow() noexcept;

		/*!
		Returns pointer to the top (physical) window GLFWwindow object. Do not delete the received object yourself.
		*/
		GLFWwindow* GetGLFWwindow()	const noexcept;

		/*!
		Sets the new window size.
		*/
		void SetWindowSize(Dimension<size_t> newSize) noexcept;

		/*!
		Sets the new window position.
		*/
		void SetPosition(Point<int> newPosition) noexcept override;

		/*!
		Changes the window position.
		*/
		void ChangePosition(Point<int> positionChange) noexcept override;

		/*!
		Sets the new window name/title.
		*/
		void SetWindowTitle(std::string newWindowTitle) noexcept;

		/*!
		Sets the constraints of the window.
		*/
		void SetMinMaxDimensions(int iMinWidth, int iMinHeight, int iMaxWidth, int iMaxHeight) noexcept override;

		/*!
		Sets the visibility of the window.
		*/
		void SetVisibility(bool mode) noexcept override;

		/*!
		Returns the value corresponding to the visibility of the window.
		*/
		bool GetVisibility() const noexcept override;

		/*!
		Returns the window name.
		*/
		std::string GetWindowName();

		/*!
		Sets callback function;
		*/
		void SetCallback(Callback callback) noexcept;

		/*!
		Resets callback.
		*/
		void ResetCallback() noexcept;

		/*!
		Sets new flags for the window.
		*/
		void SetFlags(int newFlags) noexcept override;

		/*!
		Activates listening for upcoming events.
		*/
		void EnableWidget() noexcept override;

		/*!
		Deactivates listening for upcoming events.
		*/
		void DisableWidget() noexcept override;

		/*!
		Adds a widget rendering event to the event list.
		*/
		void Render() noexcept override;

		/*!
		Allows to unregister internal event hook.
		*/
		void UnregisterEvent(EventType eventType) override;


	protected:

		/*!
		Initializes the window in "Parent" mode.
		*/
		void InitParent(const Widget2DProps& props) noexcept;

		/*!
		Initializes the window in "Child" mode.
		*/
		void InitChild(const Widget2DProps& props) noexcept;

		/*!
		Clears the memory of the window object. It also performs memory cleaning in sub windows. The function
		is secured against re-use.
		*/
		void Cleanup() noexcept;

		/*!
		Event handling mehod. Performs automatic viewport change and changes in the world's matrices. Only for
		window of 'Parent' type.
		*/
		EventHandle OnWindowResize(Event* event) noexcept;

		/*!
		Event handling mehod. Changes the internal state of "bIconify". Only for window of 'Parent' type.
		*/
		EventHandle OnWindowIconify(Event* event) noexcept;

		/*!
		Event handling mehod. Invoking Cleanup() method. Only for window of 'Parent' type.
		*/
		EventHandle OnWindowClose(Event* event) noexcept;

		/*!
		Event handling mehod. Updates variables that store the position of the window. Only for window of
		'Parent' type.
		*/
		EventHandle OnWindowMove(Event* event) noexcept;

		/*!
		Event handling mehod. Changes the internal state of "bFocus". Only for window of 'Parent' type.
		*/
		EventHandle OnWindowFocusChange(Event* event) noexcept;

		/*!
		Event handling mehod for window of "Child" type. Analyzes events and sends them to the child window
		to process (sometimes in an altered form).
		*/
		EventHandle SubWindowAlterEvent(Event* event) noexcept;

		/*Stores data regarding window states.*/
		WindowProps m_WindowProps;

		/*Indicates the window type (Parent or Child).*/
		WindowType m_WindowType;

		/*An pointer to the object that stores OpenGL shaders. Copied from the physical (top) window.*/
		ShaderManager* m_ShaderManager;

		/*An pointer to the object that stores VP matrices. Copied from the physical (top) window.*/
		World* m_World;

		/* An pointer to the callback function. */
		Callback m_Callback;

		/*List of iterators after internal binding.*/
		std::list<BindListIterator> m_InternalBindings;

		/*List of iterators after external binding (to the other windows/widgets).*/
		std::list<BindListIterator> m_ExternalBindings;

		/*Pointer for the "GLFWwindow" object of the GLFW library.*/
		GLFWwindow* m_GLFWWindow;

		/*Static variable to secure the re-initialization of the GLFW library.*/
		static bool s_GLFWInitialized;

		static int s_WindowsAmount;

		static std::thread s_t_WakeUper;


	};

}
