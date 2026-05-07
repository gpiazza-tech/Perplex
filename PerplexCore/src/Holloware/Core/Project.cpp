#include <pch.h>
#include "Project.h"

#include <filesystem>

namespace Holloware
{
	namespace fs = std::filesystem;

	Project::Project(const char* path) : m_ProjectPath(fs::path(path)), m_AssetsPath(fs::path(fs::path(path) / "assets"))
	{
		// copy res folder into project
		fs::path resPath = std::filesystem::current_path() / "../PerplexCore/res";
		fs::copy(resPath, m_ProjectPath / "engine/res",
			fs::copy_options::recursive | fs::copy_options::overwrite_existing);
	}

	fs::path Project::EngineRes(const fs::path& relative) const
	{
		return m_ProjectPath / "engine/res" / relative;
	}
}