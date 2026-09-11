#pragma once

#include <cstddef>
#include <string_view>
#include <array>

namespace Perplex
{
	class ScriptSymbol
	{
	public:
		static constexpr size_t MaxSize{ 24 };
	public:
		ScriptSymbol(std::string_view name) : m_Name(name) { }

		std::string_view GetName() const { return m_Name; }

		void* GetData() { return m_Data.data(); }
		const void* GetData() const { return m_Data.data(); }
	private:
		std::string_view m_Name{};
		std::array<std::byte, MaxSize> m_Data{};
	};
}