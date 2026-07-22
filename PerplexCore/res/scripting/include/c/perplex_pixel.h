#pragma once

#include "perplex_math.h"

typedef struct Pixel
{
	Vec2 Position;
	Color Color;
	float Emission;

	Vec2 Velocity;

	float Lifetime;
} Pixel;