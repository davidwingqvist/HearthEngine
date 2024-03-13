#pragma once
#include "SceneManager.h"
#include "LightPass.h"

class EngineGUI
{
private:
	EngineGUI();
	~EngineGUI();

	SceneManager* m_sceneManagerRef = nullptr;

	bool m_showEditTab = false;

	// Script Tab values
	char m_createScriptPath[36] = {};

	// Object Tab values
	bool m_showObjectsTab = false;

	// Properties Tab Values
	recs::Entity m_currentEntity = recs::NULL_ENTITY;
	bool m_showPropertiesTab = true;
	char m_createScriptPathProp[36] = {};

	bool m_showNewComponentTab = false;

	char m_modelinputField[100] = {};
	char m_textureInputField[100] = {};

	// Bottom Bar Values
	bool m_showBottomFiles = false;
	bool m_showBottomConsole = false;
	bool m_showBottomStatistics = false;

	// Console Window Values
	std::vector<ConsoleLogEvent> m_consoleLogs;
	char m_consoleWindowInput[100] = {};
	void ClearConsoleWindowText();
	void ClearConsoleLogToLimit();
	int m_consoleLogLimit = 200;
	// Console window log filter.
	UINT m_filter = (UINT)ConsoleLogEventType::ALL;

	// File keeping window values
	bool m_showFileKeeper = false;

	/*
	
		Resources Tabs
	
	*/

	// Models
	bool m_showModelsTab = false;
	bool m_showTextureTab = false;
	bool m_showScriptsTab = false;
	bool m_showScriptsHandlerTab = false;


	/*
	
		Resources Tabs
	
	*/

	void PutEditTabsToFalse();
	void BottomBarPutToFalse();
public:

	/*
		Temporary values
	*/
	LightPass* m_lightPassRef = nullptr;

	static auto& Get()
	{
		static EngineGUI instance;
		return instance;
	}

	static void RenderGUI();
	static void CommitGUI();

	static void SetSceneManagerRef(SceneManager* ref_pointer);

	static void RegisterConsoleLog(const ConsoleLogEvent& event);
	static ConsoleLogEvent& AdjustConsoleLog(const size_t& pos);
	static const size_t& GetConsoleLogSize();

private:

	void RenderTopBar();
	void RenderBottomBar();
	void RenderHierarchy();
	void RenderConsole();
	void RenderStatistics();
	void RenderProperties();
	void RenderFileKeepingWindow();
	void RenderNewComponentTab();
	void RenderScriptsTab();
	void RenderModelsTab();
	void RenderTextureTab();
};

