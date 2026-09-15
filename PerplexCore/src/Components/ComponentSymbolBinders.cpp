#include <Perplex/pch.h>
#include <Perplex/Components/ComponentSymbolBinders.h>

#include <Perplex/Components/Components.h>
#include <Perplex/Scripting/CUnit.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scripting/Interpreter.h>
#include <Perplex/Scripting/ScriptInstance.h>

#define PX_MEMBER_POINTER_SYMBOL(component, member) unit.AddSymbol(#component"_Get"#member"Ptr", +[](Entity entity) { return entity.HasComponent<component>() ? &entity.GetComponent<component>().member : NULL; });

namespace Perplex
{
	void BindSymbols(CUnit& unit, IDComponent)
	{
	}

	void BindSymbols(CUnit& unit, TagComponent)
	{
	}

	void BindSymbols(CUnit& unit, EnableComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(EnableComponent, Enabled);
	}

	void BindSymbols(CUnit& unit, TransformComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(TransformComponent, Position);
		PX_MEMBER_POINTER_SYMBOL(TransformComponent, Rotation);
		PX_MEMBER_POINTER_SYMBOL(TransformComponent, Scale);
	}

	void BindSymbols(CUnit& unit, SpriteRendererComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(SpriteRendererComponent, Color);
		PX_MEMBER_POINTER_SYMBOL(SpriteRendererComponent, SpriteAsset);
		PX_MEMBER_POINTER_SYMBOL(SpriteRendererComponent, Emission);
		PX_MEMBER_POINTER_SYMBOL(SpriteRendererComponent, EmissionSpriteAsset);
	}

	void BindSymbols(CUnit& unit, BoxRendererComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(BoxRendererComponent, Bounds);
		PX_MEMBER_POINTER_SYMBOL(BoxRendererComponent, Color);
		PX_MEMBER_POINTER_SYMBOL(BoxRendererComponent, Emission);
		PX_MEMBER_POINTER_SYMBOL(BoxRendererComponent, PixelPerfect);
	}

	void BindSymbols(CUnit& unit, CircleRendererComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(CircleRendererComponent, Radius);
		PX_MEMBER_POINTER_SYMBOL(CircleRendererComponent, Thickness);
		PX_MEMBER_POINTER_SYMBOL(CircleRendererComponent, Color);
		PX_MEMBER_POINTER_SYMBOL(CircleRendererComponent, Emission);
		PX_MEMBER_POINTER_SYMBOL(CircleRendererComponent, PixelPerfect);
	}

	void BindSymbols(CUnit& unit, LineRendererComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(LineRendererComponent, Start);
		PX_MEMBER_POINTER_SYMBOL(LineRendererComponent, End);
		PX_MEMBER_POINTER_SYMBOL(LineRendererComponent, Color);
		PX_MEMBER_POINTER_SYMBOL(LineRendererComponent, Emission);
		PX_MEMBER_POINTER_SYMBOL(LineRendererComponent, PixelPerfect);
	}

	void BindSymbols(CUnit& unit, CameraComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(CameraComponent, Background);
		PX_MEMBER_POINTER_SYMBOL(CameraComponent, PixelsPerUnit);
		PX_MEMBER_POINTER_SYMBOL(CameraComponent, Primary);
		PX_MEMBER_POINTER_SYMBOL(CameraComponent, Zoom);
		PX_MEMBER_POINTER_SYMBOL(CameraComponent, PixelPerfect);
	}

	void BindSymbols(CUnit& unit, ScriptComponent)
	{
		unit.AddSymbol("ScriptComponent_TryCall", +[](Entity entity, const char* name) 
			{
				std::optional<AngelObject> instance = entity.GetScene()->GetSystem<Interpreter>().GetInstance(entity.GetUUID());
				if (instance)
					instance->Call<void>(name);
			});
	}

	void BindSymbols(CUnit& unit, PerpixelRendererComponent)
	{
	}

	void BindSymbols(CUnit& unit, BoxColliderComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(BoxColliderComponent, Scale);
	}

	void BindSymbols(CUnit& unit, PhysicsBodyComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(PhysicsBodyComponent, Density);
		PX_MEMBER_POINTER_SYMBOL(PhysicsBodyComponent, Friction);
		PX_MEMBER_POINTER_SYMBOL(PhysicsBodyComponent, GravityScale);
		PX_MEMBER_POINTER_SYMBOL(PhysicsBodyComponent, LockRotation);
	}

	void BindSymbols(CUnit& unit, SpriteAnimatorComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(SpriteAnimatorComponent, PlayOnStart);
		PX_MEMBER_POINTER_SYMBOL(SpriteAnimatorComponent, Speed);
	}

	void BindSymbols(CUnit& unit, TextComponent)
	{
		PX_MEMBER_POINTER_SYMBOL(TextComponent, Color);
		PX_MEMBER_POINTER_SYMBOL(TextComponent, Emission);
		PX_MEMBER_POINTER_SYMBOL(TextComponent, FontAsset);
	}
}