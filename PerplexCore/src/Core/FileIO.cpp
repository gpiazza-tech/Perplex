#include <Perplex/pch.h>
#include <Perplex/Core/FileIO.h>

#include <Perplex/Core/Application.h>

#include <filesystem>
#include <optional>
#include <string_view>
#include <system_error>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

namespace Perplex
{
	std::optional<fs::path> FileIO::FindFileWithExtension(std::string_view extension, const std::filesystem::path& directory)
	{
		std::optional<fs::path> result = std::nullopt;

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (entry.path().extension() == extension)
				result = entry.path();
		}

		return result;
	}

	bool FileIO::TryCreateDirectories(const std::filesystem::path& directory)
	{
		std::error_code errorCode{};

		if (!fs::exists(directory))
			fs::create_directories(directory, errorCode);

		return !errorCode.value();
	}

	std::filesystem::path FileIO::GameRootDirectory()
	{
		return Application::Get().GetGame().RootDirectory;
	}

	std::string FileIO::LoadTextFile(const std::filesystem::path& directory)
	{
		std::ifstream ifs(directory.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		return oss.str();
	}

	void FileIO::SaveTextFile(const std::filesystem::path& directory, const std::string& text)
	{
		std::ofstream ofs(directory.string());
		ofs << text;
	}
}