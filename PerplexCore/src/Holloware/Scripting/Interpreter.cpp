#include <pch.h>
#include "Interpreter.h"

#include "ScriptProperty.h"

#include <string>
#include <vector>

namespace Holloware
{
	namespace fs = std::filesystem;

	void Interpreter::Begin()
	{
	}

	void Interpreter::End()
	{
	}

	static int find_first_break(const std::string str, int start)
	{
		int spaceIndex = (int)str.find_first_of(' ', start);
		int scIndex = (int)str.find_first_of(';', start);
		int returnIndex = (int)str.find_first_of('\n', start);

		if (spaceIndex == -1) spaceIndex = INT32_MAX;
		if (scIndex == -1) scIndex = INT32_MAX;
		if (returnIndex == -1) returnIndex = INT32_MAX;

		// return the smallest value
		return (spaceIndex < scIndex && spaceIndex < returnIndex) ? spaceIndex : (scIndex < returnIndex) ? scIndex : returnIndex;
	}

	static int find_first_not_break(const std::string str, int start)
	{
		int spaceIndex = (int)str.find_first_not_of(' ', start);
		int scIndex = (int)str.find_first_not_of(';', start);
		int returnIndex = (int)str.find_first_not_of('\n', start);

		// return the smallest value
		return (spaceIndex < scIndex && spaceIndex < returnIndex) ? spaceIndex : (scIndex < returnIndex) ? scIndex : returnIndex;
	}

	static std::string cutstr(const std::string str, int start, int end)
	{
		return str.substr(0, start) + str.substr(end, str.length() - 1);
	}

	std::vector<ScriptProperty> Interpreter::FindProperties(const std::string& src)
	{
		std::vector<ScriptProperty> properties = std::vector<ScriptProperty>();

		std::string sub = src;
		size_t propertyIndex = src.find("PROPERTY ");

		while (propertyIndex != std::string::npos)
		{
			int i = propertyIndex + 9;
			size_t endIndex = sub.find_first_of(';', i);

			i = sub.find_first_not_of(' ', i);

			if (sub.find("struct") == i)
			{
				i += 6;
			}

			i = sub.find_first_not_of(' ', i);

			std::string type = sub.substr(i, sub.find_first_of(' ', i) - i);

			i = find_first_break(sub, i);
			i = sub.find_first_not_of(' ', i);

			std::string name = sub.substr(i, find_first_break(sub, i) - i);

			i = find_first_break(sub, i);
			i = sub.find_first_not_of(' ', i);

			std::string defaultLabel = "";
			if (sub[i] == '=')
			{
				i = sub.find_first_not_of(' ', i + 1); // + 1 to skip the '=' which i is on now
				defaultLabel = sub.substr(i, endIndex - i);
			}

			ScriptProperty property = ScriptProperty(name, type, defaultLabel);
			properties.push_back(property);
			propertyIndex = src.find("PROPERTY ", i);
		}

		return properties;
	}

	std::string Interpreter::TrimProperties(const std::string& src)
	{
		std::string sub = src;
		size_t propertyIndex = src.find("PROPERTY ");

		while (propertyIndex != std::string::npos)
		{
			size_t i = propertyIndex + 9;
			size_t endIndex = sub.find_first_of(';', i);

			size_t equalsIndex = sub.find_first_of('=', i);

			if (equalsIndex > endIndex)
			{
				i = endIndex + 1;
			}

			else
			{
				sub = cutstr(sub, equalsIndex, endIndex);
				i = equalsIndex;
			}

			propertyIndex = src.find("PROPERTY ", i);
		}

		return sub;
	}
}