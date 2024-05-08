#include "Header.h"
#include "SceneManager.h"
#include "Debugger.h"
#include "GameScene.h"
#include "EngineGUI.h"

void SceneManager::LoadSpecificScene(const std::string& sceneName)
{

}

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

	if (m_editScene)
		delete m_editScene;
}

void SceneManager::AddInternalScene(const std::string& sceneName)
{
	Scene* newScene = new Scene();
	newScene->SetSceneName(sceneName);
	m_editScene = newScene;
	m_currentScene = m_editScene;
	m_currentSceneName = "None selected.";
	m_editScene->Assign();
	DEBUG_INFO("Added Edit Scene: " + sceneName);
}

void SceneManager::AddScene(const std::string& sceneName)
{
	GameScene* newScene = new GameScene();
	newScene->SetSceneName(sceneName);
	m_scenes[sceneName] = newScene;
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

	DEBUG_INFO("Scene edit set to: '" + sceneName + "'\n");

	m_editScene->AssignEdit(m_scenes[sceneName]);
	m_currentSceneName = sceneName;
}

void SceneManager::RemoveScene(const std::string& sceneName)
{
	if (sceneName == this->m_currentSceneName)
	{
		m_editScene->Reset();
		this->m_currentScene = m_editScene;
		m_currentSceneName = "None selected.";
	}

	delete m_scenes[sceneName];
	m_scenes.erase(sceneName);

	for (int i = 0; i < m_sceneNames.size(); i++)
	{
		if (m_sceneNames[i] == sceneName)
		{
			m_sceneNames.erase(m_sceneNames.begin() +  i);
			break;
		}
	}
}

InternalScene* SceneManager::GetScene(const std::string& sceneName)
{
	return m_scenes[sceneName];
}

const std::string& SceneManager::GetCurrentSceneName() const
{
	return m_currentSceneName;
}

void SceneManager::StartGameScene(const std::string& sceneName)
{
	m_scenes[m_currentSceneName]->Awake();
	m_currentScene = m_scenes[m_currentSceneName];
}

void SceneManager::LoadScenes()
{
	std::ifstream stream(OPTIONPATH + "Scenes.txt");

	std::string line;
	while (std::getline(stream, line))
	{
		AddScene(line);
	}
}

void SceneManager::SaveScenes()
{
	std::ofstream outfile(OPTIONPATH + "Scenes.txt");

	for (auto& scene : m_sceneNames)
	{
		outfile << scene << "\n";
	}
}

void SceneManager::ResetScenes()
{
	for (auto& scene : m_scenes)
	{
		delete scene.second;
	}

	m_scenes.clear();
	m_sceneNames.clear();
	m_editScene->Reset();
}

const std::vector<std::string>& SceneManager::GetSceneNames() const
{
	return m_sceneNames;
}

InternalScene* SceneManager::GetCurrentScene() const
{
	return m_currentScene;
}

GameScene* SceneManager::GetCurrentGameScene()
{
	return dynamic_cast<GameScene*>(GetScene(GetCurrentSceneName()));
}
