#pragma once

#include <Holloware/Core/UUID.h>
#include <Holloware/Core/HollowareObject.h>
#include <Holloware/Assets/Asset.h>
#include <Holloware/Scene/SceneCamera.h>
#include <Holloware/Scripting/ScriptInstance.h>
#include <Holloware/Scripting/ScriptData.h>
#include <rendering/Camera.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <filesystem>
#include <string>
#include <vector>

namespace Holloware
{
	class SerializedField;

	struct EntityData
	{
		UUID ID = 0;
		std::string Tag = "";
	};

	struct IDComponent : public HollowareObject
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		void DrawGui() override;
	};

	struct TagComponent : public HollowareObject
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) { }

		void DrawGui() override;
	};

	struct TransformComponent : public HollowareObject
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

		void DrawGui() override;
	};

	struct SpriteRendererComponent : public HollowareObject
	{
		Asset SpriteAsset = Asset();
		glm::vec4 Color = glm::vec4(1.0f);

		Asset EmissionSpriteAsset = Asset();
		float Emission = 0.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

		void DrawGui() override;
	};

	struct CameraComponent : public HollowareObject
	{
		bool Primary = true;
		float Zoom = 10.0f;
		glm::vec4 Background = { 0.0f, 0.05f, 0.2f, 1.0f };
		int PixelsPerUnit = 16; 
		bool PixelPerfect = true;
		pxr::ScalingMode ScalingMode = pxr::ScalingMode::Width;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void DrawGui() override;
	};

	class ScriptProperty;

	struct ScriptComponent : public HollowareObject
	{
		Asset ScriptAsset;
		ScriptInstance Instance;
		std::vector<ScriptProperty> Properties;

		ScriptComponent()
			: ScriptAsset(Asset()), Instance(ScriptInstance()), Properties(std::vector<ScriptProperty>())
		{
		}
		ScriptComponent(const ScriptComponent&) = default;
		ScriptComponent(const std::filesystem::path& filepath)
			: ScriptAsset(Asset(filepath)), Instance(ScriptInstance()), Properties(ScriptAsset.GetData<ScriptData>()->Properties)
		{
		}

		void SyncProperties()
		{
			std::vector<ScriptProperty> oldProperties = Properties;
			Properties = ScriptAsset.GetData<ScriptData>()->Properties;

			for (auto& oldProperty : oldProperties)
			{
				for (auto& newProperty : Properties)
				{
					newProperty.TrySync(oldProperty);
				}
			}
		}

		void DrawGui() override;
	};
}