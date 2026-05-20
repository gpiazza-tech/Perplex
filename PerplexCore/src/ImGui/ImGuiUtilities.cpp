#include <Perplex/pch.h>
#include <Perplex/ImGui/ImGuiUtilities.h>

#include <Perplex/Core/UUID.h>
#include <Perplex/Scene/Components.h>
#include <Perplex/Scene/Entity.h>
#include <Perplex/Assets/Asset.h>

#include <glm/fwd.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include <any>
#include <string>

namespace Perplex
{
	consteval ImVec4 Multiply(const ImVec4 col, float val)
	{
		return ImVec4{ col.x * val, col.y * val, col.z * val, col.w };
	}

	void ImGuiUtilities::SetGlobalStyles()
	{
#define CHILL
#ifdef CHILL
		constexpr ImVec4 DARK = ImVec4
		{
			0.0f / 255.0f,
			0.0f / 255.0f,
			0.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 MID = ImVec4
		{
			20.0f / 255.0f,
			33.0f / 255.0f,
			61.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 LIGHT = ImVec4
		{
			24.0f / 255.0f,
			58.0f / 255.0f,
			55.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 POP = ImVec4
		{
			229.0f / 255.0f,
			229.0f / 255.0f,
			229.0f / 255.0f,
			1.0f
		};
#else
		constexpr ImVec4 DARK = ImVec4
		{
			0.0f / 255.0f,
			0.0f / 255.0f,
			0.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 MID = ImVec4
		{
			247.0f / 255.0f,
			39.0f / 255.0f,
			152.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 LIGHT = ImVec4
		{
			202.0f / 255.0f,
			125.0f / 255.0f,
			31.0f / 255.0f,
			1.0f
		};
		constexpr ImVec4 POP = ImVec4
		{
			235.0f / 255.0f,
			244.0f / 255.0f,
			133.0f / 255.0f,
			1.0f
		};
#endif
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
		style.Colors[ImGuiCol_Text] = POP;
		style.Colors[ImGuiCol_TextDisabled] = POP;
		style.Colors[ImGuiCol_TextSelectedBg] = POP;
		style.Colors[ImGuiCol_WindowBg] = DARK;
		style.Colors[ImGuiCol_PopupBg] = DARK;
		style.Colors[ImGuiCol_Border] = MID;
		style.Colors[ImGuiCol_BorderShadow] = MID;
		style.Colors[ImGuiCol_FrameBg] = MID;
		style.Colors[ImGuiCol_FrameBgHovered] = MID;
		style.Colors[ImGuiCol_FrameBgActive] = MID;
		style.Colors[ImGuiCol_TitleBg] = DARK;
		style.Colors[ImGuiCol_TitleBgCollapsed] = MID;
		style.Colors[ImGuiCol_TitleBgActive] = DARK;
		style.Colors[ImGuiCol_MenuBarBg] = MID;
		style.Colors[ImGuiCol_ScrollbarBg] = DARK;
		style.Colors[ImGuiCol_ScrollbarGrab] = DARK;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = DARK;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = DARK;
		style.Colors[ImGuiCol_CheckMark] = MID;
		style.Colors[ImGuiCol_SliderGrab] = MID;
		style.Colors[ImGuiCol_SliderGrabActive] = MID;
		style.Colors[ImGuiCol_Button] = LIGHT;
		style.Colors[ImGuiCol_ButtonHovered] = Multiply(LIGHT, 0.9f);
		style.Colors[ImGuiCol_ButtonActive] = Multiply(LIGHT, 0.8f);
		style.Colors[ImGuiCol_Header] = MID;
		style.Colors[ImGuiCol_HeaderHovered] = MID;
		style.Colors[ImGuiCol_HeaderActive] = MID;
		style.Colors[ImGuiCol_ResizeGrip] = DARK;
		style.Colors[ImGuiCol_ResizeGripHovered] = DARK;
		style.Colors[ImGuiCol_ResizeGripActive] = DARK;
		style.Colors[ImGuiCol_PlotLines] = MID;
		style.Colors[ImGuiCol_PlotLinesHovered] = MID;
		style.Colors[ImGuiCol_PlotHistogram] = MID;
		style.Colors[ImGuiCol_PlotHistogramHovered] = MID;
		style.Colors[ImGuiCol_Tab] = MID;
		style.Colors[ImGuiCol_TabDimmed] = MID;
		style.Colors[ImGuiCol_TabDimmedSelected] = MID;
		style.Colors[ImGuiCol_TabDimmedSelectedOverline] = MID;
		style.Colors[ImGuiCol_TabActive] = MID;
		style.Colors[ImGuiCol_TabHovered] = MID;
	}
}