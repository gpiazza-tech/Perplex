#pragma once

#include <filesystem>

namespace Holloware
{
	class Project
	{
	public:
		Project(const char* path);

		const std::filesystem::path& GetProjectPath() const { return m_ProjectPath; }
		const std::filesystem::path& GetAssetsPath() const { return m_AssetsPath; }

		std::filesystem::path EngineRes(const std::filesystem::path& relative) const;
	private:
		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_AssetsPath;
	};
}