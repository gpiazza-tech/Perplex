#pragma once

#include "ComponentLabelers.h"
#include "ComponentDrawers.h"

#include <memory>
#include <string>

namespace Holloware
{
	class ComponentConcept
	{
	public:
		virtual ~ComponentConcept() = default;

		virtual std::unique_ptr<ComponentConcept> Clone() const = 0;

		virtual std::string Label() = 0;
		virtual void Draw() = 0;
		virtual void Serialize() = 0;

		virtual void Remove() = 0;
		virtual bool EqualsType(ComponentConcept& other) = 0;
	};

	template <typename ComponentT, typename GetStrategy, typename RemoveStrategy>
	class ComponentRefModel : public ComponentConcept
	{
	public:
		ComponentRefModel(ComponentT& tag, GetStrategy getter, RemoveStrategy remover)
			: m_Getter(getter), m_Remover(remover) {}

		std::unique_ptr<ComponentConcept> Clone() const override
		{
			return std::make_unique<ComponentRefModel>(*this);
		}

		std::string Label() override
		{
			ComponentT& component = m_Getter();
			return Holloware::Label(component);
		}

		void Draw() override
		{
			ComponentT& component = m_Getter();
			Holloware::Draw(component);
		}

		void Serialize() override
		{
		}

		void Remove() override
		{
			m_Remover();
		}

		bool EqualsType(ComponentConcept& other) override
		{
			return other.Label() == Label();
		}
	private:
		GetStrategy m_Getter;
		RemoveStrategy m_Remover;
	};

	class Component
	{
	public:
		template<typename ComponentT>
		explicit Component(ComponentT tag)
		{
			using EmptyGetStrategy = ComponentT&(*)();
			using EmptyRemoveStrategy = void(*)();
			EmptyGetStrategy emptyGetStrategy = []() -> ComponentT&
				{
					ComponentT c{};
					return c;
				};
			EmptyRemoveStrategy emptyRemoveStrategy = []() {};
			m_Pimpl = std::make_unique<ComponentRefModel<ComponentT, EmptyGetStrategy, EmptyRemoveStrategy>>(tag, emptyGetStrategy, emptyRemoveStrategy);
		}

		template<typename ComponentT, typename GetStrategy, typename RemoveStrategy>
		explicit Component(ComponentT& component, GetStrategy getter, RemoveStrategy remover)
		{
			m_Pimpl = std::make_unique<ComponentRefModel<ComponentT, GetStrategy, RemoveStrategy>>(component, getter, remover);
		}

		Component(const Component& other) : m_Pimpl(other.m_Pimpl->Clone()) {}
		Component& operator= (const Component& other)
		{
			Component copy{ other };
			m_Pimpl.swap(copy.m_Pimpl);
			return *this;
		}

		~Component() = default;
		Component(Component&&) = default;
		Component& operator= (Component&&) = default;

		void Draw() { m_Pimpl->Draw(); }
		std::string Label() const { return m_Pimpl->Label(); }
		void Serialize() const { m_Pimpl->Serialize(); }

		void Remove() { m_Pimpl->Remove(); }
		bool operator==(const Component& other) const { return m_Pimpl->EqualsType(*other.m_Pimpl.get()); }
	private:
		std::unique_ptr<ComponentConcept> m_Pimpl;
	};
}