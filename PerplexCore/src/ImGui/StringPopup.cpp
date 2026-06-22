#include <Perplex/pch.h>
#include <Perplex/ImGui/StringPopup.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Perplex
{
	StringPopup::StringPopup(const char* label) : m_Label{ label }, m_Value{}, m_TriggerOpen{ false } {}

	void StringPopup::Open(const std::string defaultValue, std::function<void(const std::string&)> closeCallback)
	{
		m_Value = defaultValue;
		m_CloseCallback = closeCallback;
		m_TriggerOpen = true;
	}

	void StringPopup::Update()
	{
		if (m_TriggerOpen)
		{
			ImGui::OpenPopup(m_Label);
			m_TriggerOpen = false;
		}

        if (ImGui::BeginPopupModal(m_Label))
        {
			if (ImGui::IsWindowAppearing())
				ImGui::SetKeyboardFocusHere(0);

			if (ImGui::InputText(m_Label, &m_Value, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_CloseCallback(m_Value);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::IsKeyDown(ImGuiKey_Escape))
				ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
	}
}