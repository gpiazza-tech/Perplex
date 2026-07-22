#include <Perplex/pch.h>
#include <Perplex/Scripting/ScriptInstance.h>

#include <Perplex/Scripting/CUnit.h>
#include <Perplex/Core/Input.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Core/Application.h>
#include <Perplex/Core/Project.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Scripting/Interpreter.h>
#include <Perplex/Physics/Simulator.h>
#include <Perplex/Audio/AudioSystem.h>
#include <Perplex/Scene/SceneManager.h>
#include <Perplex/Perpixel/PerpixelInstance.h>
#include <Perplex/Perpixel/PerpixelSystem.h>
#include <c/perplex_pixel.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <string>
#include <string.h>

#define COMPONENT_REMOVER_SYMBOL(x) m_Unit.AddSymbol("Remove"#x, +[](Scene* scene, UUID uuid) { scene->GetEntity(uuid).RemoveComponent<x>(); })

namespace Perplex
{
	static float get_mouse_world_pos_x() { return Input::GetMouseWorldPosition().first; }
	static float get_mouse_world_pos_y() { return Input::GetMouseWorldPosition().second; }
	static glm::vec3* get_position_ptr(Scene* scene, UUID uuid) { return &scene->GetEntity(uuid).GetComponent<TransformComponent>().Position; }
	static glm::vec3* get_rotation_ptr(Scene* scene, UUID uuid) { return &scene->GetEntity(uuid).GetComponent<TransformComponent>().Rotation; }
	static glm::vec3* get_scale_ptr(Scene* scene, UUID uuid) { return &scene->GetEntity(uuid).GetComponent<TransformComponent>().Scale; }

	static float get_sprite_width(Scene* scene, UUID uuid)
	{
		Entity entity = scene->GetEntity(uuid);
		if (!entity || !entity.HasComponent<SpriteRendererComponent>() || !entity.HasComponent<TransformComponent>())
		{
			HW_CORE_ERROR("{0} does not have a SpriteComponent or TransformComponent!", entity.GetTag());
			return 0.0f;
		}

		Ref<pxr::Sprite> colorSprite = entity.GetComponent<SpriteRendererComponent>().SpriteAsset.GetData<pxr::Sprite>();
		return (colorSprite ? colorSprite->ScaleFactorX : 1.0f / 16.0f) * entity.GetComponent<TransformComponent>().Scale.x;
	}

	static float get_sprite_height(Scene* scene, UUID uuid)
	{
		Entity entity = scene->GetEntity(uuid);
		if (!entity || !entity.HasComponent<SpriteRendererComponent>() || !entity.HasComponent<TransformComponent>())
		{
			HW_CORE_ERROR("{0} does not have a SpriteComponent or TransformComponent!", entity.GetTag());
			return 0.0f;
		}

		Ref<pxr::Sprite> colorSprite = entity.GetComponent<SpriteRendererComponent>().SpriteAsset.GetData<pxr::Sprite>();
		return (colorSprite ? colorSprite->ScaleFactorY : 1.0f / 16.0f) * entity.GetComponent<TransformComponent>().Scale.y;
	}

	static glm::vec4* get_color_ptr(Scene* scene, UUID uuid) { return &scene->GetEntity(uuid).GetComponent<SpriteRendererComponent>().Color; }

	static void try_call(Scene* scene, UUID uuid, const char* funcName)
	{
		ScriptInstance* instance = scene->GetSystem<Interpreter>().GetInstance(uuid);

		if (instance != nullptr)
		{
			instance->TryCall(funcName);
		}
		else
		{
			HW_CORE_WARN("Tried to call function on entity with no ScriptComponent!");
		}
	}

	static long long _spawn(Scene* scene, uint64_t prefabAssetID)
	{
		Asset prefabAsset{ UUID{ prefabAssetID} };
		if (!prefabAsset)
		{
			HW_ERROR("Trying to spawn empty prefab!");
			return static_cast<long long>(UUID{});
		}

		Ref<Scene> prefabScene = prefabAsset.GetData<Scene>();
 		Entity newEntity = scene->CopyEntity(prefabScene->GetParentEntities().at(0));
		UUID newEntityID = newEntity.GetUUID();
		return static_cast<long long>(newEntityID);
	}

	static void _destroy(Scene* scene, UUID entity) { scene->DestroyEntity(scene->GetEntity(entity)); }
	static void _destroy_delay(Scene* scene, UUID entity, float delay) { scene->DestroyEntity(scene->GetEntity(entity), delay); }
	static void _set_velocity(Scene* scene, UUID entity, glm::vec2 velocity) { scene->GetSystem<Simulator>().SetVelocity(entity, velocity); }

	static void _to_perpixel(Scene* scene, UUID entityID)
	{
		Entity entity = scene->GetEntity(entityID);
		
		if (!entity.HasComponent<SpriteRendererComponent>())
		{
			HW_CORE_WARN("Entity {0} does not have sprite renderer to convert to perpixel!", (uint64_t)entityID);
			return;
		}

		SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

		PerpixelRendererComponent perpixelDef;
		perpixelDef.Shape.Type = PerpixelShapeType::Sprite;
		perpixelDef.Shape.Info.ColorAsset = spriteRenderer.SpriteAsset;
		perpixelDef.Shape.Info.EmissionAsset = spriteRenderer.EmissionSpriteAsset;
		perpixelDef.Shape.Info.Color = spriteRenderer.Color;
		perpixelDef.Shape.Info.Emission = spriteRenderer.Emission;
		PerpixelRendererComponent& perpixelRenderer = entity.AddComponent<PerpixelRendererComponent>(perpixelDef);

		entity.RemoveComponent<SpriteRendererComponent>();

		if (entity.HasComponent<BoxColliderComponent>())
			entity.RemoveComponent<BoxColliderComponent>();
		if (entity.HasComponent<PhysicsBodyComponent>())
			entity.RemoveComponent<PhysicsBodyComponent>();
	}

	static void _perpixel_spawn_pixel(Scene* scene, UUID entityID, Pixel pixel)
	{
		PerpixelInstance* perpixelInstance = scene->GetSystem<PerpixelSystem>().GetInstance(entityID);
		if (!perpixelInstance)
		{
			HW_CORE_WARN("Entity {0} does not have associated perpixel instance!", (uint64_t)entityID);
			return;
		}
		perpixelInstance->SpawnPixel(pixel);
	}

	bool ScriptInstance::Compile(const std::string& srcName, const std::string& src, Entity entity, const std::vector<ScriptProperty>& properties)
	{
		if (m_Unit.IsCompiled())
			return true;

		m_Unit.SetErrorFunction((void*)srcName.c_str(), [](void* userData, const char* message) { HW_CORE_ERROR("C Error in {0}: {1}", (const char*)userData, message); });

		m_EntityID = entity.GetUUID();;
		m_Properties = properties;
		m_SceneContext = entity.GetScene();

		const Project& project = Application::Get().GetCurrentProject();
		m_Unit.AddIncludePath(project.EngineRes("scripting/include/c").string().c_str());

		m_Unit.DefineSymbol("PROPERTY", "__declspec(dllimport)");

		// Bind properties
		for (auto& property : m_Properties)
		{
			m_Unit.AddSymbol(property.GetName().c_str(), property.GetPtr());
		} 

		// Bind transform
		m_Unit.AddSymbol("scene", &m_SceneContext);
		m_Unit.AddSymbol("this", &m_EntityID);

		// Bind host functions
		m_Unit.AddSymbol("_has_tag", +[](Scene* scene, UUID entity, char* tag)
			{ 
				Entity entityObj = scene->GetEntity(entity);
				if (!entityObj)
				{
					HW_CORE_WARN("Entity with UUID {0} does not exist!", (uint64_t)entity);
					return false;
				}
				return entityObj.GetTag() == tag;
			});

		m_Unit.AddSymbol("_set_paused", +[](Scene* scene, bool paused) { scene->SetPaused(paused); });
		m_Unit.AddSymbol("_pause", +[](Scene* scene) { scene->SetPaused(true); });
		m_Unit.AddSymbol("_resume", +[](Scene* scene) { scene->SetPaused(false); });

		m_Unit.AddSymbol("get_mouse_world_pos_x", get_mouse_world_pos_x);
		m_Unit.AddSymbol("get_mouse_world_pos_y", get_mouse_world_pos_y);

		m_Unit.AddSymbol("_set_enabled", +[](Scene* scene, UUID entity, bool enabled) { scene->GetEntity(entity).GetComponent<EnableComponent>().Enabled = enabled; });
		m_Unit.AddSymbol("get_sprite_width", get_sprite_width);
		m_Unit.AddSymbol("get_sprite_height", get_sprite_height);
		m_Unit.AddSymbol("get_color_ptr", get_color_ptr);

		m_Unit.AddSymbol("get_position_ptr", get_position_ptr);
		m_Unit.AddSymbol("get_rotation_ptr", get_rotation_ptr);
		m_Unit.AddSymbol("get_scale_ptr", get_scale_ptr);

		m_Unit.AddSymbol("console_trace", +[](const char* msg) { HW_TRACE(msg); });
		m_Unit.AddSymbol("console_info", +[](const char* msg) { HW_INFO(msg); });
		m_Unit.AddSymbol("console_warn", +[](const char* msg) { HW_WARN(msg); });
		m_Unit.AddSymbol("console_error", +[](const char* msg) { HW_ERROR(msg); });

		m_Unit.AddSymbol("key_pressed", Input::IsKeyPressed);
		m_Unit.AddSymbol("mouse_button_pressed", Input::IsMouseButtonPressed);
		m_Unit.AddSymbol("degrees", +[](float rad) { return glm::degrees(rad); });
		m_Unit.AddSymbol("radians", +[](float deg) { return glm::radians(deg); });

		m_Unit.AddSymbol("try_call", try_call);

		m_Unit.AddSymbol("_spawn", _spawn);
		m_Unit.AddSymbol("_destroy", _destroy);
		m_Unit.AddSymbol("_destroy_delay", _destroy_delay);
		m_Unit.AddSymbol("_set_velocity", _set_velocity);
		m_Unit.AddSymbol("set_timescale", +[](Scene* scene, float timescale) { scene->SetTimescale(timescale); });

		m_Unit.AddSymbol("_to_perpixel", _to_perpixel);
		m_Unit.AddSymbol("_perpixel_spawn_pixel", _perpixel_spawn_pixel);

		m_Unit.AddSymbol("_play_sound", +[](Scene* scene, const char* filepath) { scene->GetSystem<AudioSystem>().PlaySound(filepath); });
		m_Unit.AddSymbol("_start_loop", +[](Scene* scene, const char* filepath) { return scene->GetSystem<AudioSystem>().StartLoop(filepath); });
		m_Unit.AddSymbol("_end_loop", +[](Scene* scene, Sound* sound) { scene->GetSystem<AudioSystem>().EndLoop(sound); });

		m_Unit.AddSymbol("load_scene", +[](Asset asset) { SceneManager::Get().LoadScene(asset); });

		COMPONENT_REMOVER_SYMBOL(TransformComponent);

		for (auto& externalFunctions : m_ExternalFunctions)
			m_Unit.AddSymbol(externalFunctions.Name.c_str(), externalFunctions.Ptr);

		m_Unit.Compile(src.c_str());

		return m_Unit.IsCompiled();
	}

	void ScriptInstance::AddFunction(const ExternalFunction& function)
	{
		m_ExternalFunctions.emplace_back(function);
	}
}