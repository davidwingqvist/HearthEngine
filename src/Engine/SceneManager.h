#pragma once
#include "Scene.h"

class SceneManager
{
private:

	std::unordered_map<std::string, InternalScene*> m_scenes;
	InternalScene* m_currentScene;
	std::string m_currentSceneName;
	std::vector<std::string> m_sceneNames;

public:

	SceneManager();
	~SceneManager();

	InternalScene* GetCurrentScene() const;

	void AddInternalScene(const std::string& sceneName);
	void AddScene(const std::string& sceneName);
	void SetScene(const std::string& sceneName);
	InternalScene* GetScene(const std::string& sceneName);

	const std::vector<std::string>& GetSceneNames() const;

};