#pragma once

#include <Perplex/Core/Core.h>

#include <nlohmann/json_fwd.hpp>

#include <filesystem>
#include <string>

namespace Perplex
{
	class Scene;
	class Asset;
	class Entity;

	class SceneSerializer
	{
	public:
		static bool Serialize(const Ref<Scene>& scene, const std::filesystem::path& path);
		static Ref<Scene> Deserialize(const std::filesystem::path& path);

		static void SerializeEntity(nlohmann::json& json, Entity& entity);
		static void DeserializeEntity(const nlohmann::json& json, Entity& entity);
	};
} 