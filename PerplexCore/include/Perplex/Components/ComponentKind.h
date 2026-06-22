#pragma once

#include <Perplex/Core/TypeID.h>
#include <Perplex/Core/Parser.h>
#include <Perplex/Components/ComponentDrawers.h>
#include <Perplex/Components/ComponentSerializers.h>
#include <Perplex/ImGui/GuiSelection.h>
#include <Perplex/Scene/Entity.h>

#include <vector>
#include <memory>
#include <string_view>

namespace Perplex
{
	template<typename T>
	concept ComponentKindTemplateConcept = requires(T t, Entity entity, nlohmann::json& json)
	{
		entity.HasComponent<T>();
		entity.GetComponent<T>();
		entity.AddComponent<T>();
		entity.RemoveComponent<T>();
		to_json(json, t);
		from_json(json, t);
		Draw(t);
		GetTypeID<T>();
		GetTypeName<T>();
	};

	class ComponentKindConcept
	{
	public:
		virtual std::unique_ptr<ComponentKindConcept> Clone() = 0;

		virtual TypeID GetTypeID() = 0;
		virtual std::string_view GetTypeName() = 0;

		virtual void Add(Entity& entity) = 0;
		virtual void Copy(Entity& from, Entity& to) = 0;
		virtual void Remove(Entity& entity) = 0;
		virtual bool Has(Entity& entity) = 0;

		virtual void DrawSelection(std::vector<Entity>& entitySelection) = 0;

		virtual void ToJson(nlohmann::json& json, Entity& entity) = 0;
		virtual void FromJson(const nlohmann::json& json, Entity& entity) = 0;
	};

	template<ComponentKindTemplateConcept T>
	class ComponentKindModel : public ComponentKindConcept
	{
	public:
		std::unique_ptr<ComponentKindConcept> Clone() override
		{
			return std::make_unique<ComponentKindModel>(*this);
		}

		TypeID GetTypeID() override { return Perplex::GetTypeID<T>(); }
		std::string_view GetTypeName() override { return Perplex::GetTypeName<T>(); }

		void Add(Entity& entity) override
		{
			entity.AddComponent<T>();
		}

		void Copy(Entity& from, Entity& to) override
		{
			to.AddComponent<T>(from.GetComponent<T>());
		}

		void Remove(Entity& entity) override
		{
			entity.RemoveComponent<T>();
		}

		bool Has(Entity& entity) override
		{
			return entity.HasComponent<T>();
		}

		void DrawSelection(std::vector<Entity>& entitySelection) override
		{
			GuiSelection<T> component;
			for (auto& entity : entitySelection)
				component.Add(entity.GetComponent<T>());
			Draw(component);
		}

		void ToJson(nlohmann::json& json, Entity& entity) override
		{
			json[Perplex::GetTypeName<T>()] = entity.GetComponent<T>();
		}

		void FromJson(const nlohmann::json& json, Entity& entity) override
		{
			if (!entity.HasComponent<T>())
				entity.AddComponent<T>();

			entity.GetComponent<T>() = json[Perplex::GetTypeName<T>()];
		}
	};
 
	class ComponentKind
	{
	public:
		template<typename T>
		ComponentKind(T tag) : m_Pimpl{ std::make_unique<ComponentKindModel<T>>() } {}
		ComponentKind(const ComponentKind& other) : m_Pimpl{ other.m_Pimpl->Clone() } {}

		TypeID GetTypeID() const { return m_Pimpl->GetTypeID(); }
		std::string_view GetTypeName() const { return m_Pimpl->GetTypeName(); }

		void Add(Entity& entity) const { m_Pimpl->Add(entity); }
		void Copy(Entity& from, Entity& to) const { m_Pimpl->Copy(from, to); }
		void Remove(Entity& entity) const { m_Pimpl->Remove(entity); }
		bool Has(Entity& entity) const { return m_Pimpl->Has(entity); }

		void DrawSelection(std::vector<Entity>& entitySelection) const { m_Pimpl->DrawSelection(entitySelection); }

		void ToJson(nlohmann::json& json, Entity& entity) const { m_Pimpl->ToJson(json, entity); }
		void FromJson(const nlohmann::json& json, Entity& entity) const { m_Pimpl->FromJson(json, entity); }
	private:
		std::unique_ptr<ComponentKindConcept> m_Pimpl;
	};
}