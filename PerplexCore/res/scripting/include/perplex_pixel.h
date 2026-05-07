#pragma once

#include "perplex_math.h"

struct pixel
{
	struct vector2 Position;
	struct vector4 Color;
	float Emission;
	float Lifetime;
};