#include <pch.h>
#include "ContentBrowserPanel.h"

#include <Perplex/Perplex.h>
#include <Perplex/Platform/SystemUtils.h>
#include <pxr/pxr.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <filesystem>
#include <cstdlib>

namespace Perplex
{
	namespace fs = std::filesystem;

	ContentBrowserPanel::ContentBrowserPanel()
		: m_RenameFilePopup{ "Rename File" }, m_DeleteFilePopup{ "Delete File?" }
	{
			m_DirectoryIcon = CreateRef<pxr::TextureBuffer>(Application::Get().EngineRes("textures/folder_icon.png"));
			m_FileIcon = CreateRef<pxr::TextureBuffer>(Application::Get().EngineRes("textures/file_icon.png"));
			m_BackArrowIcon = CreateRef<pxr::TextureBuffer>(Application::Get().EngineRes("textures/back_arrow.png"));
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content");

		const Game& game = Application::Get().GetGame();

		if (game.RootDirectory != "" && game.AssetsDirectory != "")
		{
			RenderContent(game.AssetsDirectory);
		}

		else
		{
			const char* text = "no content to display";

			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 textSize = ImGui::CalcTextSize(text);
			ImGui::SetCursorPos(ImVec2{ windowSize.x * 0.5f - textSize.x * 0.5f, windowSize.y * 0.5f - textSize.y * 0.5f });
			ImGui::Text(text);
		}

		ImGui::End();
	}

	void ContentBrowserPanel::RenderContent(const std::filesystem::path& assetDir)
	{
		static fs::path currentDir = assetDir;

		// Drop item on window
		if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->InnerRect, ImGui::GetCurrentWindow()->ID))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
			{
				Entity payloadEntity = *(Entity*)payload->Data;
				Ref<Scene> prefabScene = CreateRef<Scene>();
				prefabScene->CopyEntity(payloadEntity);
				fs::path prefabPath = currentDir / fs::path(payloadEntity.GetTag() + ".pxp");
				SceneSerializer::Serialize(prefabScene, prefabPath);
			}
			ImGui::EndDragDropTarget();
		}

		// Popup Menu
		if (ImGui::BeginPopupContextWindow("Global Popup Menu", ImGuiPopupPositionPolicy_ComboBox | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Open in File Explorer"))
				OpenFilesystemGui(NativePath(currentDir));

			if (ImGui::BeginMenu("Create"))
			{
				fs::path templateFiles = Application::Get().EngineRes("template_files");

				for (auto& directoryEntry : fs::directory_iterator{ templateFiles })
				{
					if (directoryEntry.is_directory())
						continue;

					fs::path templatePath = directoryEntry.path();
					fs::path currentDirectory = currentDir;
					std::string fileName = templatePath.stem().string();

					if (ImGui::MenuItem(fileName.c_str()))
						m_RenameFilePopup.Open(fileName, [templatePath, currentDirectory](const std::string& name)
							{
								std::string content = FileIO::LoadTextFile(templatePath);
								ReplaceAll(content, "{{FILENAME}}", name);
								FileIO::SaveTextFile(currentDirectory / (name + templatePath.extension().string()), content);
							});
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		// Back arrow
		if (currentDir != fs::path(assetDir))
		{
			ImTextureRef arrowTextureRef = ImTextureRef(m_BackArrowIcon->GetID());

			if (ImGui::ImageButton("back", arrowTextureRef, { 20, 20 }, { 0, 1 }, { 1, 0 }))
			{
				currentDir = currentDir.parent_path();
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

		for (auto& directoryEntry : std::filesystem::directory_iterator(currentDir))
		{
			if (directoryEntry.path().extension() == ".meta") continue;
			const fs::path& path = directoryEntry.path();
			ImGui::PushID(path.string().c_str());

			auto relativePath = std::filesystem::relative(path, assetDir);
			std::string filenameString = relativePath.filename().string();

			Ref<pxr::TextureBuffer> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImTextureRef iconTextureRef = ImTextureRef(icon->GetID());

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(filenameString.c_str(), iconTextureRef, { iconSize, iconSize }, { 0, 1 }, { 1, 0 });

			if (!directoryEntry.is_directory() && ImGui::BeginDragDropSource())
			{
				Asset payloadAsset{ path };
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

				else if (!fs::is_directory(path) && ImGui::MenuItem("Delete"))
					m_DeleteFilePopup.Open([path](bool deleteFile)
						{
							if (deleteFile)
								std::filesystem::remove(path);
						});

				ImGui::EndPopup();
			}

			// Open when double clicked
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					currentDir /= path.filename();
				else
				{
					m_OpenFileCallback(path);
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		m_RenameFilePopup.Update();
		m_DeleteFilePopup.Update();

		ImGui::Columns(1);
	}
}