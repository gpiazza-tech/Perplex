#pragma once

#include "perplex_defs.h"
#include "perplex_math.h"
#include "perplex_bool.h"

PX_EXTERN void Renderer_DrawCircle(Vec2 center, float radius, float thickness, Color color, float emission, bool pixelPerfect);
PX_EXTERN void Renderer_DrawLine(Vec2 start, Vec2 end, Color color, float emission, bool pixelPerfect);
PX_EXTERN void Renderer_DrawBox(Vec2 center, Vec2 bounds, Color color, float emission, bool pixelPerfect);