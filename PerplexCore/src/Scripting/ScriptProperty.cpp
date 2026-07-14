#include <Perplex/pch.h>
#include <Perplex/Scripting/ScriptProperty.h>

#include <Perplex/Core/PerplexTypes.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Components/Components.h>
#include <Perplex/ImGui/PerplexDrawers.h>
#include <Perplex/Components/ComponentSerializers.h>
#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Assets/AssetType.h>
#include <c/perplex_math.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>
#include <glm/fwd.hpp>

#include <any>
#include <string>

namespace Perplex
{
	ScriptProperty::ScriptProperty(const std::string& name, const std::string& type, const std::string& strValue)
	{
		m_Name = name;
		m_Type = PerplexTypesConversions::CToHwType(type);
		m_TypeStr = type;

		if (strValue != "")
		{
			switch (m_Type)
			{
			case PerplexTypes::Int:
				m_Value = StringToValue<int>(strValue);
				break;
			case PerplexTypes::Float:
				m_Value = StringToValue<float>(strValue);
				break;
			case PerplexTypes::Double:
				m_Value = StringToValue<double>(strValue);
				break;
			case PerplexTypes::Bool:
				m_Value = StringToValue<bool>(strValue);
				break;
			}
		}

		else // default values
		{
			switch (m_Type)
			{
			case PerplexTypes::Int:
				m_Value = 0;
				break;
			case PerplexTypes::Float:
				m_Value = 0.0f;
				break;
			case PerplexTypes::Double:
				m_Value = 0.0;
				break;
			case PerplexTypes::Bool:
				m_Value = false;
				break;
			case PerplexTypes::Entity:
				m_Value = EntityData();
				break;
			case PerplexTypes::Asset:
				m_Value = Asset();
				break;
			case PerplexTypes::Bounds:
				m_Value = Bounds{};
				break;
			case PerplexTypes::Radius:
				m_Value = Radius{};
				break;
			}
		}
	}

	void ScriptProperty::TrySync(const ScriptProperty& property)
	{
		if (property.m_Name == m_Name && property.m_Type == m_Type)
		{
			m_Value = property.m_Value;
		}
	}

	void ScriptProperty::DrawGui()
	{
		switch (m_Type)
		{
		case PerplexTypes::Int:
			DrawAny<int>(m_Value, m_Name.c_str());
			break;
		case PerplexTypes::Float:
			DrawAny<float>(m_Value, m_Name.c_str());
			break;
		case PerplexTypes::Double:
			DrawAny<double>(m_Value, m_Name.c_str());
 			break;
		case PerplexTypes::Bool:
			DrawAny<bool>(m_Value, m_Name.c_str());
			break;
		case PerplexTypes::Vec2:
			DrawAny<glm::vec2>(m_Value, m_Name.c_str());
			break;
		case PerplexTypes::Vec3:
			DrawAny<glm::vec3>(m_Value, m_Name.c_str());
			break;
		case PerplexTypes::Entity:
			DrawEntityField(m_Name.c_str(), std::any_cast<EntityData&>(m_Value));
			break;
		case PerplexTypes::Asset:
			DrawAssetField(m_Name.c_str(), std::any_cast<Asset&>(m_Value), StringToAssetType(m_TypeStr));
			break;
		case PerplexTypes::Bounds:
			DrawAny<Bounds>(m_Value, m_Name.c_str());
			break;
		case PerplexTypes::Radius:
			DrawAny<Radius>(m_Value, m_Name.c_str());
			break;
		default:
			break;
		}
	}

	void to_json(nlohmann::json& j, const ScriptProperty& property) 
	{
		j["name"] = property.m_Name;
		j["type"] = property.m_Type;
		j["typeStr"] = property.m_TypeStr;

		switch (property.m_Type)
		{
		case PerplexTypes::Int:
			j["value"] = std::any_cast<int>(property.m_Value);
			break;
		case PerplexTypes::Float:
			j["value"] = std::any_cast<float>(property.m_Value);
			break;
		case PerplexTypes::Double:
			j["value"] = std::any_cast<double>(property.m_Value);
			break;
		case PerplexTypes::Bool:
			j["value"] = std::any_cast<bool>(property.m_Value);
			break;
		case PerplexTypes::Vec2:
			j["value"] = std::any_cast<glm::vec2>(property.m_Value);
			break;
		case PerplexTypes::Vec3:
			j["value"] = std::any_cast<glm::vec3>(property.m_Value);
			break;
		case PerplexTypes::Entity:
			j["value"] = std::any_cast<EntityData>(property.m_Value);
			break;
		case PerplexTypes::Asset:
			j["value"] = std::any_cast<Asset>(property.m_Value);
			break;
		case PerplexTypes::Bounds:
			j["value"] = std::any_cast<Bounds>(property.m_Value);
			break;
		case PerplexTypes::Radius:
			j["value"] = std::any_cast<Radius>(property.m_Value);
			break;
		default:
			break;
		}
	}

	void from_json(const nlohmann::json& j, ScriptProperty& property)
	{
		property.m_Name = j["name"].get<std::string>();
		property.m_Type = j["type"].get<PerplexTypes>();
		property.m_TypeStr = j["typeStr"].get<std::string>();

		switch (property.m_Type)
		{
		case PerplexTypes::Int:
			property.m_Value = j["value"].get<int>();
			break;
		case PerplexTypes::Float:
			property.m_Value = j["value"].get<float>();
			break;
		case PerplexTypes::Double:
			property.m_Value = j["value"].get<double>();
			break;
		case PerplexTypes::Bool:
			property.m_Value = j["value"].get<bool>();
			break;
		case PerplexTypes::Vec2:
			property.m_Value = j["value"].get<glm::vec2>();
			break;
		case PerplexTypes::Vec3:
			property.m_Value = j["value"].get<glm::vec3>();
			break;
		case PerplexTypes::Entity:
			property.m_Value = j["value"].get<EntityData>();
			break;
		case PerplexTypes::Asset:
			property.m_Value = j["value"].get<Asset>();
			break;
		case PerplexTypes::Bounds:
			property.m_Value = j["value"].get<Bounds>();
			break;
		case PerplexTypes::Radius:
			property.m_Value = j["value"].get<Radius>();
			break;
		default:
			break;
		}
	}
}