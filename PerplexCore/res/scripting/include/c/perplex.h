#pragma once

#include "perplex_defs.h"
#include "perplex_bool.h"
#include "perplex_math.h"
#include "perplex_pixel.h"
#include "perplex_renderer.h"

#include <stdint.h>

typedef unsigned long long UUID;
typedef unsigned long long PrefabAsset;
typedef unsigned long long SceneAsset;

typedef struct Scene
{

} Scene;

typedef struct Entity
{
	int EntityHandler;
	Scene* ScenePtr;
} Entity;

PX_EXTERN Scene* scene;
PX_EXTERN Entity self;

// ENTITY

PX_EXTERN bool Entity_HasTag(Entity entity, const char* tag);
PX_EXTERN void Entity_SetEnabled(Entity entity, bool enabled);

PX_EXTERN Vec3* Entity_GetPositionPtr(Entity entity);
#define position_of(entity) (*Entity_GetPositionPtr(entity))
#define position (*Entity_GetPositionPtr(self))

PX_EXTERN Vec3* Entity_GetRotationPtr(Entity entity);
#define rotation_of(entity) (*Entity_GetRotationPtr(entity))
#define rotation (*Entity_GetRotationPtr(self))

PX_EXTERN Vec3* Entity_GetScalePtr(Entity entity);
#define scale_of(entity) (*Entity_GetScalePtr(entity))
#define scale (*Entity_GetScalePtr(self))

PX_EXTERN void Entity_ToPerpixel(Entity entity);

PX_EXTERN void Entity_Destroy(Entity entity);
PX_EXTERN void Entity_DestroyDelay(Entity entity, float delay);

// SCENE

PX_EXTERN void Scene_SetPaused(Scene* scene, bool paused);
#define set_paused(paused) (Scene_SetPaused(scene, paused))

PX_EXTERN void Scene_Pause(Scene* scene);
#define pause() (Scene_Pause(scene))

PX_EXTERN void Scene_Resume(Scene* scene);
#define resume() (Scene_Resume(scene))

PX_EXTERN Entity Scene_Spawn(Scene* scene, PrefabAsset prefab);
#define spawn(prefab) (Scene_Spawn(scene, prefab))

// DEBUG

PX_EXTERN void Trace(char* msg);
PX_EXTERN void Info(char* msg);
PX_EXTERN void Warn(char* msg);
PX_EXTERN void Error(char* msg);

// MATH

PX_EXTERN float Degrees(float rad);
PX_EXTERN float Radians(float deg);

// COMPONENTS

PX_EXTERN Vec2* Sprite_GetSizePtr(Entity entity);
#define spriteSize (*Sprite_GetSizePtr(self))

PX_EXTERN Color* Sprite_GetColorPtr(Entity entity);
#define spriteColor (*Sprite_GetColorPtr(self));

PX_EXTERN void Script_TryCall(Entity entity, char* funcName);
PX_EXTERN void Perpixel_SpawnPixel(Entity entity, Pixel pixel);

PX_EXTERN void PhysicsBody_SetVelocity(Entity entity, Vec2 velocity);

// TODO: PX_EXTERN void Entity_PerpixelSpawnPixel(Entity entity);

typedef void* Sound;
PX_EXTERN void Scene_PlaySound(Scene* scene, const char* filepath);
#define play_sound(filepath) (Scene_PlaySound(scene, filepath))

PX_EXTERN Sound Scene_StartLoop(Scene* scene, const char* filepath);
#define start_loop(filepath) (Scene_StartLoop(scene, filepath))

PX_EXTERN void Scene_EndLoop(Scene* scene, Sound sound);
#define end_loop(sound) (Scene_EndLoop(scene, sound))

PX_EXTERN void SceneAsset_Load(SceneAsset sceneAsset);
PX_EXTERN void Scene_SetTimescale(Scene* scene, float timescale);

// __declspec(dllimport) void _set_velocity(Scene s, Entity e, struct Vec2 velocity);
// void set_velocity(struct Vec2 velocity) { _set_velocity(scene, entity, velocity); }