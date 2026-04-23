#pragma once

#include <Holloware/Core/Timestep.h>
#include <rendering/Camera.h>

#include <glm/fwd.hpp>

namespace Holloware
{
	class Event;
	class MouseScrolledEvent;

	class EditorCamera : public pxr::Camera
	{
	public:
		EditorCamera();
		~EditorCamera() = default;

		void OnUpdate(Timestep ts);

		void OnEvent(Event& event);
		inline bool OnMouseScrolled(MouseScrolledEvent& e);
		void OnResize(float width, float height);

		const glm::mat4& GetViewProjection() const;
	private:
		void CalculateView();
	private:
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 20.0f);
		float m_PanSpeed = 5.0f;
	};
}