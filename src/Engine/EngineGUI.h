#pragma once
#include "SceneManager.h"

class EngineGUI
{
private:
	EngineGUI();
	~EngineGUI();

	SceneManager* m_sceneManagerRef = nullptr;

	bool m_showEditTab = false;

	// Script Tab values
	bool m_showScriptsTab = false;
	char m_createScriptPath[36] = {};

	// Object Tab values
	bool m_showObjectsTab = false;

	// Properties Tab Values
	recs::Entity m_currentEntity = (recs::Entity) - 1;
	bool m_showPropertiesTab = true;
	char m_createScriptPathProp[36] = {};

	char m_modelinputField[100] = {};
	char m_textureInputField[100] = {};

	// Bottom Bar Values
	bool m_showBottomEntities = false;
	bool m_showBottomConsole = false;

	// Console Window Values
	std::vector<ConsoleLogEvent> m_consoleLogs;
	char m_consoleWindowInput[100] = {};
	void ClearConsoleWindowText();
	// Console window log filter.
	UINT m_filter = (UINT)ConsoleLogEventType::ALL;

	void BottomBarPutToFalse();
public:
	static auto& Get()
	{
		static EngineGUI instance;
		return instance;
	}

	static void RenderGUI();
	static void CommitGUI();

	static void SetSceneManagerRef(SceneManager* ref_pointer);

	static void RegisterConsoleLog(const ConsoleLogEvent& event);

private:

	void RenderTopBar();
	void RenderBottomBar();
	void RenderHierarchy();
	void RenderConsole();
};

