#include "perplex_bool.h"
#include "perplex_math.h"

#include <stdint.h>

#ifndef PROPERTY
#define PROPERTY
#endif

typedef unsigned long long Entity;
typedef unsigned long long PrefabAsset;

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

__declspec(dllimport) Scene scene;
__declspec(dllimport) Entity entity;

__declspec(dllimport) void console_trace(char* msg);
__declspec(dllimport) void console_info(char* msg);
__declspec(dllimport) void console_warn(char* msg);
__declspec(dllimport) void console_error(char* msg);

__declspec(dllimport) struct Vec3* get_position_ptr(Scene s, Entity e);
__declspec(dllimport) struct Vec3* get_rotation_ptr(Scene s, Entity e);
__declspec(dllimport) struct Vec3* get_scale_ptr(Scene s, Entity e);

__declspec(dllimport) float degrees(float rad);
__declspec(dllimport) float radians(float deg);

__declspec(dllimport) void try_call(Scene s, Entity e, char* funcName);

__declspec(dllimport) Entity _spawn(Scene _scene, PrefabAsset _prefab);
__declspec(dllimport) void _destroy(Scene _scene, Entity _entity);
__declspec(dllimport) void _destroy_delay(Scene _scene, Entity _entity, float delay);

#define get_position(e) (*get_position_ptr(scene, e))
#define get_rotation(e) (*get_rotation_ptr(scene, e))
#define get_scale(e) (*get_scale_ptr(scene, e))

#define position (*get_position_ptr(scene, entity))
#define rotation (*get_rotation_ptr(scene, entity))
#define scale (*get_scale_ptr(scene, entity))

#define call(entity, funcName) (try_call(scene, entity, funcName))

#define spawn(prefab) (_spawn(scene, prefab))
#define destroy(entity) (_destroy(scene, entity))
#define destroy_delay(entity, delay) (_destroy_delay(scene, entity, delay))

// __declspec(dllimport) void _set_velocity(Scene s, Entity e, struct Vec2 velocity);
// void set_velocity(struct Vec2 velocity) { _set_velocity(scene, entity, velocity); }