#pragma once

#include <Holloware.h>

class Sandbox2D : public Holloware::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Holloware::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Holloware::Event& e) override;
private:
	Holloware::OrthographicCameraController m_CameraController;

	Holloware::Ref<Holloware::Texture2D> m_FaceTexture;
	Holloware::Ref<Holloware::Texture2D> m_CheckerboardTexture;
	Holloware::Ref<Holloware::Texture2D> m_SpriteSheet;

	Holloware::Ref<Holloware::SubTexture2D> m_Grass;

	Holloware::Ref<Holloware::SubTexture2D> m_KeySubTexture;
	glm::vec2 m_KeySubTextureCoords = { 6, 6 };
	glm::vec2 m_KeySubTextureSize = { 1, 1 };

	Holloware::Ref<Holloware::GameObject> m_Square;
	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };

	float m_frameMS;
	float m_FPS;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
};