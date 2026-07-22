#include <Perplex/pch.h>
#include <Perplex/Perpixel/PerpixelSystem.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Components/Components.h>
#include <Perplex/Perpixel/PerpixelInstance.h>
#include <Perplex/Scripting/Interpreter.h>
#include <Perplex/Scripting/ScriptInstance.h>
#include <c/perplex_pixel.h>

#include <cstdint>
#include <vector>
#include <utility>
#include <functional>

namespace Perplex
{
	void TryCallPerpixelStart(Entity entity, PerpixelInstance& instance)
	{
		Interpreter& interpreter = entity.GetScene()->GetSystem<Interpreter>();
		ScriptInstance* scriptInstance = interpreter.GetInstance(entity.GetUUID());
		if (scriptInstance)
		{
			std::vector<Pixel>& pixels = instance.GetPixels();
			scriptInstance->TryCall("perpixel_start", pixels.data(), pixels.size());
		}
	}

	void TryCallPerpixelUpdate(Timestep ts, Entity entity, PerpixelInstance& instance)
	{
		Interpreter& interpreter = entity.GetScene()->GetSystem<Interpreter>();
		ScriptInstance* scriptInstance = interpreter.GetInstance(entity.GetUUID());
		if (scriptInstance)
		{
			std::vector<Pixel>& pixels = instance.GetPixels();
			scriptInstance->TryCall("perpixel_update", ts.GetSeconds(), pixels.data(), pixels.size());
		}
	}

	void TryCallPerpixelEnd(Entity entity, PerpixelInstance& instance)
	{
		Interpreter& interpreter = entity.GetScene()->GetSystem<Interpreter>();
		ScriptInstance* scriptInstance = interpreter.GetInstance(entity.GetUUID());
		if (scriptInstance)
		{
			scriptInstance->TryCall("perpixel_end");
		}
	}

	void PerpixelSystem::OnSceneStart()
	{
		auto view = m_Scene->View<PerpixelRendererComponent>();
		for (auto e : view)
		{
			Entity entity{ e, m_Scene.get() };
			UUID entityID = entity.GetUUID();
			const PerpixelRendererComponent& perpixelComponent = view.get<PerpixelRendererComponent>(e);

			PerpixelInstance& perpixelInstance = m_PerpixelInstanceMap.at(entityID);
			perpixelInstance.ResetPixels();

			TryCallPerpixelStart(entity, perpixelInstance);
		}
	}

	void PerpixelSystem::OnSceneUpdate(Timestep ts)
	{
		if (m_Scene->IsPaused())
			return;

		auto view = m_Scene->View<PerpixelRendererComponent>();
		for (auto e : view)
		{
			Entity entity{ e, m_Scene.get() };
			if (!entity.HasComponent<TransformComponent>())
				continue;

			UUID entityID = entity.GetUUID();
			HW_CORE_ASSERT(m_PerpixelInstanceMap.find(entityID) != m_PerpixelInstanceMap.end(),
				"Entity {} does not have PerpixelInstance!", (uint64_t)entityID);

			const PerpixelRendererComponent& perpixelComponent = view.get<PerpixelRendererComponent>(e);
			PerpixelInstance& perpixelInstance = m_PerpixelInstanceMap.at(entityID);

			if (perpixelInstance.GetAlivePixelCount() > 0)
				TryCallPerpixelUpdate(ts, entity, perpixelInstance);
			else
				TryCallPerpixelEnd(entity, perpixelInstance);
		}
	}

	void PerpixelSystem::OnSceneStop()
	{

	}


	void PerpixelSystem::OnComponentAdded(Entity entity)
	{
		UUID entityID = entity.GetUUID();
		m_PerpixelInstanceMap.insert(std::pair{ entityID, PerpixelInstance{} });

		PerpixelInstance& perpixelInstance = m_PerpixelInstanceMap.at(entityID);
		PerpixelRendererComponent& perpixelComponent = entity.GetComponent<PerpixelRendererComponent>();

		perpixelInstance.SetSpawnShape(perpixelComponent.Shape);
		perpixelInstance.ResetPixels();

		if (m_Scene->IsPlaying())
			TryCallPerpixelStart(entity, perpixelInstance);
	}

	void PerpixelSystem::OnComponentRemoved(Entity entity)
	{
		UUID entityID = entity.GetUUID();
		m_PerpixelInstanceMap.erase(entityID);
	}

	std::vector<Pixel>& PerpixelSystem::GetPixels(UUID perpixelEntityID)
	{
		static std::vector<Pixel> nullPxls{};

		if (!m_PerpixelInstanceMap.contains(perpixelEntityID))
			return nullPxls;

		return m_PerpixelInstanceMap.at(perpixelEntityID).GetPixels();
	}

	const std::vector<Pixel>& PerpixelSystem::GetPixels(UUID perpixelEntityID) const
	{
		static std::vector<Pixel> nullPxls{};

		if (!m_PerpixelInstanceMap.contains(perpixelEntityID))
			return nullPxls;

		return m_PerpixelInstanceMap.at(perpixelEntityID).GetPixels();
	}

	PerpixelInstance* PerpixelSystem::GetInstance(UUID perpixelEntityID)
	{
		if (!m_PerpixelInstanceMap.contains(perpixelEntityID))
			return nullptr;

		return &m_PerpixelInstanceMap.at(perpixelEntityID);
	}
}