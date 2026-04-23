#include <pch.h>
#include "EditorCamera.h"

#include <Holloware/Core/Core.h>
#include <Holloware/Core/Input.h>
#include <Holloware/Core/Timestep.h>
#include <Holloware/Events/Event.h>
#include <Holloware/Events/MouseEvent.h>
#include <pxr/pxr.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Holloware
{
	EditorCamera::EditorCamera()
	{
		SetZoom(m_Position.z);
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
		SetZoom(m_Position.z);
		CalculateView();
		return false;
	}

	void EditorCamera::OnResize(float width, float height)
	{
		Resize({ width, height });
	}

	void EditorCamera::CalculateView()
	{
		m_View = glm::inverse(glm::translate(glm::mat4(1.0f), { m_Position.x, m_Position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
	}

	const glm::mat4& EditorCamera::GetViewProjection() const 
	{
		return GetProjection() * m_View;
	}
}