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
#include <Perplex/Assets/Asset.h>
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

	static Entity Scene_Spawn(Scene* scene, uint64_t prefabAssetID)
	{
		Asset prefabAsset{ UUID{ prefabAssetID } };
		if (!prefabAsset)
		{
			HW_ERROR("Trying to spawn empty prefab!");
			return Entity{};
		}

		Ref<Scene> prefabScene = prefabAsset.GetData<Scene>();
 		Entity newEntity = scene->CopyEntity(prefabScene->GetParentEntities().at(0));
		return newEntity;
	}

	static void _to_perpixel(Entity entity)
	{
		if (!entity.HasComponent<SpriteRendererComponent>())
		{
			HW_CORE_WARN("Entity {0} does not have sprite renderer to convert to perpixel!", (uint64_t)entity.GetUUID());
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

	static void _perpixel_spawn_pixel(Entity entity, Pixel pixel)
	{
		PerpixelInstance* perpixelInstance = entity.GetScene()->GetSystem<PerpixelSystem>().GetInstance(entity.GetUUID());
		if (!perpixelInstance)
		{
			HW_CORE_WARN("Entity {0} does not have associated perpixel instance!", (uint64_t)entity.GetUUID());
			return;
		}
		perpixelInstance->SpawnPixel(pixel);
	}

	bool ScriptInstance::Compile(const std::string& srcName, const std::string& src, Entity entity, const std::vector<ScriptProperty>& properties)
	{
		if (m_Unit.IsCompiled())
			return true;

		m_Unit.SetErrorFunction((void*)srcName.c_str(), [](void* userData, const char* message) { HW_CORE_ERROR("C Error in {0}: {1}", (const char*)userData, message); });

		m_Entity = entity;
		m_ScenePtr = entity.GetScene();
		m_Properties = properties;

		const Project& project = Application::Get().GetCurrentProject();
		m_Unit.AddIncludePath(project.EngineRes("scripting/include/c").string().c_str());

		m_Unit.DefineSymbol("PROPERTY", "__declspec(dllimport)");

		// Bind properties
		for (auto& property : m_Properties)
		{
			m_Unit.AddSymbol(property.GetName().c_str(), property.GetPtr());
		}

		// Bind transform
		m_Unit.AddSymbol("scene", &m_ScenePtr);
		m_Unit.AddSymbol("self", &m_Entity);

		// Bind host functions

		// ENTITY
		m_Unit.AddSymbol("Entity_HasTag", +[](Entity entity, const char* tag)
			{ 
				if (!entity)
				{
					HW_CORE_WARN("Entity does not exist!");
					return false;
				}
				return entity.GetTag() == tag;
			});
		m_Unit.AddSymbol("Entity_SetEnabled", +[](Entity entity, bool enabled) { entity.GetComponent<EnableComponent>().Enabled = enabled; });

		m_Unit.AddSymbol("Entity_GetPositionPtr", +[](Entity entity) { return &entity.GetComponent<TransformComponent>().Position; });
		m_Unit.AddSymbol("Entity_GetRotationPtr", +[](Entity entity) { return &entity.GetComponent<TransformComponent>().Rotation; });
		m_Unit.AddSymbol("Entity_GetScalePtr", +[](Entity entity) { return &entity.GetComponent<TransformComponent>().Scale; });

		m_Unit.AddSymbol("Entity_ToPerpixel", _to_perpixel);
		m_Unit.AddSymbol("Perpixel_SpawnPixel", _perpixel_spawn_pixel);

		m_Unit.AddSymbol("Entity_Destroy", +[](Entity entity) { entity.GetScene()->DestroyEntity(entity); });
		m_Unit.AddSymbol("Entity_DestroyDelay", +[](Entity entity, float delay) { entity.GetScene()->DestroyEntity(entity, delay); });

		// SCENE

		m_Unit.AddSymbol("Scene_SetPaused", +[](Scene* scene, bool paused) { scene->SetPaused(paused); });
		m_Unit.AddSymbol("Scene_Pause", +[](Scene* scene) { scene->SetPaused(true); });
		m_Unit.AddSymbol("Scene_Resume", +[](Scene* scene) { scene->SetPaused(false); });

		m_Unit.AddSymbol("Scene_Spawn", Scene_Spawn);

		m_Unit.AddSymbol("Scene_SetTimescale", +[](Scene* scene, float timescale) { scene->SetTimescale(timescale); });

		m_Unit.AddSymbol("Scene_PlaySound", +[](Scene* scene, const char* filepath) { scene->GetSystem<AudioSystem>().PlaySound(filepath); });
		m_Unit.AddSymbol("Scene_StartLoop", +[](Scene* scene, const char* filepath) { return scene->GetSystem<AudioSystem>().StartLoop(filepath); });
		m_Unit.AddSymbol("Scene_EndLoop", +[](Scene* scene, Sound* sound) { scene->GetSystem<AudioSystem>().EndLoop(sound); });

		// COMPONENTS

		m_Unit.AddSymbol("Sprite_GetColorPtr", +[](Entity entity) { return &entity.GetComponent<SpriteRendererComponent>().Color; });
		m_Unit.AddSymbol("PhysicsBody_SetVelocity", +[](Entity entity, glm::vec2 velocity) { entity.GetScene()->GetSystem<Simulator>().SetVelocity(entity.GetUUID(), velocity); });

		m_Unit.AddSymbol("Script_TryCall", +[](Entity entity) { entity.GetScene()->GetSystem<Interpreter>().GetInstance(entity.GetUUID()); });
		// DEBUG

		m_Unit.AddSymbol("Trace", +[](const char* msg) { HW_TRACE(msg); });
		m_Unit.AddSymbol("Info", +[](const char* msg) { HW_INFO(msg); });
		m_Unit.AddSymbol("Warn", +[](const char* msg) { HW_WARN(msg); });
		m_Unit.AddSymbol("Error", +[](const char* msg) { HW_ERROR(msg); });

		// MATH

		m_Unit.AddSymbol("key_pressed", Input::IsKeyPressed);
		m_Unit.AddSymbol("mouse_button_pressed", Input::IsMouseButtonPressed);
		m_Unit.AddSymbol("degrees", +[](float rad) { return glm::degrees(rad); });
		m_Unit.AddSymbol("radians", +[](float deg) { return glm::radians(deg); });

		m_Unit.AddSymbol("get_mouse_world_pos_x", get_mouse_world_pos_x);
		m_Unit.AddSymbol("get_mouse_world_pos_y", get_mouse_world_pos_y);

		// RENDERER

		m_Unit.AddSymbol("Renderer_DrawCircle", +[](glm::vec2 center, float radius, float thickness, glm::vec4 color, float emission, bool pixelPerfect) { pxr::Renderer::DrawCircle(center, radius, thickness, color, emission, pixelPerfect); });
		m_Unit.AddSymbol("Renderer_DrawLine", +[](glm::vec2 start, glm::vec2 end, glm::vec4 color, float emission, bool pixelPerfect) { pxr::Renderer::DrawLine(start, end, color, emission, pixelPerfect); });
		m_Unit.AddSymbol("Renderer_DrawBox", +[](glm::vec2 center, glm::vec2 bounds, glm::vec4 color, float emission, bool pixelPerfect) { pxr::Renderer::DrawBox(center, bounds, color, emission, pixelPerfect); });
		
		// MISC

		m_Unit.AddSymbol("SceneAsset_Load", +[](Asset asset) { SceneManager::Get().LoadScene(asset); });

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