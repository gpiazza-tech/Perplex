#pragma once

#ifdef cplusplus
#include <glm/glm.hpp>
using vector2 = glm::vec2;
using vector3 = glm::vec3;
using vector4 = glm::vec4;
#else
struct vector2
{
	union
	{
		struct { float x, y; };
		struct { float r, g; };
	};
};

struct vector3
{
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
};

struct vector4
{
	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
	};
};
#endif