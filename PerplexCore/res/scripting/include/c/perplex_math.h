#pragma once

#include <stdlib.h>

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

// Returns a random value from 0.0f inclusively to 1.0f exclusively
static inline float RandomFloat()
{
	return (float)rand() / (float)RAND_MAX;
}

// Returns a random value from min inclusively to max exclusively
static inline float RandomFloatRange(float min, float max)
{
	return RandomFloat() * (max - min) + min;
}

// Returns a random integer
static inline int RandomInt()
{
	return rand();
}

// Returns a random value from min inclusively to max exclusively
static inline int RandomIntRange(float min, float max)
{
	return (int)(RandomFloat() * (max - min) + min);
}

static inline Vec2 RandomPointInBounds(Bounds* bounds)
{
	Vec2 point;
	point.x = RandomFloatRange(bounds->CenterX - bounds->BoundsX, bounds->CenterX + bounds->BoundsX);
	point.y = RandomFloatRange(bounds->CenterY - bounds->BoundsY, bounds->CenterY + bounds->BoundsY);
	return point;
}