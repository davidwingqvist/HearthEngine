#include "Header.h"
#include "GameScene.h"
#include "LuaState.h"

void GameScene::Update()
{
	m_registry.Update();
}

void GameScene::Awake()
{

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
