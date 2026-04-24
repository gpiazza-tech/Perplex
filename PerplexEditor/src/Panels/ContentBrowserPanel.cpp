#include "ContentBrowserPanel.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Assets/Asset.h"
#include "Holloware/Assets/AssetManager.h"
#include <backends/TextureBuffer.h>

#include <imgui/imgui.h>

namespace Holloware
{
	ContentBrowserPanel::ContentBrowserPanel()
		: m_AssetsPath(Application::Get().GetCurrentProject().GetAssetsPath()), m_CurrentDirectory(m_AssetsPath)
	{
		m_DirectoryIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/folder_icon.png");
		m_FileIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/file_icon.png");
		m_BackArrowIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/back_arrow.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content");

		if (m_CurrentDirectory != std::filesystem::path(m_AssetsPath))
		{
			ImTextureRef arrowTextureRef = ImTextureRef(m_BackArrowIcon->GetID());

			if (ImGui::ImageButton("back", arrowTextureRef, { 20, 20 }, {0, 1}, {1, 0}))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		float iconSize = 100.0f;
		float padding = 10.0f;
		float cellSize = iconSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (directoryEntry.path().extension() == ".meta") continue;

			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, m_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			Ref<pxr::TextureBuffer> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImTextureRef iconTextureRef = ImTextureRef(icon->GetID());

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(filenameString.c_str(), iconTextureRef, { iconSize, iconSize }, {0, 1}, {1, 0} );

			if (!directoryEntry.is_directory() && ImGui::BeginDragDropSource())
			{
				Asset payloadAsset = AssetManager::Get(path);
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", &payloadAsset, sizeof(uintptr_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
				else
				{
					std::string command = "start " + path.string();
					std::system(command.c_str());
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::End();
	}
}