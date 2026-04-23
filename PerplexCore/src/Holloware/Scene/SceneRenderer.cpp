#include <pch.h>
#include "SceneRenderer.h"

#include <Holloware/Scene/EditorCamera.h>
#include <Holloware/Core/Core.h>
#include "Components.h"
#include "Entity.h"
#include <Holloware/Assets/Asset.h>
#include <pxr/pxr.h>

#include <glm.hpp>

namespace Holloware
{
	static int s_Width;
	static int s_Height;

	static pxr::Framebuffer* s_Framebuffer;

	static pxr::BloomRenderer s_BloomRenderer;
	static pxr::Tonemapper s_Tonemapper;
	static pxr::Pixelator s_Pixelator;

	static pxr::Camera s_Camera = pxr::Camera(glm::vec2(s_Width, s_Height), 16, 6.0f, pxr::ScalingMode::SmallerSide);

	void SceneRenderer::Init(int width, int height, int pixelsPerUnit)
	{
		pxr::Renderer::Init(pixelsPerUnit);
		s_Framebuffer = new pxr::Framebuffer(width, height, true);

		s_BloomRenderer.Init(width, height);
		s_Tonemapper.Init(width, height);
		s_Pixelator.Init(width, height);

		s_Camera = pxr::Camera(glm::vec2(s_Width, s_Height), 16, 6.0f, pxr::ScalingMode::SmallerSide);

		s_Width = width;
		s_Height = height;
	}

	void SceneRenderer::Shutdown()
	{
		delete s_Framebuffer;
		s_BloomRenderer.Destroy();
		s_Tonemapper.Destroy();
		s_Pixelator.Destroy();
		pxr::Renderer::Shutdown();
	}

	void SceneRenderer::BeginScene(const EditorCamera& editorCamera)
	{
		s_Framebuffer->Bind();
		s_Camera = editorCamera;

		pxr::Renderer::BeginFrame({ 0.0f, 0.02f, 0.1f, 1.0f });
		pxr::Renderer::BeginBatch(editorCamera.GetViewProjection());
	}

	void SceneRenderer::BeginScene(const pxr::Camera& camera, const TransformComponent& cameraTransform, const glm::vec4& background)
	{
		s_Framebuffer->Bind();
		s_Camera = camera;

		glm::vec3 pixelPerfectPosition = pxr::MakePixelPerfect(cameraTransform.Position, camera.GetPixelsPerUnit());
		glm::mat4 pixelPerfectTransform = glm::translate(glm::mat4(1.0f), pixelPerfectPosition);

		pxr::Renderer::BeginFrame(background);
		pxr::Renderer::BeginBatch(camera.GetProjection() * glm::inverse(pixelPerfectTransform));
	}

	void SceneRenderer::EndScene()
	{
		// Draw Scene
		pxr::Renderer::EndBatch();
		pxr::Renderer::Flush();

		// Renderer Post Processing
		static bool bloom = true;
		static float threshold = 1.0f;
		static float filterRadius = 0.003f;
		if (bloom)
		{
			s_BloomRenderer.RenderBloomTexture(s_Framebuffer->GetTextureID(), threshold, filterRadius);
			s_Framebuffer->DrawTexture(s_BloomRenderer.BloomTexture());
		}

		static bool tonemap = true;
		if (tonemap)
		{
			s_Tonemapper.RenderTonemap(s_Framebuffer->GetTextureID());
			s_Framebuffer->DrawTexture(s_Tonemapper.TonemappedTexture());
		}

		static bool pixelate = true;
		if (pixelate)
		{
			s_Pixelator.RenderPixelator(s_Framebuffer->GetTextureID(), s_Camera.GetPixelResolution());
			s_Framebuffer->DrawTexture(s_Pixelator.PixelatedTexture());
		}
		s_Framebuffer->DrawToScreen();
	}

	void SceneRenderer::RenderSprite(const SpriteRendererComponent& src, const TransformComponent& tc)
	{
		if (src.SpriteAsset)
		{
			Ref<const pxr::Sprite> sprite = src.SpriteAsset.GetData<pxr::Sprite>();
			Ref<const pxr::Sprite> emissionSprite = src.EmissionSpriteAsset.GetData<pxr::Sprite>();

			if (src.EmissionSpriteAsset)
				pxr::Renderer::DrawQuad(tc.Position, tc.Scale, *sprite.get(), *emissionSprite.get(), src.Color, src.Emission, true);
			else
				pxr::Renderer::DrawQuad(tc.Position, tc.Scale, *sprite.get(), *sprite.get(), src.Color, src.Emission, true);
		}
		else
		{
			pxr::Renderer::DrawQuad(tc.Position, tc.Scale, src.Color, src.Emission);
		}
	}

	void SceneRenderer::Resize(int width, int height)
	{
		pxr::RenderCommands::ResizeViewport(width, height);

		s_Camera.Resize({ width, height });

		s_BloomRenderer.Resize(width, height);
		s_Tonemapper.Resize(width, height);
		s_Framebuffer->Resize(width, height);
		s_Pixelator.Resize(width, height);

		s_Width = width;
		s_Height = height;
	}

	uint32_t SceneRenderer::GetMainFramebufferTexture()
	{
		return s_Framebuffer->GetTextureID();
	}
}