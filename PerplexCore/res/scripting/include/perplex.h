#include "perplex_bool.h"
#include "perplex_math.h"

#include <stdint.h>

#ifndef PROPERTY
#define PROPERTY
#endif

#ifndef Entity
#define Entity unsigned long long
#endif

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

struct Vec3 vec2_create(float x, float y)
{
	struct Vec3 vec3;
	vec3.x = x;
	vec3.y = y;
	vec3.z = 0.0f;
	return vec3;
}

typedef void* Scene;

__declspec(dllimport) Scene scene;
__declspec(dllimport) Entity entity;

__declspec(dllimport) struct Vec3 position;
__declspec(dllimport) struct Vec3 rotation;
__declspec(dllimport) struct Vec3 scale;

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

#define get_position(e) (*get_position_ptr(scene, e))
#define get_rotation(e) (*get_rotation_ptr(scene, e))
#define get_scale(e) (*get_scale_ptr(scene, e))

#define call(entity, funcName) (try_call(scene, entity, funcName))