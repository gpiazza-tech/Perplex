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
	float m_frameMS;
	float m_FPS;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
};