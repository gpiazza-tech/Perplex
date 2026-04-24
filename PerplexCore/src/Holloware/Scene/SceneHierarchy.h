#pragma once

#include <Holloware/Core/UUID.h>

#include <nlohmann/json_fwd.hpp>

#include <unordered_map>
#include <vector>

namespace Holloware
{
	struct EntityNode
	{
		EntityNode() = default; 
		EntityNode(const EntityNode&) = default;
		EntityNode(UUID id, int i = 0) : ID(id), ParentID(0), ChildIDs(0), Index(i) { }

		UUID ID = 0;
		int Index = 0;
		UUID ParentID = 0;
		std::vector<UUID> ChildIDs = std::vector<UUID>(0);
	};

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

		friend void to_json(nlohmann::json& j, const SceneHierarchy& property);
		friend void from_json(const nlohmann::json& j, SceneHierarchy& property);
	};
}