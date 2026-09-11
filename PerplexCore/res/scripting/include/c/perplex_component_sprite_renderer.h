#pragma once

#include "perplex_defs.h"
#include "perplex_math.h"
#include "perplex_entity.h"

PX_EXTERN float _SpriteGetWidth(Entity entity);
PX_EXTERN float _SpriteGetHeight(Entity entity);

PX_EXTERN Color* _SpriteGetColorPtr(Entity entity);
#define sprite_color (*_SpriteGetColorPtr(self))