#include <Perplex/pch.h>
#include <Perplex/Platform/SystemUtils.h>
#include <Perplex/Parsing/Parser.h>

#include <filesystem>
#include <string>
#include <sstream>
#include <cstdlib>

namespace Perplex
{
	std::filesystem::path NativePath(const std::filesystem::path& path)
	{
		std::string nativePathStr{ path.string() };
		ReplaceAll(nativePathStr, "/", "\\");
		return std::filesystem::path{ nativePathStr };
	}

	void OpenFilesystemGui(const std::filesystem::path& path)
	{
		std::stringstream commandStream;
		commandStream << "explorer " << path.string();

		std::string commandString = commandStream.str();
		std::system(commandString.c_str());
	}
}