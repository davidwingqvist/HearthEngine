#pragma once
#include "Scene.h"

class SceneManager
{
private:

	std::unordered_map<std::string, Scene> m_scenes;
	Scene* m_currentScene;

public:

	SceneManager();
	~SceneManager();

	Scene* GetCurrentScene() const;

	void AddScene(const std::string& sceneName);
	void SetScene(const std::string& sceneName);
	Scene* GetScene(const std::string& sceneName);

};