#include "pch.h"
#include "Framebuffer.h"

#include "Holloware/Core/Core.h"
#include "Holloware/Renderer/RendererAPI.h"
#include "Holloware/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Holloware
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: HW_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		HW_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}