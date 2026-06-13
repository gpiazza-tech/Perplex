#pragma once

namespace Perplex
{
	class TypeID
	{
	public:
		explicit TypeID(size_t id) : m_ID{ id } {}

		TypeID operator ++() { return TypeID{ ++m_ID }; }
		friend bool operator==(const TypeID& lhs, const TypeID& rhs) { return lhs.m_ID == rhs.m_ID; }
	private:
		size_t m_ID{};
	};

	inline TypeID nextID{ 0 };

	template<typename T>
	TypeID GetTypeID()
	{
		static const TypeID uniqueID = ++nextID;
		return uniqueID;
	}
}