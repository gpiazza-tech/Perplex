#pragma once

#include <Perplex/Scene/Entity.h>

#include <glm/fwd.hpp>

namespace Perplex
{
	struct HitInfo
	{
		Entity other;
		glm::vec2 point;
	};
}