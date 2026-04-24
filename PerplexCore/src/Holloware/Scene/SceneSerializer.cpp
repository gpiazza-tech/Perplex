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

			entityJson["ID"] = entity.GetComponent<IDComponent>().ID;
			entityJson["Tag"] = entity.GetComponent<TagComponent>().Tag;

			if (entity.HasComponent<TransformComponent>())
				entityJson["TransformComponent"] = entity.GetComponent<TransformComponent>();
			if (entity.HasComponent<SpriteRendererComponent>())
				entityJson["SpriteRendererComponent"] = entity.GetComponent<SpriteRendererComponent>();
			if (entity.HasComponent<CameraComponent>())
				entityJson["CameraComponent"] = entity.GetComponent<CameraComponent>();
			if (entity.HasComponent<ScriptComponent>())
				entityJson["ScriptComponent"] = entity.GetComponent<ScriptComponent>();
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

			if (entityJson.contains("TransformComponent"))
				entity.AddComponent<TransformComponent>() = entityJson["TransformComponent"];
			if (entityJson.contains("SpriteRendererComponent"))
				entity.AddComponent<SpriteRendererComponent>() = entityJson["SpriteRendererComponent"];
			if (entityJson.contains("CameraComponent"))
				entity.AddComponent<CameraComponent>() = entityJson["CameraComponent"];
			if (entityJson.contains("ScriptComponent"))
				entity.AddComponent<ScriptComponent>() = entityJson["ScriptComponent"];
		}

		scene->m_Hierarchy = sceneJson["Hierarchy"];

		return scene;
	}
}