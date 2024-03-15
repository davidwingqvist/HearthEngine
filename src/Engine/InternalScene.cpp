#include "Header.h"
#include "InternalScene.h"
#include "ResourceManager.h"
#include "LuaGameState.h"
#include "LuaState.h"
#include "Texture.h"
#include "Utility.h"
#include "Time.h"
#include "EngineGUI.h"
#include "D3D11Context.h"

void InternalScene::Assign()
{
	m_camera.Activate();
	LUA.m_currentRegistry = &m_registry;
}

void InternalScene::AssignEdit(InternalScene* scene)
{

}

void InternalScene::SetSceneName(const std::string& sceneName)
{
	m_sceneName = sceneName;
	m_registry.SetDataFolderPath("Options/recs/" + m_sceneName + "_");
}

void InternalScene::SetupComponents()
{
	m_registry.RegisterComponent<Model>();

	m_registry.RegisterDataToState(GameObject());
	m_registry.RegisterDataToState(Transform());
	m_registry.RegisterDataToState(Light());
	m_registry.RegisterDataToState(RigidBody());
	m_registry.RegisterDataToState(ModelID());
	m_registry.RegisterDataToState(Script());

	m_registry.RegisterOnCreate<ModelID>([&](const recs::Entity& entity, ModelID& id)
		{
			if ((!m_registry.HasComponent<Model>(entity) && ((id.model_id != 0) || (id.texture_id != 0))))
			{
				Model* model = m_registry.AddComponent<Model>(entity);

				if (id.model_id != 0)
				{
					model->model_data = ResourceManager::Get().GetResource<Model3D>(id.model_id).get();
				}
				if (id.texture_id != 0)
				{
					model->model_texture = ResourceManager::Get().GetResource<Texture>(id.texture_id).get();
				}
			}
		});

	m_registry.RegisterOnCreate<Script>([&](const recs::Entity& entity, Script& script) {

		LUA_GAME.SetCurrentEntity(entity);
		for (int i = 0; i < 5; i++)
		{
			LUA_GAME.CreateObjectFromScript(script.script_id[i], entity);
			//LUA_GAME.AwakeObjectFromScript(script.script_id[i], entity);
		}

		});

	m_registry.RegisterOnUpdate<Script>([&](const recs::Entity& entity, Script& script) {

		LUA_GAME.SetCurrentEntity(entity);
		for (int i = 0; i < 5; i++)
		{
			LUA_GAME.UpdateObjectFromScript(script.script_id[i], entity);
		}

		});

	pushTransform(LUA.State());
}

recs::recs_registry* InternalScene::GetRegistry()
{
    return &m_registry;
}

InternalScene::InternalScene()
{
}
