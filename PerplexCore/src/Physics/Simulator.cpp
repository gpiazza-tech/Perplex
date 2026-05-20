#include <Perplex/pch.h>
#include <Perplex/Physics/Simulator.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Scene/Scene.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Scene/Components.h>
#include <Perplex/Core/Timestep.h>

#include <glm/fwd.hpp>
#include <box2d/box2d.h>

#include <bit>
#include <cstdint>

#define WORLD (std::bit_cast<b2WorldId>(m_World))

namespace Perplex
{
	struct Box2DUserData
	{
		glm::vec2 OldBox2DPosition{};
		glm::vec3 OldEntityPosition{};
	};

	void Simulator::Start(Ref<Scene> scene)
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { 0.0f, -10.0f };

		m_World = std::bit_cast<int>(b2CreateWorld(&worldDef));

		auto view = scene->View<BoxColliderComponent>();
		for (auto e : view)
		{
			Entity entity{ e, scene.get() };

			const TransformComponent& globalTransform = entity.GetGlobalTransform();
			const BoxColliderComponent& collider = view.get<BoxColliderComponent>(e);

			glm::vec2 halfSize{ collider.Scale / 2.0f };
			if (entity.HasComponent<SpriteRendererComponent>())
			{
				Ref<pxr::Sprite> colorSprite = entity.GetComponent<SpriteRendererComponent>().SpriteAsset.GetData<pxr::Sprite>();
				if (colorSprite)
					halfSize = { halfSize.x * colorSprite->ScaleFactorX, halfSize.y * colorSprite->ScaleFactorY };
			}

			b2BodyDef bodyDef = b2DefaultBodyDef();
			b2Polygon dynamicBox = b2MakeBox(halfSize.x, halfSize.y);
			b2ShapeDef shapeDef = b2DefaultShapeDef();

			bodyDef.position = { globalTransform.Position.x, globalTransform.Position.y };

			Box2DUserData* userData = new Box2DUserData{};
			userData->OldBox2DPosition = { globalTransform.Position.x, globalTransform.Position.y };
			userData->OldEntityPosition = globalTransform.Position;

			bodyDef.userData = userData;
			bodyDef.rotation = b2MakeRot(globalTransform.Rotation.z);

			if (entity.HasComponent<PhysicsBodyComponent>())
			{
				const PhysicsBodyComponent& physicsBody = entity.GetComponent<PhysicsBodyComponent>();
				bodyDef.type = b2_dynamicBody;
				shapeDef.density = physicsBody.Density;
				shapeDef.material.friction = physicsBody.Friction;
				bodyDef.enableSleep = false;
			}

			else
			{
				bodyDef.type = b2_staticBody; 
			}

			b2BodyId bodyId = b2CreateBody(WORLD, &bodyDef);
			b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

			m_Bodies[entity.GetUUID()] = std::bit_cast<uint64_t>(bodyId);
		}
	}

	void Simulator::Update(Ref<Scene> scene, Timestep ts)
	{
		// TODO: this function should be called on a separate thread with
		//		a consitent framerate!

		float timeStep = ts.GetSeconds();

		// recommended by box2d docs
		int subStepCount = 4;

		b2World_Step(WORLD, timeStep, subStepCount);

		// Update positions
		auto view = scene->View<BoxColliderComponent>();
		for (auto e : view)
		{
			Entity entity{ e, scene.get() };
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			TransformComponent globalTransform = entity.GetGlobalTransform();

			b2BodyId bodyId = std::bit_cast<b2BodyId>(m_Bodies[entity.GetUUID()]);
			b2Vec2 position = b2Body_GetPosition(bodyId);
			b2Rot rotation = b2Body_GetRotation(bodyId);

			Box2DUserData* userData = (Box2DUserData*)b2Body_GetUserData(bodyId);
			glm::vec2 oldBox2DPosition = userData->OldBox2DPosition;
			glm::vec3 oldEntityPosition = userData->OldEntityPosition;

			float angle = b2Rot_GetAngle(b2Body_GetRotation(bodyId));

			glm::vec3 box2dDelta =
				glm::vec3{ position.x, position.y, transform.Position.z } -
				glm::vec3{ oldBox2DPosition.x, oldBox2DPosition.y, transform.Position.z };

			if (box2dDelta != glm::vec3{ 0.0f })
			{
				transform.Position += box2dDelta;
				transform.Rotation.z = b2Rot_GetAngle(rotation);
			}

			globalTransform = entity.GetGlobalTransform();
			glm::vec3 entityDelta =
				glm::vec3{ globalTransform.Position.x, globalTransform.Position.y, globalTransform.Position.z } -
				glm::vec3{ oldEntityPosition.x, oldEntityPosition.y, globalTransform.Position.z };

			if (entityDelta != glm::vec3{ 0.0f })
			{
				globalTransform = entity.GetGlobalTransform();
				rotation = b2Body_GetRotation(bodyId);
				b2Body_SetTransform(bodyId, { globalTransform.Position.x, globalTransform.Position.y }, rotation);
			}

			position = b2Body_GetPosition(bodyId);
			userData->OldBox2DPosition = { position.x, position.y };
			userData->OldEntityPosition = globalTransform.Position;
		}
	}

	void Simulator::Stop(Ref<Scene> scene)
	{
		auto view = scene->View<BoxColliderComponent>();
		for (auto e : view)
		{
			Entity entity{ e, scene.get() };

			b2BodyId bodyId = std::bit_cast<b2BodyId>(m_Bodies[entity.GetUUID()]);
			Box2DUserData* userData = (Box2DUserData*)b2Body_GetUserData(bodyId);

			delete userData;
		}

		b2DestroyWorld(WORLD);
	}

	void Simulator::SetVelocity(Entity entity, glm::vec2 velocity)
	{
		b2BodyId bodyId = std::bit_cast<b2BodyId>(m_Bodies[entity.GetUUID()]);
		b2Body_SetLinearVelocity(bodyId, { velocity.x, velocity.y });
	}
}