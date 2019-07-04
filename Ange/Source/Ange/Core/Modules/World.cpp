#include "Apch.h"
#include "World.h"
#include "Ange/Core/Modules/Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Ange {

	World::World()
	{
	}

	World::World(Dimension<size_t> boundary) {
		Resize(boundary);
	}

	World::World(const World & w) {
		 m_GuiMatrices = w.m_GuiMatrices;
		 m_GuiCamProps = w.m_GuiCamProps;
		 m_Boundary = w.m_Boundary;
		 m_fRatio = w.m_fRatio;
	}

	World::~World() {
	}

	const WorldMatrices& World::GetGuiMatrices() const
	{
		return m_GuiMatrices;
	};

	const float World::GetRatio() const
	{
		return m_fRatio;
	}


	float World::CalculateRatio()
	{
		return static_cast<float>(m_Boundary.tWidth) / m_Boundary.tHeight;
	}

	void World::UpdateProjectionMatrix(ContextType contextType)
	{
		if (contextType == ContextType::GUI) {
			m_GuiMatrices.m4Projection = glm::perspective<float>(glm::radians(60.0f), (float)m_fRatio, m_GuiCamProps.fNear, (float)m_Boundary.tHeight*2);
		}
	}

	void World::UpdateViewMatrix(ContextType contextType)
	{
		if (contextType == ContextType::GUI) {
			m_GuiMatrices.m4View = glm::lookAt<float>(
				glm::vec3(m_GuiMatrices.v3CameraPosition.x, m_GuiMatrices.v3CameraPosition.y, ((float)m_Boundary.tHeight*2)*sqrt(3)/2),// Camera is at (0,0,z), in World Space
				glm::vec3(m_GuiMatrices.v3LookAt.x, m_GuiMatrices.v3LookAt.y,0), 						// and looks at the origin //x y 0 + camera direction
				glm::vec3(0, 1, 0)  					// Head is up (set to 0,-1,0 to look upside-down)
				);
		}
	}

	void World::Resize(Dimension<size_t> boundary)
	{
		if (boundary.tHeight == 0) {
			ANGE_WARNING("Any dimension of World cannot be '0'! [%ux%u]", boundary.tWidth, boundary.tHeight);
			return;
		}
		m_Boundary = boundary;
		m_fRatio = CalculateRatio();
		if (m_Boundary.tWidth % 2 == 1) m_Boundary.tWidth += 1;
		if (m_Boundary.tHeight % 2 == 1) m_Boundary.tHeight += 1;
		SetGuiSpacePosition({ (int)((float)m_Boundary.tWidth / 2), (int)((float)m_Boundary.tHeight / 2) });
		m_Boundary = boundary;
		UpdateProjectionMatrix(ContextType::GUI);
		UpdateViewMatrix(ContextType::GUI);
	}

	void World::SetGuiSpacePosition(Point<int> newPosition)
	{
		m_GuiMatrices.v3CameraPosition.x =	(float)newPosition.tX * 2;
		m_GuiMatrices.v3CameraPosition.y =	(float)newPosition.tY * 2;
		m_GuiMatrices.v3LookAt.x =			(float)newPosition.tX * 2;
		m_GuiMatrices.v3LookAt.y =			(float)newPosition.tY * 2;
		UpdateViewMatrix(ContextType::GUI);
	}
	
	void World::ChangeGuiSpacePosition(Point<int> positionChange)
	{
		m_GuiMatrices.v3CameraPosition.x += (float)positionChange.tX * 2;
		m_GuiMatrices.v3CameraPosition.y += (float)positionChange.tY * 2;
		m_GuiMatrices.v3LookAt.x +=			(float)positionChange.tX * 2;
		m_GuiMatrices.v3LookAt.y +=			(float)positionChange.tY * 2;
		UpdateViewMatrix(ContextType::GUI);
	}

}