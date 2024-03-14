#include "Header.h"
#include "SceneManager.h"
#include "Debugger.h"
#include "GameScene.h"

SceneManager::SceneManager()
{
	m_currentScene = nullptr;
}

SceneManager::~SceneManager()
{
	for (auto& scene : m_scenes)
	{
		delete scene.second;
	}
}

void SceneManager::AddInternalScene(const std::string& sceneName)
{
	Scene* newScene = new Scene();
	newScene->SetSceneName(sceneName);
	m_scenes[sceneName] = newScene;
	DEBUG_INFO("Added Edit Scene: " + sceneName);
}

void SceneManager::AddScene(const std::string& sceneName)
{
	GameScene* newScene = new GameScene();
	newScene->SetSceneName(sceneName);
	m_scenes[sceneName] = newScene;
	m_currentSceneName = sceneName;
	m_sceneNames.push_back(sceneName);
	Debugger::Get().Print("Added Scene: '" + sceneName + "'\n", Debugger::COLOR_GREEN);
}

void SceneManager::SetScene(const std::string& sceneName)
{
	if (m_scenes.find(sceneName) == m_scenes.end())
	{
		DEBUG_ERROR("No scene with the name: '" + sceneName + "' found.")
		return;
	}

	DEBUG_INFO("Scene set to: '" + sceneName + "'\n")
	m_currentScene = m_scenes[sceneName];
	m_currentScene->Assign();
}

void SceneManager::SetSceneForEdit(const std::string& sceneName)
{
	if (m_scenes.find(sceneName) == m_scenes.end())
	{
		DEBUG_ERROR("No scene with the name: '" + sceneName + "' found.")
			return;
	}

	DEBUG_INFO("Scene set to: '" + sceneName + "'\n");

	m_scenes[sceneName]->AssignEdit();
	m_currentSceneName = sceneName;
}

InternalScene* SceneManager::GetScene(const std::string& sceneName)
{
	return m_scenes[sceneName];
}

const std::vector<std::string>& SceneManager::GetSceneNames() const
{
	return m_sceneNames;
}

InternalScene* SceneManager::GetCurrentScene() const
{
	return m_currentScene;
}