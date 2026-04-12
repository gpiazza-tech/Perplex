#include <Holloware.h>
#include <Holloware/Core/EntryPoint.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

class Sandbox : public Holloware::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{
	}
};

Holloware::Application* Holloware::CreateApplication() 
{
	return new Sandbox();
}