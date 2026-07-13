#pragma once

#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Components/Components.h>

namespace Perplex
{
	// ID
	PERPLEX_DEFINE_JSON_STRUCT(IDComponent, ID)

	// Tag
	PERPLEX_DEFINE_JSON_STRUCT(TagComponent, Tag)

	// Enable
	PERPLEX_DEFINE_JSON_STRUCT(EnableComponent, Enabled)

	// Transform
	PERPLEX_DEFINE_JSON_STRUCT(TransformComponent, Position, Rotation, Scale)

	// Sprite Renderer
	PERPLEX_DEFINE_JSON_STRUCT(Sprite, ColorAsset, EmissionAsset)
	PERPLEX_DEFINE_JSON_STRUCT(SpriteRendererComponent, SpriteAsset, Color, EmissionSpriteAsset, Emission)

	// Box Renderer
	PERPLEX_DEFINE_JSON_STRUCT(BoxRendererComponent, Bounds, Color, Emission, PixelPerfect)

	// Circle Renderer
	PERPLEX_DEFINE_JSON_STRUCT(CircleRendererComponent, Radius, Thickness, Color, Emission, PixelPerfect)

	// Line Renderer
	PERPLEX_DEFINE_JSON_STRUCT(LineRendererComponent, Start, End, Color, Emission, PixelPerfect)

	// Camera
	PERPLEX_DEFINE_JSON_STRUCT(CameraComponent, Primary, Zoom, Background, PixelsPerUnit, PixelPerfect, ScalingMode)

	// Script
	PERPLEX_DEFINE_JSON_STRUCT(ScriptComponent, ScriptAsset, Properties, AlwaysReceiveUpdates)

	// Box Collider
	PERPLEX_DEFINE_JSON_STRUCT(BoxColliderComponent, Scale)

	// Physics Body
	PERPLEX_DEFINE_JSON_STRUCT(PhysicsBodyComponent, GravityScale, Density, Friction)

	// Perpixel Renderer
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelShapeInfo, CircleRadius, RectSize, ColorAsset, EmissionAsset, Color, Emission)
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelShape, Info, Type)
	PERPLEX_DEFINE_JSON_STRUCT(PerpixelRendererComponent, Shape)

	// Sprite Animator
	PERPLEX_DEFINE_JSON_STRUCT(SpriteAnimatorComponent, Sprites, Speed, PlayOnStart)

	// Text Renderer
	PERPLEX_DEFINE_JSON_STRUCT(TextComponent, FontAsset, Text, Color, Emission, HorizontalAlignment, VerticalAlignment, HorizontalSpacing, VerticalSpacing)

	// Misc
	PERPLEX_DEFINE_JSON_STRUCT(EntityData, ID, Tag)
	PERPLEX_DEFINE_JSON_STRUCT(EntityNode, ID, Index, ParentID, ChildIDs)
}