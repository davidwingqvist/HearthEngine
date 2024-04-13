#include "Header.h"
#include "GameScene.h"
#include "LuaState.h"
#include "LuaGameState.h"

void GameScene::Update()
{
	m_registry.Update();
}

void GameScene::Awake()
{
	m_registry.View<Script>().ForEach([&](const recs::Entity& entity, Script& script) {

		LUA_GAME.SetCurrentEntity(entity);
		for (int i = 0; i < 5; i++)
		{
			LUA_GAME.AwakeObjectFromScript(script.script_id[i], entity);
		}

		});

	m_gameCamera.Activate();
}

void GameScene::Draw()
{
}

void GameScene::PreDraw()
{
}

void GameScene::PostDraw()
{
}

void GameScene::SetCameraPoint(Transform* t, CameraPoint* p)
{
	if (t && p)
	{
		m_gameCamera.UpdateInfo(t, p);
		m_transform = t;
		m_cameraPoint = p;
	}
}

void GameScene::UpdateCamera()
{
	if (m_transform && m_cameraPoint)
	{
		m_gameCamera.UpdateInfo(m_transform, m_cameraPoint);
	}
}

GameScene::GameScene()
	:InternalScene()
{
	this->SetupComponents();
}
