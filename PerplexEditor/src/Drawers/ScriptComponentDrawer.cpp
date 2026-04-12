#include <Holloware/Scene/Components.h>
#include <Holloware/Core/Log.h>
#include <Holloware/ImGui/ImGuiUtilities.h>
#include <Holloware/ImGui/Drawer.h>

#include <imgui/imgui.h>

namespace Holloware
{
	void ScriptComponent::DrawGui()
	{
		ImGui::PushID(this);

		// Head
		std::string fileName = ScriptAsset.GetPath().filename().string();
		ImGui::Text(fileName.c_str());

		ImGui::SameLine();
		std::string assetPathString = ScriptAsset.GetPath() != "" ? ScriptAsset.GetPath().string() : "None";
		ImGui::Button(assetPathString.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				Asset asset = *(Asset*)payload->Data;
				std::string pathString = asset.GetPath().string();

				if (std::filesystem::path(pathString).extension() == ".c")
				{
					ScriptAsset = asset;
					std::string scriptName = ScriptAsset.GetPath().stem().string();
				}
				else { HW_CORE_ERROR("{0} is not a c script file!", pathString); }
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		ImGui::PopID();

		for (auto& property : Properties)
		{
			property.DrawGui();
		}
	}
}