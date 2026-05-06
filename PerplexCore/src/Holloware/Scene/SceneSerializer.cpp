#include <pch.h>
#include "SceneSerializer.h"

#include "Holloware/Scene/Scene.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Serialization/Json.h"

#include <nlohmann/json.hpp>

namespace Holloware
{
	bool SceneSerializer::Serialize(const Ref<Scene>& scene, const std::filesystem::path& path)
	{
		nlohmann::json sceneJson = nlohmann::json();

		for (auto entityHandler : scene->m_Registry.view<entt::entity>())
		{
			Entity entity = Entity(entityHandler, scene.get());
			nlohmann::json& entityJson = sceneJson["Entities"][entity.GetComponent<TagComponent>().Tag + " - " + std::to_string(entity.GetUUID())];
			SerializeEntity(entityJson, entity);
		}
		sceneJson["Hierarchy"] = scene->GetHierarchy();

		JsonHelper::WriteToFile(sceneJson, path);

		return true;
	}   

	Ref<Scene> SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		nlohmann::json sceneJson = JsonHelper::LoadFromFile(path);

		Ref<Scene> scene = CreateRef<Scene>();

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
	}

	void SceneSerializer::DeserializeEntity(const nlohmann::json& json, Entity& entity)
	{
		if (json.contains("TransformComponent"))
			entity.AddComponent<TransformComponent>() = json["TransformComponent"];
		if (json.contains("SpriteRendererComponent"))
			entity.AddComponent<SpriteRendererComponent>() = json["SpriteRendererComponent"];
		if (json.contains("CameraComponent"))
			entity.AddComponent<CameraComponent>() = json["CameraComponent"];
		if (json.contains("ScriptComponent"))
			entity.AddComponent<ScriptComponent>() = json["ScriptComponent"];
	}
}