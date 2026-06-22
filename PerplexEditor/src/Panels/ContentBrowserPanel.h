#pragma once

#include <Perplex/Perplex.h>
#include <Perplex/ImGui/StringPopup.h>
#include <pxr/pxr.h>

#include <filesystem>

namespace Perplex
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

		Ref<pxr::TextureBuffer> m_DirectoryIcon;
		Ref<pxr::TextureBuffer> m_FileIcon;
		Ref<pxr::TextureBuffer> m_BackArrowIcon;

		StringPopup m_RenameFilePopup;
	};
}