#pragma once

#include "Shader.h"
#include "VertexArray.h"

#include <Holloware/Core/Core.h>

#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <memory>

namespace Holloware
{
	class GameObject
	{
	public:
		GameObject() {}
		GameObject(glm::mat4x4 modelMatrix, std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray);

		inline const glm::mat4x4 GetModelMatrix() const { return m_ModelMatrix; }
		inline const Ref<Shader> GetShader() const { return m_Shader; }
		inline const Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }

		const void SetPosition(glm::vec3 pos) { m_ModelMatrix = glm::translate(glm::mat4(1.0f), pos); }
	private:
		glm::mat4x4 m_ModelMatrix;
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
	};
}