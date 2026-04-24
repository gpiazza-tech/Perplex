#pragma once

#include <Holloware/Core/UUID.h>

#include <vector>

namespace Holloware
{
	struct EntityNode
	{
		EntityNode() = default;
		EntityNode(const EntityNode&) = default;
		EntityNode(UUID id, int i = 0) : ID(id), ParentID(0), ChildIDs(0), Index(i) {}

		UUID ID = 0;
		int Index = 0;
		UUID ParentID = 0;
		std::vector<UUID> ChildIDs = std::vector<UUID>(0);
	};
}