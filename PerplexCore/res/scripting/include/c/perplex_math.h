#pragma once

#ifdef __cplusplus
#include <glm/fwd.hpp>
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

typedef struct
{
	union
	{
		struct { float x, y; };
		struct { float r, g; };
	};
} Vec2;

typedef struct
{
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
} Vec3;

typedef struct
{
	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
	};
} Vec4;

typedef Vec4 Color;

typedef struct 
{
	float CenterX;
	float CenterY;
	float BoundsX;
	float BoundsY;
} Bounds;

typedef struct
{
	float CenterX;
	float CenterY;
	float Radius;
} Radius;

static inline Vec2 RandomPointInBounds(Bounds* bounds)
{
	Vec2 point;
	point.x = (float)(rand() / INT_MAX) * bounds->BoundsX * 2.0f + bounds->CenterX - bounds->BoundsX / 2.0f;
	point.y = (float)(rand() / INT_MAX) * bounds->BoundsY * 2.0f + bounds->CenterY - bounds->BoundsY / 2.0f;
	return point;
}