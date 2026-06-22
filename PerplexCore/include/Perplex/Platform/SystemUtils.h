#pragma once

#include <filesystem>

namespace Perplex
{
	std::filesystem::path NativePath(const std::filesystem::path& path);
	void OpenFilesystemGui(const std::filesystem::path& path);
}