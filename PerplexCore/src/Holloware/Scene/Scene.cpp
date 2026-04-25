#include <pch.h>
#include "Scene.h"

#include "Components.h"
#include "EditorCamera.h"
#include "Entity.h"
#include "SceneRenderer.h"
#include <Holloware/Core/Timestep.h>
#include <Holloware/Scripting/ScriptInstance.h>
#include <Holloware/Scripting/ScriptData.h>
#include <Holloware/Assets/Asset.h>
#include <Holloware/Core/Core.h>
#include <Holloware/Core/UUID.h>
#include <Holloware/Debug/Instrumentor.h>
#include "SceneHierarchy.h"
#include <rendering/Camera.h>

#include <entt.hpp>
#include <glm/fwd.hpp>

#include <cstdint>
#include <string>

namespace Holloware
{
	Entity Scene::CreateEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = Entity(m_Registry.create(), this);

		entity.AddComponent<TransformComponent>();

		IDComponent& idc = entity.AddComponent<IDComponent>();
		idc.ID = uuid;
		m_UUIDMap[uuid] = (entt::entity)entity;

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_Hierarchy.Add(EntityNode(entity.GetUUID()), parent);

		return entity;
	}

	Entity Scene::CreateAbstractEntity(const std::string& name, UUID uuid, UUID parent)
	{
		Entity entity = Entity(m_Registry.create(), this);

		IDComponent& idc = entity.AddComponent<IDComponent>();
		idc.ID = uuid;
		m_UUIDMap[uuid] = (entt::entity)entity;

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_Hierarchy.Add(EntityNode(entity.GetUUID()), parent);

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Hierarchy.Remove(entity.GetUUID());

		m_UUIDMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::CopyEntity(Entity entity, UUID parent)
	{
		// copy main
		Entity newEntity = CreateAbstractEntity(entity.GetTag(), UUID(), parent);
		if (entity.HasComponent<TransformComponent>())
			newEntity.AddComponent<TransformComponent>(entity.GetComponent<TransformComponent>());
		if (entity.HasComponent<SpriteRendererComponent>())
			newEntity.AddComponent<SpriteRendererComponent>(entity.GetComponent<SpriteRendererComponent>());
		if (entity.HasComponent<CameraComponent>())
			newEntity.AddComponent<CameraComponent>(entity.GetComponent<CameraComponent>());
		if (entity.HasComponent<ScriptComponent>())
			newEntity.AddComponent<ScriptComponent>(entity.GetComponent<ScriptComponent>());

		// copy children
		for (auto& childID : m_Hierarchy.GetNode(entity.GetUUID()).ChildIDs)
			CopyEntity(GetEntity(childID), newEntity.GetUUID());
	}

	Entity Scene::GetEntity(UUID uuid)
	{
		return Entity(m_UUIDMap[uuid], this);
	}

	void Scene::OnUpdateEditor(Timestep ts, const EditorCamera& camera)
	{
		SceneRenderer::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto handle : group)
		{
			Entity entity = Entity(handle, this);
			auto [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(handle);
			SceneRenderer::RenderSprite(spriteRenderer, entity.GetGlobalTransform());
		}

		SceneRenderer::EndScene();
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
		CameraComponent* mainCamera = nullptr;
		TransformComponent* cameraTransform = nullptr;
		{
			HW_PROFILE_SCOPE("Find Camera");

			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera;
					cameraTransform = &transform;
					break;
				}
			}
		}

		// Render sprites
		if (mainCamera)
		{
			HW_PROFILE_SCOPE("Render Sprites");
			
			pxr::Camera camera({ m_ViewportWidth, m_ViewportHeight }, mainCamera->PixelsPerUnit, mainCamera->Zoom, mainCamera->ScalingMode);
			SceneRenderer::BeginScene(camera, *cameraTransform, mainCamera->Background);

			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto handle : group)
			{
				Entity entity = Entity(handle, this);
				auto [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(handle);
				SceneRenderer::RenderSprite(spriteRenderer, entity.GetGlobalTransform());
			}

			SceneRenderer::EndScene();
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
		SceneRenderer::Resize(width, height);

		m_ViewportWidth = width;
		m_ViewportHeight = height;
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