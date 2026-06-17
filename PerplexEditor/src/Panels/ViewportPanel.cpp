#include <pch.h>
#include "ViewportPanel.h"

#include <imgui/imgui.h>

namespace Perplex
{
    void ViewportPanel::OnUpdate()
    {
        // Resizing
        if (m_Size != *((glm::vec2*)&m_PanelSize))
        {
            m_Size = { m_PanelSize.x, m_PanelSize.y };
            m_Resizing = true;
        }
        else
        {
            m_Resizing = false;
        }
    }

	void ViewportPanel::OnImGuiRender(uint32_t sceneTextureID)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("ViewPort");
        auto viewportOffset = ImGui::GetCursorPos();

        m_PanelSize = ImGui::GetContentRegionAvail();

        m_Focused = ImGui::IsWindowFocused();
        m_Hovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_Focused || !m_Hovered);

        ImGui::Image(sceneTextureID, { m_Size.x, m_Size.y }, ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                Asset payloadAsset = *(Asset*)payload->Data;
                HW_CORE_ERROR("OnAssetDrop callback not implemented!");
                HW_CORE_INFO(payloadAsset.GetName().string());
            }
            ImGui::EndDragDropTarget();
        }

        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y - viewportOffset.y };
        m_Bounds[0] = { minBound.x, minBound.y };
        m_Bounds[1] = { maxBound.x, maxBound.y };

        ImGui::End();
        ImGui::PopStyleVar();
	}

    glm::vec2 ViewportPanel::GetMousePixelPosition() const
    {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_Bounds[0].x;
        my -= m_Bounds[0].y;
        glm::vec2 viewportSize = m_Bounds[1] - m_Bounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        return glm::vec2{ mouseX, mouseY };
    }

    glm::vec2 ViewportPanel::GetMousePosition() const
    {
        glm::vec2 mousePos = GetMousePixelPosition();
        glm::vec2 viewportSize = m_Bounds[1] - m_Bounds[0];

        // 0.0f to 2.0f
        mousePos = mousePos / viewportSize * 2.0f;

        // -1.0f to 1.0f
        mousePos -= glm::vec2{ 1.0f, 1.0f };

        return mousePos;
    }
}