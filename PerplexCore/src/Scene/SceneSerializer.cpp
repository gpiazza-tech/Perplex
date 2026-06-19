#include <Perplex/pch.h>
#include <Perplex/Scene/SceneSerializer.h>

#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Physics/Simulator.h>
#include <Perplex/Scripting/Interpreter.h>
#include <Perplex/Perpixel/PerpixelSystem.h>
#include <Perplex/Animation/AnimationSystem.h>
#include <Perplex/Components/ComponentRegistry.h>
#include <Perplex/Components/ComponentKind.h>

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
			Entity entity = Entity{ entityHandler, scene.get() };
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
		scene->AddSystem<AnimationSystem>();

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

		for (auto& componentKind : ComponentRegistry::GetAdditiveKinds())
			if (componentKind.Has(entity))
				componentKind.ToJson(json, entity);
	}

	void SceneSerializer::DeserializeEntity(const nlohmann::json& json, Entity& entity)
	{
		for (auto& componentKind : ComponentRegistry::GetAdditiveKinds())
			if (json.contains(componentKind.Label()))
				componentKind.FromJson(json, entity);
	}
}