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

		virtual std::string Label() const = 0;
		virtual void Draw() = 0;
		virtual void Serialize() = 0;

		virtual void Remove() = 0;
		virtual bool EqualsType(const ComponentConcept& other) const = 0;
	};

	template <typename ComponentT, typename RemoveStrategy>
	class ComponentRefModel : public ComponentConcept
	{
	public:
		ComponentRefModel(ComponentT& component, RemoveStrategy removeStrategy) 
			: m_Component(component), m_RemoveStrategy(removeStrategy) {}

		std::unique_ptr<ComponentConcept> Clone() const override
		{
			return std::make_unique<ComponentRefModel>(*this);
		}

		std::string Label() const override
		{
			return Holloware::Label(m_Component);
		}

		void Draw() override
		{
			Holloware::Draw(m_Component);
		}

		void Serialize() override
		{
		}

		void Remove() override
		{
			m_RemoveStrategy();
		}

		bool EqualsType(const ComponentConcept& other) const override
		{
			return other.Label() == Label();
		}
	private:


		ComponentT& m_Component;
		RemoveStrategy m_RemoveStrategy;
	};

	class Component
	{
	public:
		template<typename ComponentT>
		explicit Component(ComponentT tag)
		{
			using EmptyRemoveStrategy = void(*)();
			m_Pimpl = std::make_unique<ComponentRefModel<ComponentT, EmptyRemoveStrategy>>(tag, EmptyRemoveStrategy{});
		}

		template<typename ComponentT, typename RemoveStrategy>
		explicit Component(ComponentT& component, RemoveStrategy removeStrategy)
		{
			m_Pimpl = std::make_unique<ComponentRefModel<ComponentT, RemoveStrategy>>(component, removeStrategy);
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

		void Draw() const { m_Pimpl->Draw(); }
		std::string Label() const { return m_Pimpl->Label(); }
		void Serialize() const { m_Pimpl->Serialize(); }

		void Remove() { m_Pimpl->Remove(); }
		bool operator==(const Component& other) const { return m_Pimpl->EqualsType(*other.m_Pimpl.get()); }
	private:
		std::unique_ptr<ComponentConcept> m_Pimpl;
	};
}