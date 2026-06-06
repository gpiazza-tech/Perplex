#include <Perplex/pch.h>
#include <Perplex/Components/ComponentRegistry.h>

#include <Perplex/Components/Component.h>
#include <Perplex/Scene/Components.h>

#include <vector>

namespace Perplex
{
	const std::vector<Component>& ComponentRegistry::GetAll()
	{
		static std::vector<Component> components
		{
			Component{ IDComponent{} },
			Component{ TagComponent{} },
			Component{ TransformComponent{} },
			Component{ SpriteRendererComponent{} },
			Component{ CameraComponent{} },
			Component{ ScriptComponent{} },
			Component{ PerpixelRendererComponent{} },
			Component{ BoxColliderComponent{} },
			Component{ PhysicsBodyComponent{} },
		};

		return components;
	}
}