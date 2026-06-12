#pragma once

#include <Perplex/Components/ComponentLabelers.h>
#include <Perplex/Components/ComponentDrawers.h>
#include <Perplex/ImGui/GuiSelection.h>
#include <Perplex/Scene/Entity.h>

#include <vector>
#include <memory>
#include <string>

namespace Perplex
{
	class ComponentKindConcept
	{
	public:
		virtual std::unique_ptr<ComponentKindConcept> Clone() = 0;

		virtual void Add(Entity& entity) = 0;
		virtual void Add(std::vector<Entity>& entities) = 0;
		virtual bool Has(Entity& entity) = 0;
		virtual void DrawSelection(std::vector<Entity>& entitySelection) = 0;
		virtual std::string Label() = 0;
	};

	template<typename T>
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
		bool Has(Entity& entity) const { return m_Pimpl->Has(entity); }
		void DrawSelection(std::vector<Entity>& entitySelection) const { m_Pimpl->DrawSelection(entitySelection); }
		std::string Label() const { return m_Pimpl->Label(); }

	private:
		std::unique_ptr<ComponentKindConcept> m_Pimpl;
	};
}