#pragma once

#include <Perplex/Core/UUID.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Scripting/ScriptData.h>
#include <Perplex/Perpixel/PerpixelSpawnShape.h>
#include <Perplex/Text/Alignment.h>
#include <pxr/pxr.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <filesystem>
#include <string>
#include <vector>

/*
*	-----------------------------------------------
*	A new component type can be added in FOUR STEPS
*	-----------------------------------------------
* 
*	1.	Define Struct	- Define the struct in this file, including default and copy constructors
*	2.	Draw Function	- Create a free function in ComponentDrawers with signature Draw(GuiSelection<NewComponentType>)
*	3.	Serialization	- Add an entry to ComponentSerializers.h using the PERPLEX_DEFINE_JSON_STRUCT macro
*	4.	Add to Registry	- Add a ComponentKind entry to the appropriate group in ComponentRegistry.cpp
* 
*/

namespace Perplex
{
	// Utility Structs

	struct EntityData
	{
		UUID ID = 0;
		std::string Tag = "";
	};

	struct Sprite
	{
		Asset ColorAsset;
		Asset EmissionAsset;

		friend bool operator==(const Sprite& lhs, const Sprite& rhs) { return lhs.ColorAsset == rhs.ColorAsset && lhs.EmissionAsset == rhs.EmissionAsset; }
	};

	// Components

	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) { }
	};

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Position(translation) { }

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		Asset SpriteAsset = Asset();
		glm::vec4 Color = glm::vec4(1.0f);

		Asset EmissionSpriteAsset = Asset();
		float Emission = 0.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		bool Primary = true;
		float Zoom = 10.0f;
		glm::vec4 Background = { 0.0f, 0.05f, 0.2f, 1.0f };
		int PixelsPerUnit = 16; 
		bool PixelPerfect = true;
		pxr::ScalingMode ScalingMode = pxr::ScalingMode::Width;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class ScriptProperty;

	struct ScriptComponent
	{
		Asset ScriptAsset;
		std::vector<ScriptProperty> Properties;

		ScriptComponent()
			: ScriptAsset(Asset()), Properties(std::vector<ScriptProperty>())
		{ }
		ScriptComponent(const ScriptComponent& other) = default;
		ScriptComponent(const std::filesystem::path& filepath)
			: ScriptAsset(Asset(filepath)), Properties(ScriptAsset.GetData<ScriptData>()->Properties)
		{ }
	};

	struct PerpixelRendererComponent
	{
		PerpixelShape Shape{};

		PerpixelRendererComponent() = default;
		PerpixelRendererComponent(const PerpixelRendererComponent&) = default;
		PerpixelRendererComponent(const PerpixelShape& shape) : Shape(shape) { }
	};

	struct BoxColliderComponent
	{
		glm::vec2 Scale{ 1.0f, 1.0f };

		BoxColliderComponent() = default;
		BoxColliderComponent(const BoxColliderComponent&) = default;
	};

	struct PhysicsBodyComponent
	{
		float GravityScale{ 1.0f };
		float Density{ 1.0f };
		float Friction{ 0.3f };

		PhysicsBodyComponent() = default;
		PhysicsBodyComponent(const PhysicsBodyComponent&) = default;
	};

	struct SpriteAnimatorComponent
	{
		float Speed{ 1.0f };
		bool PlayOnStart{ true };
		std::vector<Sprite> Sprites{};

		SpriteAnimatorComponent() = default;
		SpriteAnimatorComponent(const SpriteAnimatorComponent&) = default;
	};

	struct TextComponent
	{
		Asset FontAsset{};
		std::string Text{};
		glm::vec4 Color = glm::vec4{ 1.0f };
		float Emission = 0.0f;

		HorizontalAlignment HorizontalAlignment{};
		VerticalAlignment VerticalAlignment{};
		float HorizontalSpacing{};
		float VerticalSpacing{};

		TextComponent() = default;
		TextComponent(const TextComponent&) = default;
	};
}