#pragma once
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvAppItems.h"
#include "mvItemRegistry.h"

namespace Marvel {

	template <typename T>
	void mv_configure_item(const char* item, T config)
	{
		std::shared_ptr<mvAppItem> aitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		aitem->updateConfig(config);
		aitem->updateCoreConfig();
	}

	template <typename T>
	void mv_get_item_configuration(const char* item, T config)
	{
		std::shared_ptr<mvAppItem> aitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		config = (T)aitem->getConfig();
	}

	void start_dearpygui()
	{
		mvApp::GetApp()->start("");

		mvApp::DeleteApp();
		mvEventBus::Reset();
		mvAppLog::Clear();
	}

	void end()
	{
		std::lock_guard<std::mutex> lk(Marvel::mvApp::GetApp()->getMutex());
		Marvel::mvApp::GetApp()->getItemRegistry().popParent();
	}

}