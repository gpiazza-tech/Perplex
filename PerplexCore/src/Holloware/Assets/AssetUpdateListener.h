#pragma once

#include "Holloware/Core/Log.h"
#include "Holloware/Assets/AssetManager.h"

#include <efsw/efsw.hpp>

#include <string>

namespace Holloware
{
	class AssetUpdateListener : public efsw::FileWatchListener
	{
	public:
		void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename) override 
		{
			std::string filepath = dir + filename;

			switch (action) {
			case efsw::Actions::Add:
				HW_CORE_INFO("DIR ({0}) FILE ({1}) has event Added", dir, filename);
				break;
			case efsw::Actions::Delete:
				HW_CORE_INFO("DIR ({0}) FILE ({1}) has event Deleted", dir, filename);
				break;
			case efsw::Actions::Modified:
				AssetManager::Import(filepath);
				HW_CORE_INFO("DIR ({0}) FILE ({1}) has event Modified", dir, filename);
				break;
			case efsw::Actions::Moved:
				HW_CORE_INFO("DIR ({0}) FILE ({1}) has event Moved", dir, filename);
				break;
			default:
				HW_CORE_ERROR("AssetUpdateListener: file action not handled!");
			}
		}
	};
}