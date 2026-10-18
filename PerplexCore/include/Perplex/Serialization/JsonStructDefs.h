#pragma once

#include <c/perplex_math.h>
#include <glm/fwd.hpp>

#include "JsonHelper.h"

PERPLEX_DEFINE_JSON_STRUCT(Bounds, Center, Extents)
PERPLEX_DEFINE_JSON_STRUCT(Radius, CenterX, CenterY, Radius)

namespace glm
{
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec2, x, y)
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec3, x, y, z)
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec4, r, g, b, a)
}