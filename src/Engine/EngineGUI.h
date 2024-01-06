#pragma once
#include "SceneManager.h"

class EngineGUI
{
private:
	EngineGUI();
	~EngineGUI();

	SceneManager* m_sceneManagerRef = nullptr;

	bool m_showEditTab = false;

	// Script values
	bool m_showScriptsTab = false;
	char m_createScriptPath[36] = {};

	// Object values
	bool m_showObjectsTab = false;

public:
	static auto& Get()
	{
		static EngineGUI instance;
		return instance;
	}

	static void RenderGUI();
	static void CommitGUI();

	static void SetSceneManagerRef(SceneManager* ref_pointer);

private:

	void RenderTopBar();
};

