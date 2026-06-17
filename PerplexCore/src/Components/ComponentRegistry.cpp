#include <Perplex/pch.h>
#include <Perplex/Components/ComponentRegistry.h>

#include <Perplex/Components/ComponentKind.h>
#include <Perplex/Components/Components.h>

#include <vector>

namespace Perplex
{
	const std::vector<ComponentKind> ComponentRegistry::GetBaseKinds()
	{
		std::vector<ComponentKind> components
		{
			ComponentKind{ IDComponent{} },
			ComponentKind{ TagComponent{} },
		};

		return components;
	}

	const std::vector<ComponentKind> ComponentRegistry::GetAdditiveKinds()
	{
		std::vector<ComponentKind> components
		{
			ComponentKind{ TransformComponent{} },
			ComponentKind{ SpriteRendererComponent{} },
			ComponentKind{ CameraComponent{} },
			ComponentKind{ ScriptComponent{} },
			ComponentKind{ PerpixelRendererComponent{} },
			ComponentKind{ BoxColliderComponent{} },
			ComponentKind{ PhysicsBodyComponent{} },
			ComponentKind{ SpriteAnimatorComponent{} },
		};

		return components;
	}

	const std::vector<ComponentKind> ComponentRegistry::GetAllKinds()
	{
		std::vector<ComponentKind> components;

		for (auto& baseComponentKind : GetBaseKinds())
			components.emplace_back(baseComponentKind);

		for (auto& additiveComponentKind : GetAdditiveKinds())
			components.emplace_back(additiveComponentKind);

		return components;
	}
}