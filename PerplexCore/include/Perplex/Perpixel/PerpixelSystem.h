#pragma once

#include <Perplex/Core/Core.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Scene/SceneSystem.h>
#include <Perplex/Perpixel/PerpixelInstance.h>
#include <c/perplex_pixel.h>

#include <unordered_map>
#include <vector>

namespace Perplex
{
	class Scene;
	class Entity;
	class Component;
	class TypeID;

	void TryCallPerpixelStart(Entity entity, PerpixelInstance& instance);
	void TryCallPerpixelUpdate(Timestep ts, Entity entity, PerpixelInstance& instance);

	class PerpixelSystem : SceneSystem
	{
	public:
		PerpixelSystem(Ref<Scene> scene) : SceneSystem(scene) {}

		TypeID GetComponentTypeID() override { return GetTypeID<PerpixelRendererComponent>(); }

		void OnSceneStart() override;
		void OnSceneUpdate(Timestep ts) override;
		void OnSceneStop() override;

		void OnComponentAdded(Entity entity) override;
		void OnComponentRemoved(Entity entity) override;

		std::vector<pixel>& GetPixels(UUID perpixelEntityID);
		const std::vector<pixel>& GetPixels(UUID perpixelEntityID) const;

		PerpixelInstance* GetInstance(UUID perpixelEntityID);
	private:
		std::unordered_map<UUID, PerpixelInstance> m_PerpixelInstanceMap{};
	};
}   