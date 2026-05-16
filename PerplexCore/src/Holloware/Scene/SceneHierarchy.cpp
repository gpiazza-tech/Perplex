#include <pch.h>
#include "SceneHierarchy.h"

#include "EntityNode.h"
#include <Holloware/Core/UUID.h>
#include <Holloware/Serialization/JsonHelper.h>

#include <vector>

namespace Holloware
{
	void SceneHierarchy::Add(EntityNode node, UUID parentID)
	{
		node.ParentID = parentID;

		EntityNode& parent = m_NodeMap[parentID];

		parent.ChildIDs.emplace(parent.ChildIDs.begin() + node.Index, node.ID);
		m_NodeMap[node.ID] = node;

		// Correct sibling indicies
		for (size_t i = node.Index + 1; i < parent.ChildIDs.size(); i++)
		{
			EntityNode& sibling = m_NodeMap[parent.ChildIDs[i]];
			sibling.Index++;
		}
	}

	EntityNode SceneHierarchy::Remove(UUID entityID)
	{
		EntityNode node = m_NodeMap[entityID];
		EntityNode& parent = m_NodeMap[node.ParentID];

		parent.ChildIDs.erase(parent.ChildIDs.begin() + node.Index);

		// Correct sibling indicies
		for (size_t i = node.Index; i < parent.ChildIDs.size(); i++)
		{
			EntityNode& sibling = m_NodeMap[parent.ChildIDs[i]];
			sibling.Index--;
		}

		m_NodeMap.erase(entityID);

		return node;
	}

	void SceneHierarchy::SetParent(UUID entityID, UUID parentID, int index)
	{
		EntityNode node = Remove(entityID);
		node.Index = index;
		Add(node, parentID);
	}

	void SceneHierarchy::Move(UUID entityID, int index)
	{
		EntityNode& node = m_NodeMap[entityID];
		EntityNode& parent = m_NodeMap[node.ParentID];

		parent.ChildIDs.erase(parent.ChildIDs.begin() + node.Index);
		parent.ChildIDs.emplace(parent.ChildIDs.begin() + index, node.ID);

		node.Index = index;

		// Correct sibling indicies
		for (size_t i = index + 1; i < parent.ChildIDs.size(); i++)
		{
			EntityNode& sibling = m_NodeMap[parent.ChildIDs[i]];
			sibling.Index++;
		}
	}

	void to_json(nlohmann::json& j, const SceneHierarchy& hierarchy)
	{
		// Convert map to vector, then serialize
		std::vector<EntityNode> nodes(0);
		nodes.reserve(hierarchy.m_NodeMap.size());

		for (auto& [id, node] : hierarchy.m_NodeMap)
			nodes.emplace_back(node);

		j = nodes;
	}

	void from_json(const nlohmann::json& j, SceneHierarchy& hierarchy)
	{
		// Deserialize, then convert vector to map
		std::vector<EntityNode> nodes = j.get<std::vector<EntityNode>>();

		for (auto& node : nodes)
			hierarchy.m_NodeMap[node.ID] = node;
	}
}
