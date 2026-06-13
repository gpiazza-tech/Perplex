#include <Perplex/pch.h>
#include <Perplex/Components/ComponentLabelers.h>

#include <Perplex/Scene/Components.h>

#include <string>

namespace Perplex
{
	std::string Label(IDComponent&) { return "ID"; }
	std::string Label(TagComponent&) { return "Tag"; }
	std::string Label(TransformComponent&) { return "Transform"; }
	std::string Label(SpriteRendererComponent&) { return "Sprite Renderer"; }
	std::string Label(CameraComponent&) { return "Camera"; }
	std::string Label(ScriptComponent&) { return "Script"; }
	std::string Label(PerpixelRendererComponent&) { return "Perpixel Renderer"; }
	std::string Label(BoxColliderComponent&) { return "Box Collider"; }
	std::string Label(PhysicsBodyComponent&) { return "Physics Body"; }
	std::string Label(SpriteAnimatorComponent&) { return "Sprite Animator"; }
}