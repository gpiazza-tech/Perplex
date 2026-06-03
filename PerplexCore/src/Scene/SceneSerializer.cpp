#include <Perplex/pch.h>
#include <Perplex/Scene/SceneSerializer.h>

#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Scene/Components.h>
#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Serialization/Json.h>
#include <Perplex/Physics/Simulator.h>
#include <Perplex/Scripting/ScriptInstance.h>
#include <Perplex/Scripting/Interpreter.h>
#include <Perplex/Perpixel/PerpixelSystem.h>

#include <filesystem>
#include <string>

namespace Perplex
{
	namespace fs = std::filesystem;

	bool SceneSerializer::Serialize(const Ref<Scene>& scene, const fs::path& path)
	{
		nlohmann::json sceneJson = nlohmann::json();

		for (auto entityHandler : scene->m_Registry.view<IDComponent>())
		{
			Entity entity = Entity(entityHandler, scene.get());
			nlohmann::json& entityJson = sceneJson["Entities"][entity.GetTag() + " - " + std::to_string(entity.GetUUID())];
			SerializeEntity(entityJson, entity);
		}
		sceneJson["Hierarchy"] = scene->GetHierarchy();

		JsonHelper::WriteToFile(sceneJson, path);

		return true;
	}   

	Ref<Scene> SceneSerializer::Deserialize(const fs::path& path)
	{
		nlohmann::json sceneJson = JsonHelper::LoadFromFile(path);

		Ref<Scene> scene = CreateRef<Scene>();
		scene->AddSystem<Simulator>();
		scene->AddSystem<Interpreter>();
		scene->AddSystem<PerpixelSystem>();

		for (auto& entityJson : sceneJson["Entities"])
		{
			Entity entity = scene->CreateAbstractEntity(entityJson["Tag"].get<std::string>(), entityJson["ID"].get<UUID>());
			DeserializeEntity(entityJson, entity);
		}

		scene->m_Hierarchy = sceneJson["Hierarchy"];

		return scene;
	}

	void SceneSerializer::SerializeEntity(nlohmann::json& json, Entity& entity)
	{
		json["ID"] = entity.GetComponent<IDComponent>().ID;
		json["Tag"] = entity.GetComponent<TagComponent>().Tag;

		if (entity.HasComponent<TransformComponent>())
			json["TransformComponent"] = entity.GetComponent<TransformComponent>();
		if (entity.HasComponent<SpriteRendererComponent>())
			json["SpriteRendererComponent"] = entity.GetComponent<SpriteRendererComponent>();
		if (entity.HasComponent<CameraComponent>())
			json["CameraComponent"] = entity.GetComponent<CameraComponent>();
		if (entity.HasComponent<ScriptComponent>())
			json["ScriptComponent"] = entity.GetComponent<ScriptComponent>();
		if (entity.HasComponent<BoxColliderComponent>())
			json["BoxColliderComponent"] = entity.GetComponent<BoxColliderComponent>();
		if (entity.HasComponent<PhysicsBodyComponent>())
			json["PhysicsBodyComponent"] = entity.GetComponent<PhysicsBodyComponent>();
		if (entity.HasComponent<PerpixelRendererComponent>())
			json["PerpixelRendererComponent"] = entity.GetComponent<PerpixelRendererComponent>();
	}

	void SceneSerializer::DeserializeEntity(const nlohmann::json& json, Entity& entity)
	{
		if (json.contains("TransformComponent"))
			entity.AddComponent<TransformComponent>(json["TransformComponent"]);
		if (json.contains("SpriteRendererComponent"))
			entity.AddComponent<SpriteRendererComponent>(json["SpriteRendererComponent"]);
		if (json.contains("CameraComponent"))
			entity.AddComponent<CameraComponent>(json["CameraComponent"]);
		if (json.contains("ScriptComponent"))
			entity.AddComponent<ScriptComponent>(json["ScriptComponent"]);
		if (json.contains("BoxColliderComponent"))
			entity.AddComponent<BoxColliderComponent>(json["BoxColliderComponent"]);
		if (json.contains("PhysicsBodyComponent"))
			entity.AddComponent<PhysicsBodyComponent>(json["PhysicsBodyComponent"]);
		if (json.contains("PerpixelRendererComponent"))
			entity.AddComponent<PerpixelRendererComponent>(json["PerpixelRendererComponent"]);
	}
}