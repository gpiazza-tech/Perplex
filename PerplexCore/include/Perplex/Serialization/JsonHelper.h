#pragma once

#include <Perplex/Scene/Components.h>
#include <Perplex/Scene/EntityNode.h>

#include <glm/fwd.hpp>
#include <nlohmann/json.hpp>

namespace Perplex
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
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BoxColliderComponent, Scale);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PhysicsBodyComponent, GravityScale, Density, Friction);

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData, ID, Tag);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityNode, ID, Index, ParentID, ChildIDs);
}

namespace glm
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec2, x, y)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec3, x, y, z)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec4, r, g, b, a)
}
