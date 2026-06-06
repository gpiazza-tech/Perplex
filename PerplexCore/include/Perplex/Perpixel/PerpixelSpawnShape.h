#pragma once

#include <Perplex/Assets/Asset.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace Perplex
{
	enum class PerpixelShapeType
	{
		None = 0,
		Sprite,
		Rect,
		Circle
	};

	struct PerpixelShapeInfo
	{
		Asset ColorAsset{};
		Asset EmissionAsset{};
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Emission = 1.0f;

		glm::vec2 RectSize{};
		float CircleRadius{};
	};

	struct PerpixelShape
	{
		PerpixelShapeType Type{ PerpixelShapeType::Sprite };
		PerpixelShapeInfo Info{};
	};


}