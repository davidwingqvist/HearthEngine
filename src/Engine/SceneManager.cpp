#include "Header.h"
#include "SceneManager.h"
#include "Debugger.h"

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

void SceneManager::AddScene(const std::string& sceneName)
{
	Scene* newScene = new Scene();
	m_scenes[sceneName] = newScene;
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
}

InternalScene* SceneManager::GetScene(const std::string& sceneName)
{
	return m_scenes[sceneName];
}

InternalScene* SceneManager::GetCurrentScene() const
{
	return m_currentScene;
}