#pragma once

#include "perplex_defs.h"
#include "perplex_math.h"

struct Scene;

typedef struct Entity
{
	int EntityHandler;
	Scene* ScenePtr;
} Entity;

Entity self;

PX_EXTERN Vec3* _EntityGetPositionPtr(Entity entity);
PX_EXTERN Vec3* _EntityGetRotationPtr(Entity entity);
PX_EXTERN Vec3* _EntityGetScalePtr(Entity entity);
PX_EXTERN bool* _EntityGetEnabledPtr(Entity entity);
PX_EXTERN bool _EntityHasTag(Entity entity, const char* tag);
PX_EXTERN void _EntityGetTag(Entity entity, char* buf, size_t size);
PX_EXTERN void _EntityDestroy(Entity entity, float delay);
PX_EXTERN void _EntityToPerpixel(Entity entity);

#define position (*_EntityGetPositionPtr(self))
#define rotation (*_EntityGetRotationPtr(self))
#define scale (*_EntityGetScalePtr(self))
#define enabled (*_EntityGetEnabledPtr(self))
#define destroy(delay) _EntityDestroy(self)
#define has_tag(tag) _EntityHasTag(self, tag)
#define to_perpixel() _EntityToPerpixel(self)

#define entity_position(entity) (*_EntityGetPositionPtr(entity))
#define entity_rotation(entity) (*_EntityGetRotationPtr(entity))
#define entity_scale(entity) (*_EntityGetScalePtr(entity))
#define entity_enabled(entity) (*_EntityGetEnabledPtr(entity))
#define entity_destroy(entity, delay) _EntityDestroy(entity, delay)
#define entity_has_tag(entity, tag) _EntityHasTag(entity, tag)
#define entity_to_perpixel(entity) _EntityToPerpixel(entity)