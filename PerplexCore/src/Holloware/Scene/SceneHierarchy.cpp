#include <pch.h>
#include "SceneHierarchy.h"

#include <Holloware/Core/UUID.h>
#include "Entity.h"

#include <nlohmann/json_fwd.hpp>

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

	void to_json(nlohmann::json& j, const SceneHierarchy& property)
	{

	}

	void from_json(const nlohmann::json& j, SceneHierarchy& property)
	{

	}
}
