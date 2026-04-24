#pragma once

#include <Holloware/Core/Core.h>
#include <backends/TextureBuffer.h>

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

		Ref<pxr::TextureBuffer> m_DirectoryIcon;
		Ref<pxr::TextureBuffer> m_FileIcon;
		Ref<pxr::TextureBuffer> m_BackArrowIcon;
	};
}