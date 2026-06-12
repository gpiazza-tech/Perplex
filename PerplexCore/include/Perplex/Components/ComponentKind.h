#pragma once

#include <Perplex/Components/ComponentLabelers.h>
#include <Perplex/Components/ComponentDrawers.h>
#include <Perplex/Components/ComponentSerializers.h>
#include <Perplex/ImGui/GuiSelection.h>
#include <Perplex/Scene/Entity.h>

#include <vector>
#include <memory>
#include <string>

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
		Label(t);
	};

	class ComponentKindConcept
	{
	public:
		virtual std::unique_ptr<ComponentKindConcept> Clone() = 0;

		virtual void Add(Entity& entity) = 0;
		virtual void Add(std::vector<Entity>& entities) = 0;
		virtual void Copy(Entity& from, Entity& to) = 0;
		virtual void Remove(Entity& entity) = 0;
		virtual void Remove(std::vector<Entity>& entities) = 0;
		virtual bool Has(Entity& entity) = 0;

		virtual void DrawSelection(std::vector<Entity>& entitySelection) = 0;

		virtual void ToJson(nlohmann::json& json, Entity& entity) = 0;
		virtual void FromJson(const nlohmann::json& json, Entity& entity) = 0;

		virtual std::string Label() = 0;
	};

	template<ComponentKindTemplateConcept T>
	class ComponentKindModel : public ComponentKindConcept
	{
	public:
		std::unique_ptr<ComponentKindConcept> Clone() override
		{
			return std::make_unique<ComponentKindModel>(*this);
		}

		void Add(Entity& entity) override
		{
			entity.AddComponent<T>();
		}

		void Add(std::vector<Entity>& entities) override
		{
			for (auto& entity : entities)
			{
				if (!entity.HasComponent<T>())
					entity.AddComponent<T>();
			}
		}

		void Copy(Entity& from, Entity& to) override
		{
			if (from.HasComponent<T>() && !to.HasComponent<T>())
				to.AddComponent<T>(from.GetComponent<T>());
		}

		void Remove(Entity& entity) override
		{
			entity.RemoveComponent<T>();
		}

		void Remove(std::vector<Entity>& entities) override
		{
			for (auto& entity : entities)
			{
				if (entity.HasComponent<T>())
					entity.RemoveComponent<T>();
			}
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
			T tag{};
			std::string label = Perplex::Label(tag);

			if (entity.HasComponent<T>())
				json[label] = entity.GetComponent<T>();
		}

		void FromJson(const nlohmann::json& json, Entity& entity) override
		{
			T tag{};
			std::string label = Perplex::Label(tag);

			if (json.contains(label) && !entity.HasComponent<T>())
				entity.AddComponent<T>(json[label]);
			else if (json.contains(label) && entity.HasComponent<T>())
				entity.GetComponent<T>() = json[label];
		}

		std::string Label() override
		{
			T tag{};
			return Perplex::Label(tag);
		}
	};
 
	class ComponentKind
	{
	public:
		template<typename T>
		ComponentKind(T tag) : m_Pimpl{ std::make_unique<ComponentKindModel<T>>() } {}
		ComponentKind(const ComponentKind& other) : m_Pimpl{ other.m_Pimpl->Clone() } {}

		void Add(Entity& entity) const { m_Pimpl->Add(entity); }
		void Add(std::vector<Entity>& entities) const { m_Pimpl->Add(entities); }
		void Copy(Entity& from, Entity& to) const { m_Pimpl->Copy(from, to); }
		void Remove(Entity& entity) const { m_Pimpl->Remove(entity); }
		void Remove(std::vector<Entity>& entities) const { m_Pimpl->Remove(entities); }
		bool Has(Entity& entity) const { return m_Pimpl->Has(entity); }

		void DrawSelection(std::vector<Entity>& entitySelection) const { m_Pimpl->DrawSelection(entitySelection); }

		void ToJson(nlohmann::json& json, Entity& entity) const { m_Pimpl->ToJson(json, entity); }
		void FromJson(const nlohmann::json& json, Entity& entity) const { m_Pimpl->FromJson(json, entity); }

		std::string Label() const { return m_Pimpl->Label(); }

	private:
		std::unique_ptr<ComponentKindConcept> m_Pimpl;
	};
}