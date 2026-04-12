#pragma once

#include <filesystem>

namespace Holloware
{
	class Project
	{
	public:
		Project(const char* path) : m_ProjectPath(std::filesystem::path(path)), m_AssetsPath(std::filesystem::path(std::filesystem::path(path) / "assets")) {}

		const std::filesystem::path& GetProjectPath() { return m_ProjectPath; }
		const std::filesystem::path& GetAssetsPath() { return m_AssetsPath; }
	private:
		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_AssetsPath;
	};
}