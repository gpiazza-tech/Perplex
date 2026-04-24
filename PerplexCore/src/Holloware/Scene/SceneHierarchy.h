#pragma once

#include "EntityNode.h"
#include <Holloware/Core/UUID.h>

#include <nlohmann/json_fwd.hpp>

#include <unordered_map>

namespace Holloware
{
	class Entity;

	class SceneHierarchy
	{
	public:
		void Add(EntityNode node, UUID parentID);
		EntityNode Remove(UUID entityID);

		void SetParent(UUID entityID, UUID parentID, int index = 0);
		void Move(UUID entityID, int index);

		const EntityNode& GetRoot() { return m_NodeMap[0]; }
		const EntityNode& GetNode(UUID uuid) { return m_NodeMap[uuid]; }
	private:
		std::unordered_map<UUID, EntityNode> m_NodeMap;

		friend void to_json(nlohmann::json& j, const SceneHierarchy& hierarchy);
		friend void from_json(const nlohmann::json& j, SceneHierarchy& hierarchy);
	};
}