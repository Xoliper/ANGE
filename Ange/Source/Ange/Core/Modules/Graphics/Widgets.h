#pragma once
#include "Apch.h"
#define GLM_ENABLE_EXPERIMENTAL

#include "Ange/Core/Primitives.h"
#include "Ange/Core/Properties.h"
#include "Ange/Core/Event.h"
#include "Ange/Core/Modules/Logger.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <map>

namespace Ange {

	/*
		______                  _____      _               _ _
		| ___ \                |_   _|    | |             (_) |
		| |_/ / __ _ ___  ___    | | _ __ | |__   ___ _ __ _| |_ __ _ _ __   ___ ___
		| ___ \/ _` / __|/ _ \   | || '_ \| '_ \ / _ \ '__| | __/ _` | '_ \ / __/ _ \
		| |_/ / (_| \__ \  __/  _| || | | | | | |  __/ |  | | || (_| | | | | (_|  __/
		\____/ \__,_|___/\___|  \___/_| |_|_| |_|\___|_|  |_|\__\__,_|_| |_|\___\___|


		+-------------------------------------------------------------------------+
		|  Basic Tools  |  Base Widget Class  |  Class For Simple Widgets         |
		+---------------|---------------------|-----------------------------------+
		|  Resizable2D  |  Widget2D           |  BasicWidget2D                    |
		|    (prop)  ----->                 ----->                                |
		+-------------------------------------------------------------------------+



		 _    _ _     _            _         _____      _               _ _
		| |  | (_)   | |          | |       |_   _|    | |             (_) |
		| |  | |_  __| | __ _  ___| |_ ___    | | _ __ | |__   ___ _ __ _| |_ __ _ _ __   ___ ___
		| |/\| | |/ _` |/ _` |/ _ \ __/ __|   | || '_ \| '_ \ / _ \ '__| | __/ _` | '_ \ / __/ _ \
		\  /\  / | (_| | (_| |  __/ |_\__ \  _| || | | | | | |  __/ |  | | || (_| | | | | (_|  __/
		 \/  \/|_|\__,_|\__, |\___|\__|___/  \___/_| |_|_| |_|\___|_|  |_|\__\__,_|_| |_|\___\___|
						 __/ |
						|___/

		+-------------------------------------------------------------------------+
		|  Derived from BasicWidget2D |  Derived form Widget2D                    |
		+-----------------------------|-------------------------------------------+
		|  Background / Rectangle2D   |  SimpleButton         VerticalScroller    |
		|                             |                                           |
		|  Image                      |  SimpleInput                              |
		|                             |                                           |
		|  Text                       |  CustomWidget                             |
		+-------------------------------------------------------------------------+
	*/



	//------------------------------------------------------------------------------------------------------
	//Forward declarations
	//------------------------------------------------------------------------------------------------------

	class Window;
	class Background;
	class Image;

	//------------------------------------------------------------------------------------------------------
	//Enums
	//------------------------------------------------------------------------------------------------------

	/*!
	Determines how to treat the "Position" parameter when determining the position of the widget.
	For example, the "Left | Bottom" flag means that the widget will be drawn to the right and up
	because the widget's anchor point is on the bottom left.
	*/
	enum Anchor
	{
		Left				= 4,
		HorizontalCenter	= 8,
		Right				= 16,
		Bottom				= 32,
		VerticalCenter		= 64,
		Top					= 128
	};

	/*!
	Determines how the widget should change its size when the parent window resizes.
	*/
	enum ResizePolicy
	{
		AutoFill			= 256,	/* The widget will fill the window space. */
	};

	/*!
	Define how the "Image" object is displayed.
	*/
	enum ImageFlags
	{
		DetectSize			= 512,	/*Automatically changes the size of the widget to that corresponding to the size of the texture used.*/
		Repeat				= 1024, /*The used texture will be repeated throughout the entire dimension of the widget.*/
	};

	/*!
	Configures the behavior of the "Text" widget.
	*/
	enum TextFlags
	{
		EnableNewlineChar	= 2048,
		Multiline			= 4096,
		AutoScale			= 8192	/*Reserved.*/
	};

	/*!
	Defines the type of widget.
	*/
	enum class WidgetType
	{
		Nullptr,Base,
		Window, Background, Image, Custom,
		Text, Button, Input, Textarea,
		Scroller, Ratio, Checkbox
	};

	//------------------------------------------------------------------------------------------------------
	//Common Structs
	//------------------------------------------------------------------------------------------------------

	/*!
	It contains all matrices and vectors used to display widgets in a specific place, with specific
	transformations.
	*/
	struct Matrices
	{
		/*!
		Default constructor.
		*/
		Matrices();

		/*!
		Copy constructor.
		*/
		Matrices(const Matrices& copy);

		/*!
		Destructor.
		*/
		~Matrices();

		/*Stores the widget shift caused by the anchor setting.*/
		glm::vec3 v3AnchorTranslation;

		/*Stores the widget's translation matrix.*/
		glm::mat4 m4Translation;

		/*Stores the widget's rotation matrix.*/
		glm::mat4 m4Rotation;

		/*Stores the widget's model matrix which is based on Translation, Rotation and Scale matrices.*/
		glm::mat4 m4Model;

		/*Stores the widget's scale matrix.*/
		glm::mat4 m4Scale;

		/*Represents the current MVP matrix for the widget.*/
		glm::mat4 m4Mvp;
	};

	/*!
	A structure containing basic data about the widget.
	*/
	struct Widget2DProps
	{
		/*!
		Default constructor with default parameters. Sets the position and size of the widget.
		*/
		Widget2DProps(
			Point<int> position = { 0,0 },
			Dimension<size_t> dimension = { 100,100 },
			int flags = Anchor::Bottom | Anchor::Left
		);

		/*!
		Copy constructor.
		*/
		Widget2DProps(const Widget2DProps& copy);

		/*!
		Destructor.
		*/
		~Widget2DProps();

		/*Stores the current position of the widget.*/
		Point<int>	Position;

		/*Stores the current dimenions of the widget.*/
		Dimension<size_t> Dimensions;

		/*Stores widget flags.*/
		int	iFlags;

		/*Defines whether the widget should be displayed or not.*/
		bool bVisibility;

		/*An helper variable describing whether in the previous tick some parameters changes occurred.*/
		bool bIfChanged;

		/*Stores information about whether the widget is on or off.*/
		bool bDisabled;

		/*Stores widget ID. The identifier is used in all classes that are derrives of ComplexWidgets class.*/
		int	iId;
	};

	//------------------------------------------------------------------------------------------------------
	//Base Classes
	//------------------------------------------------------------------------------------------------------

	/*!
	The widget interface, the lowest level used by complex high-level widgets. It essentially implements
	only the basic Set and Get methods.
	*/
	class Widget2D : public Resizable2D
	{
	public:

		/*!
		Test constructor.
		*/
		Widget2D();

		/*!
		Default constructor.
		*/
		Widget2D(Window* parent, const Widget2DProps& props);

		/*!
		Copy constructor.
		*/
		Widget2D(const Widget2D& copy);

		/*!
		Destructor.
		*/
		~Widget2D();

		/*!
		Swaps data between objects.
		*/
		friend void swap(Widget2D& first, Widget2D& second) noexcept;

		/*!
		Sets the widget in a specific position. The function must be implemented in the inheriting class.
		*/
		virtual void SetPosition(Point<int> newPosition) = 0;

		/*!
		Changes the position of the widget relative to the current one. The function must be implemented
		in an inheriting class.
		*/
		virtual void ChangePosition(Point<int> positionChange) = 0;

		/*!
		Sets the visibility of the widget. The function must be implemented in an inheriting class.
		*/
		virtual void SetVisibility(bool mode) = 0;

		/*!
		Returns the visibility of the widget. The function must be implemented in an inheriting class.
		*/
		virtual bool GetVisibility() const = 0;

		/*!
		Activates the widget's operation by registering for specific events to the parent window.
		The function must be implemented in an inheriting class.
		*/
		virtual void EnableWidget() = 0;

		/*!
		Deactivates the widget by unregistering from the parent window. Note - re-registration may
		change the z-order of the widget on rendering stack. The function must be implemented in an
		inheriting class.
		*/
		virtual void DisableWidget() = 0;

		/*!
		Renders the widget. The function must be implemented in an inheriting class.
		*/
		virtual void Render() = 0;

		/*!
		Allows to unregister the widget from a specific event in the parent window.
		*/
		virtual void UnregisterEvent(EventType eventType);

		/*!
		Sets the Id for the widget.
		*/
		virtual void SetWidgetId(int id);

		/*!
		Returns the widget Id.
		*/
		virtual int GetWidgetId() const;

		/*!
		Sets the widget flags.
		*/
		virtual void SetFlags(int newFlags);

		/*!
		Returns the widget flags.
		*/
		int GetFlags() const;

		/*!
		Returns the widget type.
		*/
		WidgetType GetWidgetType();

		/*!
		Returns the widget parent. If the parent is the top-level window, then returns nullptr.
		*/
		Window* GetParentWindow() const;

		/*!
		Returns the widget current dimentions.
		*/
		virtual const Dimension<size_t> GetDimension()	const;

		/*!
		Returns the widget current position.
		*/
		virtual const Point<int> GetPosition() const;

		/*!
		Returns the widget information as an Widget2DProps reference.
		*/
		virtual const Widget2DProps & GetWidget2DProps() const;

	protected:

		/*!
		It reduces the position of the point relative to the position of the widget.
		This allows to check if the external point is inside the widget.
		*/
		void AlterCoords(Point<int> & point);

		/*!
		Checks if the point is in the widget's space.
		*/
		bool CheckCoords(Point<int> & point);

		/* Stores basic data about the widget. */
		Widget2DProps m_Widget2DProps;

		/* Stores the widget type. */
		WidgetType m_WidgetType;

		/* Stores the pointer on the parent. */
		Window* m_ParentWindow;

		/* Stores iterators for registered events.*/
		std::list<BindListIterator> m_Bindings;

	};


	/*!
	Widget interface, medium level used by basic widgets. Implements several
	functions from the Widget2D base class.
	*/
	class BasicWidget2D : public Widget2D
	{
	public:

		/*!
		Default constructor.
		*/
		BasicWidget2D(Window* parent, const Widget2DProps& props);

		/*!
		Copy constructor.
		*/
		BasicWidget2D(const BasicWidget2D & copy);

		/*!
		Destructor.
		*/
		virtual ~BasicWidget2D();

		/*!
		Swaps data between objects.
		*/
		friend void swap(BasicWidget2D& first, BasicWidget2D& second) noexcept;


		/*!
		Renders the widget. The function must be implemented in an inheriting class.
		*/
		virtual void Render() override = 0;

		/*!
		Sets the widget in a specific position.
		*/
		void SetPosition(Point<int> newPosition) override;

		/*!
		Changes the position of the widget relative to the current one.
		*/
		void ChangePosition(Point<int> positionChange) override;

		/*!
		Sets the new widget size.
		*/
		void Resize(Dimension<size_t> newSize);

		/*!
		Changes the size of the widget.
		*/
		void ChangeSize(Dimension<size_t> sizeChange);

		/*!
		Sets the new widget rotation.
		*/
		void SetRotation(float newAngle);

		/*!
		Changes the rotation of the widget.
		*/
		void ChangeRotation(float angleChange);

		/*!
		Sets the new widget scale.
		*/
		void SetScale(Scale<float> newScale);

		/*!
		Changes the scale of the widget.
		*/
		void ChangeScale(Scale<float> scaleChange);

		/*!
		Sets the visibility of the widget.
		*/
		void SetVisibility(bool mode) override;

		/*!
		Returns the visibility of the widget.
		*/
		bool GetVisibility() const override;

		/*!
		Sets the widget flags.
		*/
		void SetFlags(int newFlags) override;

		/*!
		Activates the widget's operation by registering for specific events to the parent window.
		*/
		void EnableWidget() override;

		/*!
		Deactivates the widget by unregistering from the parent window. Note - re-registration may
		change the z-order of the widget on rendering stack.
		*/
		void DisableWidget() override;

	protected:

		/*!
		This function should bind the filled-in data arrays that are later used in the rendering process.
		This is the place where the vertex or uv arrays etc. should be bound. This function is used in
		the process of resizing the widget. The function must be implemented in an inheriting class.
		*/
		virtual void BindBuffers() = 0;

		/*!
		WindowResize event handler.
		*/
		virtual bool OnWindowResize(Event* ev);

		/*!
		DrawableInvokeRender event handler. Basicly invokes Render().
		*/
		virtual bool OnDrawableInvokeRender(Event* ev);

		/*!
		WindowClose event handler. Basicly invokes DisalbeWidget().
		*/
		virtual bool OnWindowClose(Event* ev);

		/*!
		Gets the pointer to the requested shader data and sets the widget to use it.
		*/
		void UseShader(const char* shaderName);

		/*!
		Calculates the model and MVP matrix. It is usually invoked before the rendering starts. The model
		matrix is recalculated only when the m_bChange flag from m_Widget2DProps is true.
		*/
		virtual void RecalculateMatrices();

		/*!
		Based on the m_iFlags from m_Widget2DProps, calculates the widget's shift vector.
		*/
		virtual void CalculateAnchorVec();

		/*Stores matrices describing mathematical transformations on the widget. */
		Matrices m_Matrices;

		/*Stores data about the used shader.*/
		std::map<const char*, GLuint>* m_UsedShader;

		/*Stores the angle by which the widget is rotated.*/
		float m_fRotationAngle;
	};

	/*!
	Stores and gives easy access to one of the variants of the widgets.
	*/
	union WidgetVariant
	{
		Widget2D* m_Accessor;
		Background* m_Background;
		Image* m_Image;
	};

	/*!
	Implements the WidgetVariant union in that way to make it easier to use and manage non-trivial variables.
	*/
	class FrontWidget {
	public:

		/*!
		Default constructor. Sets internals to nullptr.
		*/
		FrontWidget();

		/*!
		Second constructor.
		*/
		FrontWidget(WidgetType baseWidgetType, Widget2D* realWidget);
		
		/*!
		Copy constructor.
		*/
		FrontWidget(const FrontWidget& copy);
		
		/*!
		Destructor.
		*/
		~FrontWidget();

		/*!
		Assignment operator.
		*/
		FrontWidget& operator=(FrontWidget rhs);

		/*!
		Swap function.
		*/
		friend void swap(FrontWidget& first, FrontWidget& second) noexcept;

		/*!
		Sets the public	class fields.
		*/
		void SetWidget(WidgetType baseWidgetType, Widget2D* realWidget);

		/* Describes the type of stored widget. */
		WidgetType m_Type;

		/* Stores the real widget. */
		WidgetVariant m_Variant;

	private:

		/*!
		Clears the class by deleting the stored object.
		*/
		void Cleanup();
	};

}
