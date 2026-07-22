#pragma once

#include "perplex_bool.h"
#include "perplex_math.h"
#include "perplex_pixel.h"

#include <stdint.h>

#ifndef PROPERTY
#define PROPERTY
#endif

#ifndef PX_EXTERN
#define PX_EXTERN __declspec(dllimport)
#endif

typedef unsigned long long Entity;
typedef unsigned long long PrefabAsset;
typedef unsigned long long SceneAsset;
typedef void* Scene;

PX_EXTERN Scene scene;
PX_EXTERN Entity this;

PX_EXTERN bool _has_tag(Scene _scene, Entity e, char* tag);

PX_EXTERN void _set_paused(Scene _scene, bool paused);
PX_EXTERN void _pause(Scene _scene);
PX_EXTERN void _resume(Scene _scene);

PX_EXTERN float get_sprite_width(Scene _scene, Entity e);
PX_EXTERN float get_sprite_height(Scene _scene, Entity e);
PX_EXTERN struct Vec4* get_color_ptr(Scene s, Entity e);

PX_EXTERN void console_trace(char* msg);
PX_EXTERN void console_info(char* msg);
PX_EXTERN void console_warn(char* msg);
PX_EXTERN void console_error(char* msg);

PX_EXTERN void _set_enabled(Scene _scene, Entity _entity, bool enabled);
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
PX_EXTERN void set_timescale(Scene _scene, float timescale);

PX_EXTERN void _to_perpixel(Scene _scene, Entity _entity);
PX_EXTERN void _perpixel_spawn_pixel(Scene _scene, Entity _entity, Pixel pixel);

typedef void* Sound;
PX_EXTERN void _play_sound(Scene _scene, const char* filepath);
PX_EXTERN Sound _start_loop(Scene _scene, const char* filepath);
PX_EXTERN void _end_loop(Scene _scene, Sound sound);

bool has_tag(Entity e, char* tag) { return _has_tag(scene, e, tag); }

#define play_sound(filepath) (_play_sound(scene, filepath))
#define start_loop(filepath) (_start_loop(scene, filepath))
#define end_loop(sound) (_end_loop(scene, sound))

PX_EXTERN void load_scene(SceneAsset sceneAsset);

#define set_enabled(e, enabled) (_set_enabled(scene, e, enabled)) 
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
#define perpixel_spawn_pixel(pxl) _perpixel_spawn_pixel(scene, this, pxl)

// __declspec(dllimport) void _set_velocity(Scene s, Entity e, struct Vec2 velocity);
// void set_velocity(struct Vec2 velocity) { _set_velocity(scene, entity, velocity); }