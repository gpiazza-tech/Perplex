#pragma once

#include <stdlib.h>

typedef struct Vec2
{
	union
	{
		struct { float x, y; };
		struct { float r, g; };
	};
} Vec2;

typedef struct Vec3
{
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
} Vec3;

typedef struct Vec4
{
	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
	};
} Vec4;

typedef Vec4 Color;

static inline Vec4 Vec4_Create(float x, float y, float z, float w)
{
	Vec4 vec4;
	vec4.x = x;
	vec4.y = y;
	vec4.z = z;
	vec4.w = w;
	return vec4;
}

static inline Vec3 Vec3_Create(float x, float y, float z)
{
	Vec3 vec3;
	vec3.x = x;
	vec3.y = y;
	vec3.z = z;
	return vec3;
}

static inline Vec2 Vec2_Create(float x, float y)
{
	Vec2 vec2;
	vec2.x = x;
	vec2.y = y;
	return vec2;
}

typedef struct Bounds
{
	float CenterX;
	float CenterY;
	float BoundsX;
	float BoundsY;
} Bounds;

typedef struct Radius
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

static inline Vec2 RandomPointInBounds(const Bounds* bounds)
{
	Vec2 point;
	point.x = RandomFloatRange(bounds->CenterX - bounds->BoundsX, bounds->CenterX + bounds->BoundsX);
	point.y = RandomFloatRange(bounds->CenterY - bounds->BoundsY, bounds->CenterY + bounds->BoundsY);
	return point;
}

static inline float Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

static inline Vec2 LerpVec2(Vec2 a, Vec2 b, float t)
{
	return Vec2_Create(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t));
}

static inline Vec3 LerpVec3(Vec3 a, Vec3 b, float t)
{
	return Vec3_Create(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t));
}

static inline Vec4 LerpVec4(Vec4 a, Vec4 b, float t)
{
	return Vec4_Create(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t), Lerp(a.w, b.w, t));
}