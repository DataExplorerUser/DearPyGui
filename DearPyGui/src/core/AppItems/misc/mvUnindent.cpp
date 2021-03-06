#include "mvUnindent.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {


	void mvUnindent::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "unindent", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "'unindent'"},
			{mvPythonDataType::Float, "offset", "", "0.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Unindents following items.", "None", "Adding Widgets") });
	}


	mvUnindent::mvUnindent(const std::string& name, float default_value)
		: mvFloatPtrBase(name, default_value)
	{
		m_description.duplicatesAllowed = true;
	}

	void mvUnindent::draw()
	{
		ImGui::Unindent(*m_value);
	}


#ifdef MV_CPP
#else
	PyObject* unindent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("unindent" + std::to_string(i));
		const char* name = sname.c_str();
		float offset = 0.0f;
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["unindent"].parse(args, kwargs, __FUNCTION__,
			&name, &offset, &parent, &before, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvUnindent>(name, offset);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}
#endif
}