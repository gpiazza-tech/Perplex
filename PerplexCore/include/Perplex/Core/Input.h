#pragma once

#include <Perplex/Core/Timestep.h>

#include <utility>

namespace Perplex
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<int, int> GetMousePixelPosition();
		static std::pair<float, float> GetMousePosition();
		static std::pair<float, float> GetMouseDelta();
		static float GetMouseX();
		static float GetMouseY();

		static void SetMouseWorldPosition(std::pair<float, float> mouseWorldPosition)
		{
			s_MouseWorldPosX = mouseWorldPosition.first;
			s_MouseWorldPosY = mouseWorldPosition.second;
		}

		static std::pair<float, float> GetMouseWorldPosition()
		{
			return { s_MouseWorldPosX, s_MouseWorldPosY };
		}
	private:
		static void OnUpdate(Timestep ts); // Called from Application::Run()
	private:
		static float s_MousePosX;
		static float s_MousePosY;

		static float s_OldMousePosX;
		static float s_OldMousePosY;

		static float s_MouseWorldPosX;
		static float s_MouseWorldPosY;

		friend class Application;
	};
}