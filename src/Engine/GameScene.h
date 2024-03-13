#pragma once
#include "InternalScene.h"

// Game Scene for purpose of creating game world.
class GameScene : public InternalScene
{
public:
	void Update() override;
	void Awake() override;
	void Draw() override;
	void PreDraw() override;
	void PostDraw() override;

	GameScene();
private:
};