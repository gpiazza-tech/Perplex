#pragma once

#include <Perplex/Components/Components.h>
#include <pxr/pxr.h>

namespace Perplex
{
	class Entity;
	class EditorCamera;
	class Scene;

	class SceneRenderer
	{
	public:
		SceneRenderer(int width, int height, int pixelsPerUnit);

		void BeginScene(const EditorCamera& editorCamera);
		void BeginScene(const pxr::Camera& camera, const TransformComponent& cameraTransform, const glm::vec4& background);
		void EndScene();

		void RenderEditor(Ref<Scene> scene, const EditorCamera& camera);
		void Render(Ref<Scene> scene);

		uint32_t GetMainFramebufferTexture();
		void DrawToScreen();

		void RenderSprite(const SpriteRendererComponent& src, const TransformComponent& tc);
		void RenderPerpixel(Entity entity);
		void RenderText(Entity entity);

		void Resize(int width, int height);

		glm::vec2 ScreenToWorldPosition(const glm::vec2& screenPosition);
	private:
		int m_Width{ 1 };
		int m_Height{ 1 };

		pxr::Framebuffer m_Framebuffer;

		pxr::BloomRenderer m_BloomRenderer{};
		pxr::Tonemapper m_Tonemapper{};
		pxr::Pixelator m_Pixelator{};

		pxr::Camera m_Camera;
	};
}