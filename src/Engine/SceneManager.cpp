#include "Header.h"
#include "SceneManager.h"
#include "Debugger.h"

SceneManager::SceneManager()
{
	m_currentScene = nullptr;
}

SceneManager::~SceneManager()
{
	delete m_currentScene;
}

void SceneManager::AddScene(const std::string& sceneName)
{
	m_currentScene = new Scene();
	/*Scene newScene;
	m_scenes.emplace(sceneName, std::move(newScene));
	Debugger::Get().Print("Added Scene: '" + sceneName + "'\n", Debugger::COLOR_GREEN);*/
}

void SceneManager::SetScene(const std::string& sceneName)
{
	if (m_scenes.find(sceneName) == m_scenes.end())
	{
		Debugger::Get().Print("No scene with the name: '" + sceneName + "' found.\n", Debugger::COLOR_RED);
		return;
	}

	Debugger::Get().Print("Scene set to: '" + sceneName + "'\n", Debugger::COLOR_GREEN);
	m_currentScene = &m_scenes[sceneName];
}

Scene* SceneManager::GetScene(const std::string& sceneName)
{
	return &m_scenes[sceneName];
}

Scene* SceneManager::GetCurrentScene() const
{
	return m_currentScene;
}