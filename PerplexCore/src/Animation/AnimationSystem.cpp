#include <Perplex/pch.h>
#include <Perplex/Animation/AnimationSystem.h>

#include <Perplex/Animation/AnimatorInstance.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Core/Core.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Scene/Entity.h>

namespace Perplex
{
	void AnimationSystem::OnSceneStart()
	{
		for (auto& [uuid, instance] : m_AnimatorMap)
		{
			Entity entity = m_Scene->GetEntity(uuid);
			instance.Playing = entity.GetComponent<SpriteAnimatorComponent>().PlayOnStart;
		}
	}

	void AnimationSystem::OnSceneUpdate(Timestep ts)
	{
		for (auto& [uuid, instance] : m_AnimatorMap)
		{
			Entity entity = m_Scene->GetEntity(uuid);
			const SpriteAnimatorComponent& spriteAnimator = entity.GetComponent<SpriteAnimatorComponent>();

			if (instance.Playing)
				instance.Time += ts.GetSeconds() * spriteAnimator.Speed;
			
			size_t spriteIndex = static_cast<size_t>(instance.Time) % spriteAnimator.Sprites.size();
			if (entity.HasComponent<SpriteRendererComponent>())
			{
				SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
				spriteRenderer.SpriteAsset = spriteAnimator.Sprites.at(spriteIndex).ColorAsset;
				spriteRenderer.EmissionSpriteAsset = spriteAnimator.Sprites.at(spriteIndex).EmissionAsset;
			}
		}
	}

	void AnimationSystem::OnSceneStop()
	{
		
	}

	void AnimationSystem::OnComponentAdded(Entity entity)
	{
		UUID uuid = entity.GetUUID();
		HW_CORE_ASSERT(!m_AnimatorMap.contains(uuid), "AnimatorMap already contains entity!");
		m_AnimatorMap[uuid] = AnimatorInstance{};

	}

	void AnimationSystem::OnComponentRemoved(Entity entity)
	{
		UUID uuid = entity.GetUUID();
		HW_CORE_ASSERT(m_AnimatorMap.contains(uuid), "AnimatorMap does not contain entity!");
		m_AnimatorMap.erase(uuid);
	}

}