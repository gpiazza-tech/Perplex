#include <pch.h>
#include "OpenGLContext.h"

#include <Holloware/Core/Core.h>
#include <Holloware/Debug/Instrumentor.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Holloware
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HW_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		HW_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HW_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLContext::SwapBuffers()
	{
		HW_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}