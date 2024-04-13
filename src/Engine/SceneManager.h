#pragma once
#include "Scene.h"
#include "GameScene.h"

class SceneManager
{
private:

	std::unordered_map<std::string, InternalScene*> m_scenes;
	InternalScene* m_currentScene;
	Scene* m_editScene = nullptr;
	std::string m_currentSceneName;
	std::vector<std::string> m_sceneNames;

	void LoadSpecificScene(const std::string& sceneName);

public:

	SceneManager();
	~SceneManager();

	InternalScene* GetCurrentScene() const;

	GameScene* GetCurrentGameScene();

	void AddInternalScene(const std::string& sceneName);
	void AddScene(const std::string& sceneName);
	void SetScene(const std::string& sceneName);
	void SetSceneForEdit(const std::string& sceneName);
	void RemoveScene(const std::string& sceneName);
	InternalScene* GetScene(const std::string& sceneName);
	const std::string& GetCurrentSceneName() const;

	void StartGameScene(const std::string& sceneName);

	// Load scenes from file.
	void LoadScenes();

	// Save scenes to file.
	void SaveScenes();

	// Remove scenes and data from them.
	void ResetScenes();

	const std::vector<std::string>& GetSceneNames() const;

};