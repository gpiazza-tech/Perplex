#include <pch.h>
#include <Perplex/Core/Input.h>

#include <Perplex/Core/Window.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Core/Timestep.h>

#include <GLFW/glfw3.h>

namespace Perplex
{
	float Input::s_MousePosX = 0;
	float Input::s_MousePosY = 0;
		  
	float Input::s_OldMousePosX = 0;
	float Input::s_OldMousePosY = 0;

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	std::pair<float, float> Input::GetMouseDelta()
	{
		float mouseDeltaX = s_MousePosX - s_OldMousePosX;
		float mouseDeltaY = s_MousePosY - s_OldMousePosY;
		return { mouseDeltaX, mouseDeltaY };
	}

	float Input::GetMouseX()
	{
		return s_MousePosX;
	}

	float Input::GetMouseY()
	{
		return s_MousePosY;
	}

	void Input::OnUpdate(Timestep ts)
	{
		auto [x, y] = GetMousePosition();

		s_OldMousePosX = s_MousePosX;
		s_OldMousePosY = s_MousePosY;

		s_MousePosX = x;
		s_MousePosY = y;
	}
}