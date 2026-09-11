#pragma once

#include "perplex_types.h"
#include "perplex_entity.h"

typedef struct Scene
{

} Scene;

PX_EXTERN Scene* scene;

PX_EXTERN void _SceneInvokeEvent(Scene* scene, const char* eventName, void* data);
PX_EXTERN void _SceneSetPaused(Scene* scene, bool paused);
PX_EXTERN void _ScenePause(Scene* scene);
PX_EXTERN void _SceneResume(Scene* scene);
PX_EXTERN Entity _SceneSpawn(Scene* scene, PrefabAsset prefab);
PX_EXTERN float _SceneGetTime(Scene* scene);
PX_EXTERN void _SceneSetTimescale(Scene* scene, float timescale);
PX_EXTERN void _SceneCameraShake(Scene* scene, float trauma);

#define event(eventName, data) (_SceneInvokeEvent(scene, eventName, data))
#define set_paused(paused) (_SceneSetPaused(scene, paused))
#define pause() (_ScenePause(scene))
#define resume() (_SceneResume(scene))
#define spawn(prefab) (_SceneSpawn(scene, prefab))
#define get_time (_SceneGetTime(scene))
#define camera_shake(trauma) (_SceneCameraShake(scene, trauma))

// Audio

typedef void* Sound;

PX_EXTERN void _ScenePlaySound(Scene* scene, const char* filepath);
PX_EXTERN Sound _SceneStartLoop(Scene* scene, const char* filepath);
PX_EXTERN void _SceneEndLoop(Scene* scene, Sound sound);

#define play_sound(filepath) (_ScenePlaySound(scene, filepath))
#define start_loop(filepath) (_SceneStartLoop(scene, filepath))
#define end_loop(sound) (_SceneEndLoop(scene, sound))
