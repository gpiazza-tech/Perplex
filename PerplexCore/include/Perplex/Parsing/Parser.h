#pragma once

#include <string>
#include <vector>

namespace Perplex
{
	inline constexpr std::string_view Between(std::string_view str, char start, char end)
	{
		size_t startPos = str.find(start);
		size_t endPos = str.find(end);
		return str.substr(startPos + 1, endPos - startPos - 1);
	}

	inline constexpr std::string_view After(std::string_view str, std::string_view pattern)
	{
		return str.substr(str.find(pattern) + pattern.length() - 1, str.length());
	}

	inline constexpr std::string_view AfterLast(std::string_view str, std::string_view pattern)
	{
		return str.substr(str.find_last_of(pattern) + pattern.length() - 1, str.length());
	}

	inline void ReplaceAll(std::string& str, std::string_view from, std::string_view to)
	{
		size_t start = str.find(from);
		while (start != -1)
		{
			str.replace(start, from.length(), to);
			start = str.find(from);
		}
	}

	inline constexpr std::string_view FirstLine(std::string_view str)
	{
		return str.substr(0, str.find('\n'));
	}

	inline bool IsUppercase(char c)
	{
		return c >= 'A' && c <= 'Z';
	}

	inline bool IsLowercase(char c)
	{
		return c >= 'a' && c <= 'z';
	}

	inline size_t FindUppercase(std::string_view str)
	{
		for (size_t i{}; i < str.length(); ++i)
			if (IsUppercase(str.at(i))) return i;
		return -1;
	}

	inline void ToHumanCase(std::string& str)
	{
		// ReplaceAll(str, "ns", "NS");
	}

	inline std::vector<std::string_view> ParseLines(std::string_view string)
	{
		std::vector<std::string_view> lines{};

		while (string.find('\n') != std::string_view::npos)
		{
			lines.emplace_back(FirstLine(string));
			string = string.substr(string.find('\n') + 1, string.length());
		}

		// last line
		lines.emplace_back(FirstLine(string));
		return lines;
	}

	template<typename T>
	consteval std::string_view GetTypeName()
	{
		/*
			this function uses an MSVC only utility, __FUNCSIG__. This will be replaced 
			by the ^^ operator once the C++26 standard is officially supported
		*/

		std::string_view funcSig = __FUNCSIG__;
		std::string_view nameAndParameters = After(funcSig, " __cdecl ");
		std::string_view cName = Between(nameAndParameters, '<', '>');
		std::string_view fullName = After(cName, " ");
		std::string_view name = AfterLast(fullName, "::");
		return name;
	}
}