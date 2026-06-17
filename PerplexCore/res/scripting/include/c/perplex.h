#pragma once

#include "perplex_bool.h"
#include "perplex_math.h"

#include <stdint.h>

#ifndef PROPERTY
#define PROPERTY
#endif

#ifndef PX_EXTERN
#define PX_EXTERN __declspec(dllimport)
#endif

typedef unsigned long long Entity;
typedef unsigned long long PrefabAsset;

struct Vec4
{
	union { float x, r; };
	union { float y, g; };
	union { float z, b; };
	union { float w, a; };
};

struct Vec4 vec4_create(float x, float y, float z, float w)
{
	struct Vec4 vec4;
	vec4.x = x;
	vec4.y = y;
	vec4.z = z;
	vec4.w = w;
	return vec4;
}

struct Vec3
{
	float x;
	float y;
	float z;
};

struct Vec3 vec3_create(float x, float y, float z)
{
	struct Vec3 vec3;
	vec3.x = x;
	vec3.y = y;
	vec3.z = z;
	return vec3;
}

struct Vec2
{
	float x;
	float y;
};

struct Vec2 vec2_create(float x, float y)
{
	struct Vec2 vec2;
	vec2.x = x;
	vec2.y = y;
	return vec2;
}

typedef void* Scene;

PX_EXTERN Scene scene;
PX_EXTERN Entity this;

PX_EXTERN float get_sprite_width(Scene _scene, Entity e);
PX_EXTERN float get_sprite_height(Scene _scene, Entity e);
PX_EXTERN struct Vec4* get_color_ptr(Scene s, Entity e);

PX_EXTERN void console_trace(char* msg);
PX_EXTERN void console_info(char* msg);
PX_EXTERN void console_warn(char* msg);
PX_EXTERN void console_error(char* msg);

PX_EXTERN struct Vec3* get_position_ptr(Scene s, Entity e);
PX_EXTERN struct Vec3* get_rotation_ptr(Scene s, Entity e);
PX_EXTERN struct Vec3* get_scale_ptr(Scene s, Entity e);

PX_EXTERN float degrees(float rad);
PX_EXTERN float radians(float deg);

PX_EXTERN void try_call(Scene s, Entity e, char* funcName);

PX_EXTERN Entity _spawn(Scene _scene, PrefabAsset _prefab);
PX_EXTERN void _destroy(Scene _scene, Entity _entity);
PX_EXTERN void _destroy_delay(Scene _scene, Entity _entity, float delay);
PX_EXTERN void _set_velocity(Scene _scene, Entity _entity, struct Vec2 _velocity);
PX_EXTERN void _to_perpixel(Scene _scene, Entity _entity);

typedef void* Sound;
PX_EXTERN void play_sound(const char* filepath);
PX_EXTERN Sound start_loop(const char* filepath);
PX_EXTERN void end_loop(Sound sound);

#define get_position(e) (*get_position_ptr(scene, e))
#define get_rotation(e) (*get_rotation_ptr(scene, e))
#define get_scale(e) (*get_scale_ptr(scene, e))

#define get_color(e) (*get_color_ptr(scene, e))

#define position (*get_position_ptr(scene, this))
#define rotation (*get_rotation_ptr(scene, this))
#define scale (*get_scale_ptr(scene, this))

#define color (*get_color_ptr(scene, this))

#define call(entity, funcName) (try_call(scene, entity, funcName))

#define spawn(prefab) (_spawn(scene, prefab))
#define destroy(entity) (_destroy(scene, entity))
#define destroy_delay(entity, delay) (_destroy_delay(scene, entity, delay))

#define to_perpixel() _to_perpixel(scene, this)

// __declspec(dllimport) void _set_velocity(Scene s, Entity e, struct Vec2 velocity);
// void set_velocity(struct Vec2 velocity) { _set_velocity(scene, entity, velocity); }