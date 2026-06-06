#pragma once

#include <Perplex/Scene/Components.h>
#include <Perplex/Scene/EntityNode.h>

#include <glm/fwd.hpp>
#include <nlohmann/json.hpp>

#define PERPLEX_JSON_TO(v1) nlohmann_json_j[#v1] = nlohmann_json_t.v1;
#define PERPLEX_JSON_FROM(v1) nlohmann_json_t.v1 = nlohmann_json_j.value(#v1, nlohmann_json_t.v1);

#define PERPLEX_DEFINE_JSON_STRUCT(Type, ...)  \
    template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int> = 0> \
    void to_json(BasicJsonType& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(PERPLEX_JSON_TO, __VA_ARGS__)) } \
    template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int> = 0> \
    void from_json(const BasicJsonType& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(PERPLEX_JSON_FROM, __VA_ARGS__)) }

namespace Perplex
{
	class JsonHelper
	{
	public:
		static void WriteToFile(const nlohmann::json& j, const std::filesystem::path& path);
		static nlohmann::json LoadFromFile(const std::filesystem::path& path);
	};

	PERPLEX_DEFINE_JSON_STRUCT(IDComponent, ID)
	PERPLEX_DEFINE_JSON_STRUCT(TagComponent, Tag)
	PERPLEX_DEFINE_JSON_STRUCT(TransformComponent, Position, Rotation, Scale)
	PERPLEX_DEFINE_JSON_STRUCT(SpriteRendererComponent, SpriteAsset, Color, EmissionSpriteAsset, Emission)
	PERPLEX_DEFINE_JSON_STRUCT(CameraComponent, Primary, Zoom, Background, PixelsPerUnit, PixelPerfect, ScalingMode)
	PERPLEX_DEFINE_JSON_STRUCT(ScriptComponent, ScriptAsset, Properties)
	PERPLEX_DEFINE_JSON_STRUCT(BoxColliderComponent, Scale)
	PERPLEX_DEFINE_JSON_STRUCT(PhysicsBodyComponent, GravityScale, Density, Friction)

	PERPLEX_DEFINE_JSON_STRUCT(PerpixelShapeInfo, CircleRadius, RectSize, ColorAsset, EmissionAsset, Color, Emission)
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelShape, Info, Type)
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelRendererComponent, Shape)

	PERPLEX_DEFINE_JSON_STRUCT(EntityData, ID, Tag)
	PERPLEX_DEFINE_JSON_STRUCT(EntityNode, ID, Index, ParentID, ChildIDs)
}

namespace glm
{
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec2, x, y)
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec3, x, y, z)
	PERPLEX_DEFINE_JSON_STRUCT(glm::vec4, r, g, b, a)
}
