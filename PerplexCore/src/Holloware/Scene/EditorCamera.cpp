#include <pch.h>
#include "EditorCamera.h"

#include <Holloware/Core/Core.h>
#include <Holloware/Core/Input.h>
#include <Holloware/Core/Timestep.h>
#include <Holloware/Events/Event.h>
#include <Holloware/Events/MouseEvent.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Holloware
{
	EditorCamera::EditorCamera()
	{
		CalculateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsMouseButtonPressed(0))
		{
			m_Position -= glm::vec3(Input::GetMouseDelta().first, -Input::GetMouseDelta().second, 0.0f) * ts.GetSeconds() * 7.0f;
			CalculateView();
		}
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher = EventDispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(HW_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	inline bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Position.z -= e.GetYOffset();
		CalculateView();
		return false;
	}

	void EditorCamera::OnResize(float width, float height)
	{
		if (width == 0.0f || height == 0.0f) return;

		m_AspectRatio = width / height;
		CalculateProjection();
	}

	void EditorCamera::CalculateView()
	{
		m_View = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	}

	void EditorCamera::CalculateProjection()
	{
		m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	}
}