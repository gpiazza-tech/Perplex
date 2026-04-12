#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"
#include <Holloware/Core/Core.h>

#include <glm/fwd.hpp>

#include <cstdint>

namespace Holloware
{
	class OrthographicCamera;
	class Shader;

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transfor);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}