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
		Asset SpriteAsset{};
		glm::vec2 RectSize{};
		float CircleRadius{};
	};

	struct PerpixelShape
	{
		PerpixelShapeType Type{ PerpixelShapeType::Sprite };
		PerpixelShapeInfo Info{};
	};


}