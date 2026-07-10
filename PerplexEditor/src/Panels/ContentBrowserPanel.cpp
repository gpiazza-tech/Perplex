#include <pch.h>
#include "ContentBrowserPanel.h"

#include <Perplex/Perplex.h>
#include <Perplex/Platform/SystemUtils.h>
#include <pxr/pxr.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <filesystem>

namespace Perplex
{
	namespace fs = std::filesystem;

	ContentBrowserPanel::ContentBrowserPanel()
		: m_AssetsPath(Application::Get().GetCurrentProject().GetAssetsPath()), m_CurrentDirectory(m_AssetsPath), m_RenameFilePopup{ "Rename File" }
	{
		m_DirectoryIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/folder_icon.png");
		m_FileIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/file_icon.png");
		m_BackArrowIcon = CreateRef<pxr::TextureBuffer>(m_AssetsPath / "textures/back_arrow.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content");

		// Drop item on window
		if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->InnerRect, ImGui::GetCurrentWindow()->ID))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
			{
				Entity payloadEntity = *(Entity*)payload->Data;
				Ref<Scene> prefabScene = CreateRef<Scene>();
				prefabScene->CopyEntity(payloadEntity);
				fs::path prefabPath = m_CurrentDirectory / fs::path(payloadEntity.GetTag() + ".pxp");
				SceneSerializer::Serialize(prefabScene, prefabPath);
			}
			ImGui::EndDragDropTarget();
		}

		// Popup Menu
		if (ImGui::BeginPopupContextWindow("Global Popup Menu", ImGuiPopupPositionPolicy_ComboBox | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Open in File Explorer"))
				OpenFilesystemGui(NativePath(m_CurrentDirectory));

			if (ImGui::BeginMenu("Create"))
			{
				const Project& project = Application::Get().GetCurrentProject();
				fs::path templateFiles = project.EngineRes("template_files");

				for (auto& directoryEntry : fs::directory_iterator{ templateFiles })
				{
					if (directoryEntry.is_directory())
						continue;

					fs::path path = directoryEntry.path();
					std::string fileName = path.stem().string();

					if (ImGui::MenuItem(fileName.c_str()))
						fs::copy_file(path, m_CurrentDirectory / path.filename());
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		// Back arrow
		if (m_CurrentDirectory != fs::path(m_AssetsPath))
		{
			ImTextureRef arrowTextureRef = ImTextureRef(m_BackArrowIcon->GetID());

			if (ImGui::ImageButton("back", arrowTextureRef, { 20, 20 }, {0, 1}, {1, 0}))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		// File browser
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
			const fs::path& path = directoryEntry.path();
			ImGui::PushID(path.string().c_str());

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

			// Directory Popup Menu
			if (ImGui::BeginPopupContextItem("Directory Popup Menu", ImGuiPopupPositionPolicy_ComboBox))
			{
				if (ImGui::MenuItem("Rename"))
					m_RenameFilePopup.Open(path.stem().string(), [path](const std::string& newStr) 
						{ 
							fs::path newPath{ path.parent_path() / std::string{ newStr + path.extension().string() } };
							std::filesystem::rename(path, newPath);
						});

				ImGui::EndPopup();
			}

			// Open when double clicked
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
				else
				{
					// WINDOWS ONLY

					fs::path startCommandPath = NativePath(path);
					std::string command = "start " + startCommandPath.string() + "";
					std::system(command.c_str());
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		m_RenameFilePopup.Update();

		ImGui::Columns(1);

		ImGui::End();
	}
}