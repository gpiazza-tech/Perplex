#pragma once

namespace Perplex
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

		friend Timestep operator* (Timestep lhs, float rhs) { return Timestep{ lhs.m_Time * rhs }; }
		friend Timestep operator*= (Timestep& lhs, float rhs) { return lhs.m_Time *= rhs; }
	private:
		float m_Time;
	};
}