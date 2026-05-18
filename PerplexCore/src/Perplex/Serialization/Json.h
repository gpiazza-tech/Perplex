#pragma once

#include <array>
#include <cstddef>

namespace Perplex
{
	class Json
	{
	public:
		Json();
		~Json();

		template<typename T>
		T TryGet(const char* name);


	private:
		std::array<std::byte, 16> m_JsonBytes;
	};
}