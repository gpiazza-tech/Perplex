#include <Perplex/pch.h>
#include <Perplex/Core/Input.h>

#include <Perplex/Core/Window.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Core/Timestep.h>

#include <GLFW/glfw3.h>

namespace Perplex
{
	glm::vec2 Input::s_MousePos{};
	glm::vec2 Input::s_OldMousePos{};

	glm::vec2 Input::s_MouseWorldPos{};

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

	glm::ivec2 Input::GetMousePixelPosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		return glm::ivec2{ static_cast<int>(xpos), height - static_cast<int>(ypos) };
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		ypos = height - ypos;
		xpos = xpos / width * 2.0 - 1.0;
		ypos = ypos / height * 2.0 - 1.0;

		return glm::vec2{ static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	glm::vec2 Input::GetMouseDelta()
	{
		return s_MousePos - s_OldMousePos;
	}

	float Input::GetMouseX()
	{
		return s_MousePos.x;
	}

	float Input::GetMouseY()
	{
		return s_MousePos.y;
	}

	void Input::OnUpdate(Timestep ts)
	{
		glm::vec2 pos = GetMousePosition();

		s_OldMousePos = s_MousePos;
		s_MousePos = pos;
	}
}