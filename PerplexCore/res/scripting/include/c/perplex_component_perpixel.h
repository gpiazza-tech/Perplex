#pragma once

#include "perplex_defs.h"
#include "perplex_entity.h"
#include "perplex_math.h"

typedef struct Pixel
{
	Vec2 Position;
	Color Color;
	float Emission;

	Vec2 Velocity;

	float Lifetime;
} Pixel;

PX_EXTERN void Perpixel_SpawnPixel(Entity entity, Pixel pixel);