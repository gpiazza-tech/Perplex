#pragma once

#include "ComponentRefModel.h"

#include <memory>
#include <string>

namespace Holloware
{
	class ComponentConcept;

	class Component
	{
	public:
		template<typename ComponentT>
		Component(ComponentT& component)
		{
			m_Pimpl = std::make_unique<ComponentRefModel<ComponentT>>(component);
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
	private:
		std::unique_ptr<ComponentConcept> m_Pimpl;
	};
}