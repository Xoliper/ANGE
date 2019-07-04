#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "Ange/Core/Primitives.h"

namespace Ange {

	//-----------------------------------------------------------------------
	//Enums
	//-----------------------------------------------------------------------

	/*!
	It is responsible for selecting the matrices to use when rendering widgets / objects.
	*/
	enum class ContextType
	{
		GUI,			//Used to draw GUI objects such as the widgets provided by the library.
		Multipurpose	//Used to render 3D objects without affecting the GUI settings.
	};

	//-----------------------------------------------------------------------
	//Structs
	//-----------------------------------------------------------------------

	/*!
	Stores matrices and vectors that are useful when rendering widgets.
	*/
	struct WorldMatrices
	{
		/* Describes the location of the camera. */
		glm::vec3 v3CameraPosition	= glm::vec3(0.0f, 0.0f, 0.0f);

		/* Describes the observed location. */
		glm::vec3 v3LookAt			= glm::vec3(0.0f, 0.0f, 0.0f);

		/* Describes the projection matrix for MVP. */
		glm::mat4 m4Projection		= glm::mat4(1.0f);
		
		/* Describes the view matrix for MVP*/
		glm::mat4 m4View			= glm::mat4(1.0f);
	};

	//-----------------------------------------------------------------------

	/*!
	Stores the parameters of the camera.
	*/
	struct WorldCameraProps 
	{
		/* Describes the field of view. */
		float fFieldOfViev = 60.0f;

		/* Describes the distance of the close plan. */
		float fNear = 0.1f;
		
		/* Describes the distance of the far plan. */
		float fFar = 1000.0f;
	};

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	/*!
	Class responsible for setting the space and camera in the window.
	*/
	class World
	{
		friend class Window;
	public:
		
		/*!
		Default constructor.
		*/
		World();

		/*!
		Constructor with space field size setting.
		*/
		World(Dimension<size_t> boundary);

		/*!
		Copy constructor.
		*/
		World(const World & copy);
		
		/*!
		Destructor.
		*/
		virtual ~World();

		/*!
		Allows access to the world matrices.
		*/
		const WorldMatrices& GetGuiMatrices() const;

		/*!
		Returns the ratio of space field.
		*/
		const float GetRatio() const;

		/*!
		Allows you to change the size of the space.
		*/
		void Resize(Dimension<size_t> boundary);
		
		/*!
		Sets the GUI camera in the selected location.
		*/
		void SetGuiSpacePosition(Point<int> newPosition);
		
		/*!
		Changes the GUI location of the camera.
		*/
		void ChangeGuiSpacePosition(Point<int> positionChange);

	protected:

		/*!
		Calculates the ratio of space. Used when resizing window.
		*/
		float CalculateRatio();
		
		/*!
		Updates the projection matrix. Used when resizing window.
		*/
		void UpdateProjectionMatrix(ContextType contextType);
		
		/*!
		Updates the view matrix. Used when resizing window or changing space position.
		*/
		void UpdateViewMatrix(ContextType contextType);

		/* Stores matrices and vectors for the GUI space.*/
		WorldMatrices m_GuiMatrices;

		/* Stores camera settings for the GUI space.*/
		WorldCameraProps m_GuiCamProps;

		/* Space dimensions. */
		Dimension<size_t> m_Boundary;

		/* Ratio value of space dimensions. */
		float m_fRatio;
	};

}