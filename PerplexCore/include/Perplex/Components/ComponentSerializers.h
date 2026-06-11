#pragma once

#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Scene/Components.h>

namespace Perplex
{
	// ID
	PERPLEX_DEFINE_JSON_STRUCT(IDComponent, ID)

	// Tag
	PERPLEX_DEFINE_JSON_STRUCT(TagComponent, Tag)

	// Transform
	PERPLEX_DEFINE_JSON_STRUCT(TransformComponent, Position, Rotation, Scale)

	// Sprite Renderer
	PERPLEX_DEFINE_JSON_STRUCT(SpriteRendererComponent, SpriteAsset, Color, EmissionSpriteAsset, Emission)

	// Camera
	PERPLEX_DEFINE_JSON_STRUCT(CameraComponent, Primary, Zoom, Background, PixelsPerUnit, PixelPerfect, ScalingMode)

	// Script
	PERPLEX_DEFINE_JSON_STRUCT(ScriptComponent, ScriptAsset, Properties)

	// Box Collider
	PERPLEX_DEFINE_JSON_STRUCT(BoxColliderComponent, Scale)

	// Physics Body
	PERPLEX_DEFINE_JSON_STRUCT(PhysicsBodyComponent, GravityScale, Density, Friction)

	// Perpixel Renderer
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelShapeInfo, CircleRadius, RectSize, ColorAsset, EmissionAsset, Color, Emission)
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelShape, Info, Type)
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelRendererComponent, Shape)

	// Misc
	PERPLEX_DEFINE_JSON_STRUCT(EntityData, ID, Tag)
	PERPLEX_DEFINE_JSON_STRUCT(EntityNode, ID, Index, ParentID, ChildIDs)
}