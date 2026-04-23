#pragma once

#include <Holloware/Scene/Components.h>

#include <nlohmann/json.hpp>

namespace Holloware
{
	class JsonHelper
	{
	public:
		static void WriteToFile(const nlohmann::json& j, const std::filesystem::path& path);
		static nlohmann::json LoadFromFile(const std::filesystem::path& path);
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IDComponent, ID);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TagComponent, Tag);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransformComponent, Position, Rotation, Scale);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteRendererComponent, SpriteAsset, Color, EmissionSpriteAsset, Emission);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraComponent, Primary, Zoom, Background, PixelsPerUnit, PixelPerfect, ScalingMode);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScriptComponent, ScriptAsset, Properties);

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, ID, Tag);
}

namespace glm
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec3, x, y, z)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec4, r, g, b, a)
}