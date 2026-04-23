#pragma once

#include "Components.h"
#include <pxr/pxr.h>

namespace Holloware
{
	class Entity;
	class EditorCamera;

	class SceneRenderer
	{
	public:
		static void Init(int width, int height, int pixelsPerUnit);
		static void Shutdown();

		static void BeginScene(const EditorCamera& editorCamera);
		static void BeginScene(const pxr::Camera& camera, const TransformComponent& cameraTransform, const glm::vec4& background);
		static void EndScene();

		static uint32_t GetMainFramebufferTexture();

		static void RenderSprite(const SpriteRendererComponent& src, const TransformComponent& tc);

		static void Resize(int width, int height);
	};
}