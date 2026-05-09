#pragma once

#include "ComponentConcept.h"

#include <memory>
#include <string>

namespace Holloware
{
	template<typename ComponentT>
	void DrawComponent(ComponentT& component);

	template <typename ComponentT>
	class ComponentRefModel : public ComponentConcept
	{
	public:
		ComponentRefModel(ComponentT& component) : m_Component(component) {}

		std::unique_ptr<ComponentConcept> Clone() const override
		{
			return std::make_unique<ComponentRefModel>(*this);
		}

		std::string Label() override
		{
			return "Unlabeled Component";
		}

		void Draw() override
		{
			DrawComponent<ComponentT>(m_Component);
		}

		void Serialize() override
		{
		}
	private:
		ComponentT& m_Component;
	};
}