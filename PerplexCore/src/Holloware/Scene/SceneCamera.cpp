#include <pch.h>
#include "SceneCamera.h"

#include <nlohmann/json.hpp>
#include <glm/ext/matrix_clip_space.inl>
#include <nlohmann/json_fwd.hpp>

#include <cstdint>

namespace Holloware
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}
	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
		else
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
	}

	void to_json(nlohmann::json& j, const SceneCamera& cam)
	{
		j["projectionType"] = cam.m_ProjectionType;

		j["orthographicSize"] = cam.m_OrthographicSize;
		j["orthographicNear"] = cam.m_OrthographicNear;
		j["orthographicFar"] = cam.m_OrthographicFar;

		j["perspectiveFOV"] = cam.m_PerspectiveFOV;
		j["perspectiveNear"] = cam.m_PerspectiveNear;
		j["perspectiveFar"] = cam.m_PerspectiveFar;

		j["aspectRatio"] = cam.m_AspectRatio;
	}

	void from_json(const nlohmann::json& j, SceneCamera& cam)
	{
		cam.m_ProjectionType = j["projectionType"].get<SceneCamera::ProjectionType>();

		cam.m_OrthographicSize = j["orthographicSize"].get<float>();
		cam.m_OrthographicNear = j["orthographicNear"].get<float>();
		cam.m_OrthographicFar = j["orthographicFar"].get<float>();

		cam.m_PerspectiveFOV = j["perspectiveFOV"].get<float>();
		cam.m_PerspectiveNear = j["perspectiveNear"].get<float>();
		cam.m_PerspectiveFar = j["perspectiveFar"].get<float>();

		cam.m_AspectRatio = j["aspectRatio"].get<float>();
	}
}