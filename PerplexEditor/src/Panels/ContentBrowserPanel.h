#pragma once

#include <Holloware/Core/Core.h>

#include <filesystem>

namespace Holloware
{
	class Texture2D;

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_AssetsPath;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_BackArrowIcon;
	};
}