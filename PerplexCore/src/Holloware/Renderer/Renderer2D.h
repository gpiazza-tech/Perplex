#pragma once

#include <Holloware/Core/Core.h>

#include <glm/fwd.hpp>
#include <cstdint>

namespace Holloware
{
	class Camera;
	class EditorCamera;
	class SubTexture2D;

	struct QuadProperties
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec2 Scale = glm::vec3(1.0f);
		glm::vec4 Color = glm::vec4(1.0f);
		float TilingFactor = 1.0f;

		QuadProperties(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
			: Position({ position.x, position.y, 0.0f }), Scale(scale), Color(color) {
		}
		QuadProperties(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
			: Position(position), Scale(scale), Color(color)  {
		}
		QuadProperties(const glm::vec2& position, const glm::vec2& scale)
			: Position({ position.x, position.y, 0.0f }), Scale(scale), Color(glm::vec4(1.0f)) {
		}
		QuadProperties(const glm::vec3& position, const glm::vec2& scale)
			: Position(position), Scale(scale), Color(glm::vec4(1.0f)) {
		}
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void Renderer2D::BeginScene(const EditorCamera& camera);
		
		static void EndScene();
		static void Flush();
		
		static void DrawQuad(const glm::mat4 transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4 transform, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint = glm::vec4(1.0f), int entityID = -1);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void StartBatch();
	};
}