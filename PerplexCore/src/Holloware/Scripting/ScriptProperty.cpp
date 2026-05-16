#include <pch.h>
#include "ScriptProperty.h"

#include <Holloware/Core/HollowareTypes.h>
#include <Holloware/Assets/Asset.h>
#include <Holloware/Scene/Components.h>
#include <Holloware/ImGui/ImGuiUtilities.h>
#include <Holloware/ImGui/Drawer.h>
#include <Holloware/Serialization/JsonHelper.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>
#include <glm/fwd.hpp>

#include <any>
#include <string>

namespace Holloware
{
	ScriptProperty::ScriptProperty(const std::string& name, const std::string& type, const std::string& strValue)
	{
		m_Name = name;
		m_Type = HollowareTypesConversions::CToHwType(type);

		if (strValue != "")
		{
			switch (m_Type)
			{
			case HollowareTypes::Int:
				m_Value = StringToValue<int>(strValue);
				break;
			case HollowareTypes::Float:
				m_Value = StringToValue<float>(strValue);
				break;
			case HollowareTypes::Double:
				m_Value = StringToValue<double>(strValue);
				break;
			case HollowareTypes::Bool:
				m_Value = StringToValue<bool>(strValue);
				break;
			}
		}

		else // default values
		{
			switch (m_Type)
			{
			case HollowareTypes::Int:
				m_Value = 0;
				break;
			case HollowareTypes::Float:
				m_Value = 0.0f;
				break;
			case HollowareTypes::Double:
				m_Value = 0.0;
				break;
			case HollowareTypes::Bool:
				m_Value = false;
				break;
			case HollowareTypes::Entity:
				m_Value = EntityData();
				break;
			case HollowareTypes::Asset:
				m_Value = Asset();
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
		case HollowareTypes::Int:
			ImGuiUtilities::DrawAnyIntControl(m_Name.c_str(), m_Value);
			break;
		case HollowareTypes::Float:
			ImGuiUtilities::DrawAnyFloatControl(m_Name.c_str(), m_Value);
			break;
		case HollowareTypes::Double:
			ImGuiUtilities::DrawAnyDoubleControl(m_Name.c_str(), m_Value);
 			break;
		case HollowareTypes::Bool:
			ImGuiUtilities::DrawAnyBoolControl(m_Name.c_str(), m_Value);
			break;
		case HollowareTypes::Vec3:
			ImGuiUtilities::DrawVec3Control(m_Name, std::any_cast<glm::vec3&>(m_Value));
			break;
		case HollowareTypes::Entity:
			ImGuiUtilities::EntityInput(m_Name.c_str(), std::any_cast<EntityData&>(m_Value));
			break;
		case HollowareTypes::Asset:
			HW_CORE_ERROR("HollowareType::Asset not implemented in ScriptProperty!");
			// Drawer::AssetInput(m_Name.c_str(), std::any_cast<Asset&>(m_Value));
			break;
		default:
			break;
		}
	}

	void to_json(nlohmann::json& j, const ScriptProperty& property) 
	{
		j["name"] = property.m_Name;
		j["type"] = property.m_Type;

		switch (property.m_Type)
		{
		case HollowareTypes::Int:
			j["value"] = std::any_cast<int>(property.m_Value);
			break;
		case HollowareTypes::Float:
			j["value"] = std::any_cast<float>(property.m_Value);
			break;
		case HollowareTypes::Double:
			j["value"] = std::any_cast<double>(property.m_Value);
			break;
		case HollowareTypes::Bool:
			j["value"] = std::any_cast<bool>(property.m_Value);
			break;
		case HollowareTypes::Vec3:
			j["value"] = std::any_cast<glm::vec3>(property.m_Value);
			break;
		case HollowareTypes::Entity:
			j["value"] = std::any_cast<EntityData>(property.m_Value);
			break;
		case HollowareTypes::Asset:
			j["value"] = std::any_cast<Asset>(property.m_Value);
			break;
		default:
			break;
		}
	}

	void from_json(const nlohmann::json& j, ScriptProperty& property)
	{
		property.m_Name = j["name"].get<std::string>();
		property.m_Type = j["type"].get<HollowareTypes>();

		switch (property.m_Type)
		{
		case HollowareTypes::Int:
			property.m_Value = j["value"].get<int>();
			break;
		case HollowareTypes::Float:
			property.m_Value = j["value"].get<float>();
			break;
		case HollowareTypes::Double:
			property.m_Value = j["value"].get<double>();
			break;
		case HollowareTypes::Bool:
			property.m_Value = j["value"].get<bool>();
			break;
		case HollowareTypes::Vec3:
			property.m_Value = j["value"].get<glm::vec3>();
			break;
		case HollowareTypes::Entity:
			property.m_Value = j["value"].get<EntityData>();
			break;
		case HollowareTypes::Asset:
			property.m_Value = j["value"].get<Asset>();
			break;
		default:
			break;
		}
	}
}