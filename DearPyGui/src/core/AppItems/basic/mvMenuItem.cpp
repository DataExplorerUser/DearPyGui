#include "mvMenuItem.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvMenuItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_menu_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "shortcut", "Adds a shortcut", "''"},
			{mvPythonDataType::Bool, "check", "Makes menu with checkmarks.", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a menu item to an existing menu.", "None", "Containers") });
	}

	mvMenuItem::mvMenuItem(const std::string& name)
		: mvBoolPtrBase(name, false) {}

	void mvMenuItem::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);

		// create menuitem and see if its selected
		if (ImGui::MenuItem(m_label.c_str(), m_shortcut.c_str(), m_check ? m_value.get() : nullptr, m_core_config.enabled))
		{

			// set other menuitems's value false on same level
			for (auto sibling : m_parent->m_children)
			{
				// ensure sibling
				if (sibling->getType() == mvAppItemType::mvMenuItem)
					*((mvMenuItem*)sibling.get())->m_value = false;
			}

			*m_value = true;

			mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);

		}

	}

	void mvMenuItem::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvMenuItemConfig*)config;

		m_core_config.label = config->label;
		m_core_config.show = config->show;
		m_core_config.enabled = config->enabled;
		m_core_config.callback = config->callback;
		m_core_config.callback_data = config->callback_data;

		m_config.source = aconfig->source;

		if (config != &m_config)
			m_config = *aconfig;
	}

	mvAppItemConfig* mvMenuItem::getConfig()
	{
		return &m_config;
	}

#ifndef MV_CPP

	void mvMenuItem::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "shortcut")) m_shortcut = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "check")) m_check = ToBool(item);

}

	void mvMenuItem::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "shortcut", ToPyString(m_shortcut));
		PyDict_SetItemString(dict, "check", ToPyBool(m_check));
	}

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* shortcut = "";
		int check = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* label = "";
		int show = true;
		int enabled = true;
		const char* parent = "";
		const char* before = "";

		if (!(mvApp::GetApp()->getParsers())["add_menu_item"].parse(args, kwargs, __FUNCTION__, &name,
			&shortcut, &check, &callback, &callback_data, &label, &show, &enabled, &parent, &before))
			return ToPyBool(false);

		auto item = CreateRef<mvMenuItem>(name);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

#endif // !MV_CPP
}