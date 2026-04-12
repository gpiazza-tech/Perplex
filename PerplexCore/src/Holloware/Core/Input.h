#pragma once

#include "Holloware/Core/Timestep.h"

#include <utility>

namespace Holloware
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static std::pair<float, float> GetMouseDelta();
		static float GetMouseX();
		static float GetMouseY();
	private:
		static void OnUpdate(Timestep ts); // Called from Application::Run()
	private:
		static float s_MousePosX;
		static float s_MousePosY;

		static float s_OldMousePosX;
		static float s_OldMousePosY;

		friend class Application;
	};
}