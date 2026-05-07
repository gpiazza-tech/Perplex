#pragma once

#include <filesystem>

namespace Holloware
{
	std::filesystem::path Resource(const std::filesystem::path relative)
	{
		return std::filesystem::path(PX_RES_PATH) / relative;
	}
}