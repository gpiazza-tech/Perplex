#include <pch.h>
#include "Scene.h"

#include "Components.h"
#include "EditorCamera.h"
#include "Entity.h"
#include <Holloware/Core/Timestep.h>
#include <Holloware/Renderer/Renderer2D.h>
#include <Holloware/Renderer/SubTexture2D.h>
#include <Holloware/Scripting/ScriptInstance.h>
#include <Holloware/Scripting/ScriptData.h>
#include <Holloware/Assets/Asset.h>
#include <Holloware/Core/Core.h>
#include <Holloware/Core/UUID.h>
#include <Holloware/Debug/Instrumentor.h>
#include <Holloware/Renderer/Camera.h>

#include <entt.hpp>
#include <glm/fwd.hpp>

#include <cstdint>
#include <string>

namespace Holloware
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name, UUID uuid)
	{
		Entity entity = Entity(m_Registry.create(), this);

		entity.AddComponent<TransformComponent>();

		IDComponent& idc = entity.AddComponent<IDComponent>();
		idc.ID = uuid;
		m_UUIDMap[uuid] = (entt::entity)entity;

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	Entity Scene::CreateAbstractEntity(const std::string& name, UUID uuid)
	{
		Entity entity = Entity(m_Registry.create(), this);

		IDComponent& idc = entity.AddComponent<IDComponent>();
		idc.ID = uuid;
		m_UUIDMap[uuid] = (entt::entity)entity;

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_UUIDMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	Entity Scene::GetEntity(UUID uuid)
	{
		return Entity(m_UUIDMap[uuid], this);
	}

	void Scene::OnUpdateEditor(Timestep ts, const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			if (sprite.TextureAsset)
			{
				Ref<Texture2D> texture = sprite.TextureAsset.GetData<Texture2D>();
				Ref<SubTexture2D> subTexture = CreateRef<SubTexture2D>(texture, glm::vec2(0, 0), glm::vec2(1, 1));
				Renderer2D::DrawQuad(transform.GetTransform(), subTexture, sprite.Color, (int)entity);
			}
			else
			{
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnStartRuntime()
	{
		// Compile Scripts
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = Entity(e, this);

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				Ref<ScriptData> scriptData = sc.ScriptAsset.GetData<ScriptData>();
				if (scriptData) sc.Instance.Compile(scriptData->Source, entity);
			}
		}
		// Call Start
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = Entity(e, this);

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				if (sc.Instance.IsCompiled()) sc.Instance.TryCallStart();
			}
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Call Update
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = Entity(e, this);

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				if (sc.Instance.IsCompiled()) sc.Instance.TryCallUpdate(ts.GetSeconds());
			}
		}

		// Find Camera
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			HW_PROFILE_SCOPE("Find Camera");

			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.GetTransform();
					break;
				}
			}
		}

		// Render sprites
		if (mainCamera)
		{
			HW_PROFILE_SCOPE("Render Sprites");

			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				if (sprite.TextureAsset)
				{
					Ref<Texture2D> texture = sprite.TextureAsset.GetData<Texture2D>();
					Ref<SubTexture2D> subTexture = CreateRef<SubTexture2D>(texture, glm::vec2(0, 0), glm::vec2(1, 1));
					Renderer2D::DrawQuad(transform.GetTransform(), subTexture, sprite.Color, (int)entity);
				}
				else
				{
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnStopRuntime()
	{
		// Call Stop
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = Entity(e, this);

				auto& sc = view.get<ScriptComponent>(e);
				TagComponent& tag = entity.GetComponent<TagComponent>();

				if (sc.Instance.IsCompiled()) sc.Instance.TryCallStop();
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnScriptAssetReimported(Asset asset)
	{
		auto view = m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			auto& sc = view.get<ScriptComponent>(e);

			if ((UUID)sc.ScriptAsset == (UUID)asset)
			{
				sc.SyncProperties();
			}
		}
	}
}