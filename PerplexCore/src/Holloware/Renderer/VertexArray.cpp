#include <pch.h>
#include "VertexArray.h"

#include "Renderer.h"
#include "RendererAPI.h" 
#include <Holloware/Core/Core.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

#include <memory>

namespace Holloware
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: HW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		HW_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
