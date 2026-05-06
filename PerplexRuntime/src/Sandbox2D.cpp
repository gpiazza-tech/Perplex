#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <iostream>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

void Sandbox2D::OnAttach()
{
	HW_PROFILE_FUNCTION();
}

void Sandbox2D::OnDetach()
{
	HW_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Holloware::Timestep ts)
{
	HW_PROFILE_FUNCTION();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");

    ImGui::Text("Renderer2D Stats");
}

void Sandbox2D::OnEvent(Holloware::Event& e)
{
}
