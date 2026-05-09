#pragma once

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
	};
}