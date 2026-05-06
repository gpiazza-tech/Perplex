#pragma once

#include <Holloware/Assets/Asset.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace Holloware
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
		PerpixelShapeType Type{ PerpixelShapeType::None };
		PerpixelShapeInfo Info{};
	};


}