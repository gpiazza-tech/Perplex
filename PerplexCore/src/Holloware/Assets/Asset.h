#pragma once

#include "Holloware/Core/Core.h"
#include "Holloware/Core/UUID.h"
#include "Holloware/Assets/AssetManager.h"

#include <filesystem>
#include <cstdint>

namespace Holloware
{
	enum class AssetType : uint8_t;

	class Asset
	{
	public:
		~Asset() { AssetManager::OnAssetReferenceDestroyed(*this); };
		Asset() : m_Handler(false) {};
		Asset(UUID uuid) : m_Handler(uuid) {}
		Asset(std::filesystem::path path);

		const std::filesystem::path& GetPath();

		template<typename T>
		Ref<T> GetData()
		{
			return std::static_pointer_cast<T>(AssetManager::GetData(m_Handler));
		}
		
		operator UUID() const { return m_Handler; };
		operator uint64_t() const { return (uint64_t)m_Handler; };
		operator bool() const { return m_Handler; };

		friend void to_json(nlohmann::json& json, const Asset& asset);
		friend void from_json(const nlohmann::json& json, Asset& asset);
	private:
		UUID m_Handler;
	};
}