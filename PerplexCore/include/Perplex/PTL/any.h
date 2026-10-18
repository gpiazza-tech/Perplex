#pragma once

#include <nlohmann/json_fwd.hpp>

#include <array>
#include <cstddef>
#include <new>

namespace Perplex::ptl
{
	static constexpr size_t PX_ANY_SOO_THRESHOLD = 8;

	class any
	{
	public:
		any(const void* data, size_t size) : m_SOO(size <= PX_ANY_SOO_THRESHOLD), m_Size(size)
		{
			if (m_SOO)
			{
				std::memcpy(m_StackData.data(), data, size);
			}
			else
			{
				m_HeapData = operator new(size);
				std::memcpy(m_HeapData, data, size);
			}
		}

		any(const any& other) : m_SOO(other.m_SOO), m_Size(other.m_Size)
		{
			if (m_SOO)
				std::memcpy(m_StackData.data(), other.m_StackData.data(), m_Size);
			else
				std::memcpy(m_HeapData, other.m_HeapData, m_Size);
		}

		any(any&& other) noexcept : m_SOO(other.m_SOO), m_Size(other.m_Size)
		{
			if (m_SOO)
				m_StackData = other.m_StackData;
			if (!m_SOO)
				m_HeapData = other.m_HeapData;

			other.reset();
		}

		~any()
		{
			free();
		}

		any& operator=(const any& other)
		{
			set(other.data(), other.size());
			return *this;
		}

		any& operator=(any&& other) noexcept
		{
			free();

			m_SOO = other.m_SOO;
			m_Size = other.m_Size;
			if (m_SOO)
				m_StackData = other.m_StackData;
			else
				m_HeapData = other.m_HeapData;
			other.reset();

			return *this;
		}

		void* data() { return m_SOO ? m_StackData.data() : m_HeapData; }
		const void* data() const { return m_SOO ? m_StackData.data() : m_HeapData; }

		size_t size() const { return m_Size; }

		void set(const void* data, size_t size)
		{
			free();

			m_SOO = size <= PX_ANY_SOO_THRESHOLD;
			m_Size = size;

			if (m_SOO)
			{
				std::memcpy(m_StackData.data(), data, size);
			}
			else
			{
				m_HeapData = operator new(size);
				std::memcpy(m_HeapData, data, size);
			}
		}

		void free()
		{
			if (!m_SOO)
				operator delete(m_HeapData);
		}

		void reset()
		{
			m_SOO = true;
			m_Size = 0;
			m_StackData = {};
		}

		friend void to_json(nlohmann::json& j, const any& val);
		friend void from_json(const nlohmann::json& j, any& val);
	private:
		bool m_SOO{ true };
		size_t m_Size{ 0 };
		union
		{
			std::array<std::byte, PX_ANY_SOO_THRESHOLD> m_StackData{};
			void* m_HeapData;
		};
	};
}