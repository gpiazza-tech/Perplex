#pragma once

#include <Holloware/Core/HollowareTypes.h>
#include <Holloware/Core/HollowareObject.h>

#include <glm/glm.hpp>

namespace Holloware
{
	template<typename T>
	class SerializableValue : HollowareObject
	{
	public:
		SerializableValue(const T& data, const std::string& name, HollowareTypes type)
			: m_Data(data), m_Name(name), m_Type(type)
		{
		}
		~SerializableValue() {}
		
		inline const T& GetData()
		{
			return m_Data;
		}

		inline const HollowareTypes GetType()
		{
			return HollowareTypes::None;
		}
	private:
		T m_Data;
		const std::string m_Name;
		HollowareTypes type;
	};

	template<>
	class SerializableValue<int> : HollowareObject
	{
	public:
		SerializableValue(const int data, const std::string& name)
			: m_Data(data), m_Name(name)
		{
		}
		~SerializableValue() {}

		inline const int& GetData()
		{
			return m_Data;
		}

		inline const HollowareTypes GetType()
		{
			Holloware::HollowareTypes::Int;
		}
	private:
		int m_Data;
		const std::string m_Name;
	};

	template<>
	class SerializableValue<float> : HollowareObject
	{
	public:
		SerializableValue(const float data, const std::string& name)
			: m_Data(data), m_Name(name)
		{
		}
		~SerializableValue() {}

		inline const float& GetData()
		{
			return m_Data;
		}

		inline const HollowareTypes GetType()
		{
			Holloware::HollowareTypes::Float;
		}
	private:
		float m_Data;
		const std::string m_Name;
	};

	template<>
	class SerializableValue<bool> : HollowareObject
	{
	public:
		SerializableValue(const bool data, const std::string& name)
			: m_Data(data), m_Name(name)
		{
		}
		~SerializableValue() {}

		inline const bool& GetData()
		{
			return m_Data;
		}

		inline const HollowareTypes GetType()
		{
			Holloware::HollowareTypes::Float;
		}
	private:
		bool m_Data;
		const std::string m_Name;
	};

	template<>
	class SerializableValue<glm::vec3> : HollowareObject
	{
	public:
		SerializableValue(const glm::vec3 data, const std::string& name)
			: m_Data(data), m_Name(name)
		{
		}
		~SerializableValue() {}

		inline const glm::vec3& GetData()
		{
			return m_Data;
		}

		inline const HollowareTypes GetType()
		{
			Holloware::HollowareTypes::Vec3;
		}
	private:
		glm::vec3 m_Data;
		const std::string m_Name;
	};
}