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

GameScene::GameScene()
	:InternalScene()
{
	this->SetupComponents();
}
