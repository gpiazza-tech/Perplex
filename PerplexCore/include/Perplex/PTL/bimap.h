#pragma once

#include <unordered_map>

namespace Perplex::ptl
{
	template<typename first, typename second>
	class bimap
	{
	public:
		void set_pair(const first& first, const second& second)
		{
			m_FirstMap[first] = second;
			m_SecondMap[second] = first;
		}

		void erase_pair(const first& first, const second& second)
		{
			m_FirstMap.erase(first);
			m_SecondMap.erase(second);
		}

		const second& at(const first& first) const { return m_FirstMap.at(first); }
		const first& at(const second& second) const { return m_SecondMap.at(second); }

		bool contains(const first& value) const { return m_FirstMap.contains(value); }
		bool contains(const second& value) const { return m_SecondMap.contains(value); }

		bool contains_pair(const first& first, const second& second) const
		{
			return m_FirstMap.contains(first) && m_SecondMap.contains(second) && m_FirstMap.at(first) == second && m_SecondMap.at(second) == first;
		}
	private:
		std::unordered_map<first, second> m_FirstMap{};
		std::unordered_map<second, first> m_SecondMap{};
	};
}