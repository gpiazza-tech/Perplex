#pragma once

#include <Perplex/Core/Timestep.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <utility>

namespace Perplex
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static glm::ivec2 GetMousePixelPosition();
		static glm::vec2 GetMousePosition();
		static glm::vec2 GetMouseDelta();
		static float GetMouseX();
		static float GetMouseY();

		static void SetMouseWorldPosition(glm::vec2 mouseWorldPos) { s_MouseWorldPos = mouseWorldPos; }
		static glm::vec2 GetMouseWorldPosition() { return s_MouseWorldPos; }
	private:
		static void OnUpdate(Timestep ts); // Called from Application::Run()
	private:
		static glm::vec2 s_MousePos;
		static glm::vec2 s_OldMousePos;

		static glm::vec2 s_MouseWorldPos;

		friend class Application;
	};
}