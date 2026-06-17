#pragma once

#include <Perplex/Perplex.h>

#include <imgui/imgui.h>

namespace Perplex
{
	class ViewportPanel
	{
	public:
		void OnUpdate();
		void OnImGuiRender(uint32_t sceneTextureID);

		glm::vec2 GetMousePixelPosition() const;
		glm::vec2 GetMousePosition() const;

		bool IsFocused() const{ return m_Focused; }
		const glm::vec2& GetSize() const { return m_Size; }
		bool IsResizing() const { return m_Resizing; }
	private:
		bool m_Focused = false;
		bool m_Hovered = false;
		bool m_Resizing = false;
		ImVec2 m_PanelSize = ImVec2(1.0f, 1.0f);
		glm::vec2 m_Size = glm::vec2(1.0f, 1.0f);
		glm::vec2 m_Bounds[2]{};
	};
}