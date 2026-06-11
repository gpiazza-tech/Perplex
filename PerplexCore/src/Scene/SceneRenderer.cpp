#include <Perplex/pch.h>
#include <Perplex/Scene/SceneRenderer.h>

#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Components.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Debug/Instrumentor.h>
#include <Perplex/Scene/EditorCamera.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Perpixel/PerpixelSystem.h>
#include <c/perplex_math.h>

#include <glm/glm.hpp>

namespace Perplex
{
	SceneRenderer::SceneRenderer(int width, int height, int pixelsPerUnit)
		: m_Width(width), m_Height(height), m_Framebuffer(width, height, true),
		m_Camera(glm::vec2(width, height), 16, 6.0f, pxr::ScalingMode::SmallerSide)
	{
		m_BloomRenderer.Init(width, height);
		m_Tonemapper.Init(width, height);
		m_Pixelator.Init(width, height);
	}

	void SceneRenderer::BeginScene(const EditorCamera& editorCamera)
	{
		HW_PROFILE_FUNCTION();

		m_Framebuffer.Bind();
		m_Camera = editorCamera;

		pxr::Renderer::BeginFrame({ 0.0f, 0.02f, 0.1f, 1.0f });
		pxr::Renderer::BeginBatch(editorCamera.GetViewProjection());
	}

	void SceneRenderer::BeginScene(const pxr::Camera& camera, const TransformComponent& cameraTransform, const glm::vec4& background)
	{
		HW_PROFILE_FUNCTION();

		m_Framebuffer.Bind();
		m_Camera = camera;

		glm::vec3 pixelPerfectPosition = pxr::MakePixelPerfect(cameraTransform.Position, camera.GetPixelsPerUnit());
		glm::mat4 pixelPerfectTransform = glm::translate(glm::mat4(1.0f), pixelPerfectPosition);

		pxr::Renderer::BeginFrame(background);
		pxr::Renderer::BeginBatch(camera.GetProjection() * glm::inverse(pixelPerfectTransform));
	}

	void SceneRenderer::RenderEditor(Ref<Scene> scene, const EditorCamera& camera)
	{
		BeginScene(camera);

		auto sprites = scene->View<SpriteRendererComponent>();
		for (auto handle : sprites)
		{
			Entity entity{ handle, scene.get() };
			auto& spriteRenderer = sprites.get<SpriteRendererComponent>(handle);
			if (entity.HasComponent<TransformComponent>())
				RenderSprite(spriteRenderer, entity.GetGlobalTransform());
		}

		auto perpixels = scene->View<PerpixelRendererComponent>();
		for (auto handle : perpixels)
		{
			Entity entity{ handle, scene.get() };
			auto& perpixelRenderer = perpixels.get<PerpixelRendererComponent>(handle);
			if (entity.HasComponent<TransformComponent>())
				RenderPerpixel(entity);
		}

		EndScene();
	}

	void SceneRenderer::Render(Ref<Scene> scene)
	{
		// Find Camera
		CameraComponent* mainCamera = nullptr;
		TransformComponent* cameraTransform = nullptr;
		{
			HW_PROFILE_SCOPE("Find Camera");

			auto view = scene->View<CameraComponent>();
			for (auto handle : view)
			{
				Entity entity{ handle, scene.get() };

				auto& camera = view.get<CameraComponent>(handle);

				if (entity.HasComponent<TransformComponent>() && camera.Primary)
				{
					mainCamera = &camera;
					cameraTransform = &entity.GetComponent<TransformComponent>();
					break;
				}
			}
		}

		// Render sprites
		if (mainCamera)
		{
			HW_PROFILE_SCOPE("Render Sprites");

			pxr::Camera camera({ m_Width, m_Height }, mainCamera->PixelsPerUnit, mainCamera->Zoom, mainCamera->ScalingMode);
			BeginScene(camera, *cameraTransform, mainCamera->Background);

			auto view = scene->View<SpriteRendererComponent>();
			for (auto handle : view)
			{
				Entity entity{ handle, scene.get() };
				auto& spriteRenderer = view.get<SpriteRendererComponent>(handle);
				if (entity.HasComponent<TransformComponent>())
					RenderSprite(spriteRenderer, entity.GetGlobalTransform());
			}

			// Perpixel Renderers
			auto perpixels = scene->View<PerpixelRendererComponent>();
			for (auto handle : perpixels)
			{
				Entity entity{ handle, scene.get() };
				auto& perpixelRenderer = perpixels.get<PerpixelRendererComponent>(handle);
				if (entity.HasComponent<TransformComponent>())
					RenderPerpixel(entity);
			}

			EndScene();
		}
	}

	void SceneRenderer::EndScene()
	{
		HW_PROFILE_FUNCTION();

		// Draw Scene
		pxr::Renderer::EndBatch();
		pxr::Renderer::Flush();

		// Renderer Post Processing
		static bool bloom = true;
		static float threshold = 1.0f;
		static float filterRadius = 0.003f;
		if (bloom)
		{
			m_BloomRenderer.RenderBloomTexture(m_Framebuffer.GetTextureID(), threshold, filterRadius);
			m_Framebuffer.DrawTexture(m_BloomRenderer.BloomTexture());
		}

		static bool tonemap = true;
		if (tonemap)
		{
			m_Tonemapper.RenderTonemap(m_Framebuffer.GetTextureID());
			m_Framebuffer.DrawTexture(m_Tonemapper.TonemappedTexture());
		}

		static bool pixelate = true;
		if (pixelate)
		{
			m_Pixelator.RenderPixelator(m_Framebuffer.GetTextureID(), m_Camera.GetPixelResolution());
			m_Framebuffer.DrawTexture(m_Pixelator.PixelatedTexture());
		}
		m_Framebuffer.DrawToScreen();
	}

	void SceneRenderer::RenderSprite(const SpriteRendererComponent& src, const TransformComponent& tc)
	{
		HW_PROFILE_FUNCTION();

		if (src.SpriteAsset)
		{
			Ref<const pxr::Sprite> sprite = src.SpriteAsset.GetData<pxr::Sprite>();
			Ref<const pxr::Sprite> emissionSprite = src.EmissionSpriteAsset.GetData<pxr::Sprite>();

			if (src.EmissionSpriteAsset)
				pxr::Renderer::DrawRotatedQuad(tc.Position, tc.Rotation, tc.Scale, *sprite.get(), *emissionSprite.get(), src.Color, src.Emission, true);
			else
				pxr::Renderer::DrawRotatedQuad(tc.Position, tc.Rotation, tc.Scale, *sprite.get(), *sprite.get(), src.Color, src.Emission, true);
		}
		else
		{
			pxr::Renderer::DrawQuad(tc.Position, tc.Scale, src.Color, src.Emission);
		}
	}

	void SceneRenderer::RenderPerpixel(Entity entity)
	{
		HW_PROFILE_FUNCTION();

		PerpixelSystem& system = entity.GetScene()->GetSystem<PerpixelSystem>();
		TransformComponent transform = entity.GetGlobalTransform();

		const std::vector<pixel>& pixels = system.GetPixels(entity.GetUUID());

		for (const auto& pxl : pixels)
			if (pxl.Lifetime > 0.0f)
				pxr::Renderer::DrawPixel(transform.Position + vector3(pxl.Position.x, pxl.Position.y, 0.0f), pxl.Color, pxl.Emission);
	}

	void SceneRenderer::Resize(int width, int height)
	{
		pxr::RenderCommands::ResizeViewport(width, height);

		m_Camera.Resize({ width, height });

		m_BloomRenderer.Resize(width, height);
		m_Tonemapper.Resize(width, height);
		m_Framebuffer.Resize(width, height);
		m_Pixelator.Resize(width, height);

		m_Width = width;
		m_Height = height;
	}

	uint32_t SceneRenderer::GetMainFramebufferTexture()
	{
		return m_Framebuffer.GetTextureID();
	}

	void SceneRenderer::DrawToScreen()
	{
		HW_PROFILE_FUNCTION();

		m_Framebuffer.DrawToScreen();
	}
}