#include <Perplex/pch.h>
#include <Perplex/ImGui/ImGuiUtilities.h>

#include <imgui.h>

namespace Perplex
{
	static ImVec4 Multiply(const ImVec4 col, float val)
	{
		return ImVec4{ col.x * val, col.y * val, col.z * val, col.w };
	}

	static void SetColors(const ImVec4& dark, const ImVec4& mid, const ImVec4& light, const ImVec4& pop)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowBorderSize = 0.01f;
		style.FrameRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.WindowRounding = 0.0f;
		style.TabRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.WindowMenuButtonPosition = ImGuiDir_None;

		style.Alpha = 1.0f;
		style.FrameRounding = 3.0f;
		style.Colors[ImGuiCol_Text] = pop;
		style.Colors[ImGuiCol_TextDisabled] = pop;
		style.Colors[ImGuiCol_TextSelectedBg] = pop;
		style.Colors[ImGuiCol_WindowBg] = dark;
		style.Colors[ImGuiCol_PopupBg] = dark;
		style.Colors[ImGuiCol_Border] = light;
		style.Colors[ImGuiCol_BorderShadow] = light;
		style.Colors[ImGuiCol_FrameBg] = light;
		style.Colors[ImGuiCol_FrameBgHovered] = light;
		style.Colors[ImGuiCol_FrameBgActive] = light;
		style.Colors[ImGuiCol_TitleBg] = dark;
		style.Colors[ImGuiCol_TitleBgCollapsed] = light;
		style.Colors[ImGuiCol_TitleBgActive] = dark;
		style.Colors[ImGuiCol_MenuBarBg] = light;
		style.Colors[ImGuiCol_ScrollbarBg] = dark;
		style.Colors[ImGuiCol_ScrollbarGrab] = dark;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = dark;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = dark;
		style.Colors[ImGuiCol_CheckMark] = pop;
		style.Colors[ImGuiCol_SliderGrab] = light;
		style.Colors[ImGuiCol_SliderGrabActive] = light;
		style.Colors[ImGuiCol_Button] = mid;
		style.Colors[ImGuiCol_ButtonHovered] = Multiply(mid, 0.9f);
		style.Colors[ImGuiCol_ButtonActive] = Multiply(mid, 0.8f);
		style.Colors[ImGuiCol_Header] = light;
		style.Colors[ImGuiCol_HeaderHovered] = light;
		style.Colors[ImGuiCol_HeaderActive] = light;
		style.Colors[ImGuiCol_ResizeGrip] = dark;
		style.Colors[ImGuiCol_ResizeGripHovered] = dark;
		style.Colors[ImGuiCol_ResizeGripActive] = dark;
		style.Colors[ImGuiCol_PlotLines] = light;
		style.Colors[ImGuiCol_PlotLinesHovered] = light;
		style.Colors[ImGuiCol_PlotHistogram] = light;
		style.Colors[ImGuiCol_PlotHistogramHovered] = light;
		style.Colors[ImGuiCol_Tab] = light;
		style.Colors[ImGuiCol_TabDimmed] = light;
		style.Colors[ImGuiCol_TabDimmedSelected] = light;
		style.Colors[ImGuiCol_TabDimmedSelectedOverline] = light;
		style.Colors[ImGuiCol_TabActive] = light;
		style.Colors[ImGuiCol_TabHovered] = light;
	}

	void ImGuiUtilities::SetGlobalStyles()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowBorderSize = 0.1f;
		style.FrameRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.WindowRounding = 0.0f;
		style.TabRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.WindowMenuButtonPosition = ImGuiDir_None;

		SetEditorStyles();
	}

	void ImGuiUtilities::SetEditorStyles()
	{
		constexpr ImVec4 DARK = ImVec4
		{
			7.0f / 255.0f,
			10.0f / 255.0f,
			20.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 MID = ImVec4
		{
			24.0f / 255.0f,
			58.0f / 255.0f,
			55.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 LIGHT = ImVec4
		{
			40.0f / 255.0f,
			66.0f / 255.0f,
			122.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 POP = ImVec4
		{
			229.0f / 255.0f,
			229.0f / 255.0f,
			229.0f / 255.0f,
			1.0f
		};

		SetColors(DARK, MID, LIGHT, POP);
	}

	void ImGuiUtilities::SetRuntimeStyles()
	{
		constexpr ImVec4 DARK = ImVec4
		{
			0.0f / 255.0f,
			0.0f / 255.0f,
			0.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 MID = ImVec4
		{
			24.0f / 255.0f,
			58.0f / 255.0f,
			55.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 LIGHT = ImVec4
		{
			20.0f / 255.0f,
			33.0f / 255.0f,
			66.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 POP = ImVec4
		{
			229.0f / 255.0f,
			229.0f / 255.0f,
			229.0f / 255.0f,
			1.0f
		};

		SetColors(DARK, MID, LIGHT, POP);
	}
}