#pragma once

#include "perplex_math.h"

#ifdef __cplusplus
struct pixel
{
	vector2 Position;
	vector4 Color;
	float Emission;
	float Lifetime;
};
#else
struct pixel
{
	struct vector2 Position;
	struct vector4 Color;
	float Emission;
	float Lifetime;
};
#endif