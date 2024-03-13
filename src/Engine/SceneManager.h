#pragma once
#include "Scene.h"

class SceneManager
{
private:

	std::unordered_map<std::string, InternalScene*> m_scenes;
	InternalScene* m_currentScene;

public:

	SceneManager();
	~SceneManager();

	InternalScene* GetCurrentScene() const;

	void AddScene(const std::string& sceneName);
	void SetScene(const std::string& sceneName);
	InternalScene* GetScene(const std::string& sceneName);

};