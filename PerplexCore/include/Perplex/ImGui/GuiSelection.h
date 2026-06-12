#pragma once

#include <Perplex/Core/Core.h>

#include <vector>
#include <type_traits>

namespace Perplex
{
	template<typename T>
	class GuiSelection
	{
	public:
		GuiSelection() {}
		GuiSelection(T& ref) { Add(ref); }

		void Add(T& reference)
		{
			m_Refs.emplace_back(reference);
		}

		template<typename NewT>
		GuiSelection<NewT> GetSubSelection(NewT&(*getter)(T&))
		{
			GuiSelection<NewT> subSelection;

			for (auto& selectionRef : m_Refs)
				subSelection.Add(getter(selectionRef));

			return subSelection;
		}

		bool Synced()
		{
			for (auto& selectionRef : m_Refs)
				if (selectionRef.get() != m_Refs.at(0).get())
					return false;

			return true;
		}

		T GetValue()
		{
			HW_CORE_ASSERT(Synced(), "Calling GetValue on a non-synced GuiSelection object is not allowed!");
			HW_CORE_ASSERT(m_Refs.size() > 0, "Calling GetValue on an empty GuiSelection is not allowed!");

			return m_Refs.at(0).get();
		}

		T& At(size_t index)
		{
			return m_Refs.at(index).get();
		}

		size_t Size()
		{
			return m_Refs.size();
		}

		void SetAll(const T& value)
		{
			for (auto& selectionRef : m_Refs)
				selectionRef.get() = value;
		}

		std::vector<std::reference_wrapper<T>>::iterator begin() { return m_Refs.begin(); }
		std::vector<std::reference_wrapper<T>>::iterator end() { return m_Refs.end(); }
		std::vector<std::reference_wrapper<T>>::const_iterator begin() const { return m_Refs.begin(); }
		std::vector<std::reference_wrapper<T>>::const_iterator end() const { return m_Refs.end(); }
	private:
		std::vector<std::reference_wrapper<T>> m_Refs{};
	};
}

#define PERPLEX_SUBSELECTION(selection, subType, subMember) selection.GetSubSelection<subType>([](auto& obj) -> subType& { return obj.subMember; })