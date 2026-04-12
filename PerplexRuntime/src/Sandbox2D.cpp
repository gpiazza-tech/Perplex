#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <iostream>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	HW_PROFILE_FUNCTION();

	m_FaceTexture = Holloware::Texture2D::Create("assets/textures/face.png");
	m_CheckerboardTexture = Holloware::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Holloware::Texture2D::Create("assets/game/textures/tilemap_packed.png");

	m_Grass = Holloware::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 6 }, { 16, 16 });

	m_CameraController.SetZoomLevel(5.0f);

    Holloware::FrameBufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
}

void Sandbox2D::OnDetach()
{
	HW_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Holloware::Timestep ts)
{
	HW_PROFILE_FUNCTION();

	m_frameMS = ts.GetMilliseconds();

	m_CameraController.OnUpdate(ts);

	// Rendering
	Holloware::Renderer2D::ResetStats();
	{
		HW_PROFILE_SCOPE("Renderer Prep");

		Holloware::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Holloware::RenderCommand::Clear();
	}

	{
		static float rotation;
		rotation += ts * 3.0f;

		HW_PROFILE_SCOPE("Renderer Draw");
		Holloware::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (int y = -10; y < 10; y++)
		{
			for (int x = -10; x < 10; x++)
			{
				glm::vec4 color = { (x + 10) / 20.0f, m_SquareColor.g, (y + 10) / 20.0f, m_SquareColor.a };
				Holloware::Renderer2D::DrawQuad({ { x / 2.0f, y / 2.0f, }, { 0.47f, 0.47f, }, color });
			}
		}

		m_KeySubTexture = Holloware::SubTexture2D::CreateFromCoords(m_SpriteSheet, m_KeySubTextureCoords, { 16, 16 }, m_KeySubTextureSize);
		Holloware::Renderer2D::DrawTexture({ { 0.0f, 0.0f }, m_KeySubTextureSize }, m_Grass);

		Holloware::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");

    auto stats = Holloware::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::Text("FPS: %.3f", 1000.0f / m_frameMS);

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

    ImGui::DragFloat2("Texture Coordinates", glm::value_ptr(m_KeySubTextureCoords));
    ImGui::DragFloat2("Texture Size", glm::value_ptr(m_KeySubTextureSize));

    ImGui::End();
}

void Sandbox2D::OnEvent(Holloware::Event& e)
{
	m_CameraController.OnEvent(e);
}
