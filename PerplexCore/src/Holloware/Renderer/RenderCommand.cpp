#include <pch.h>
#include "RenderCommand.h"
#include "RendererAPI.h"

#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace Holloware
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}