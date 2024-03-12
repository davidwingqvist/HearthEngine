#pragma once
#include "InternalScene.h"

class GameScene : public InternalScene
{
public:
	void Update() override;
	void Awake() override;
	void Draw() override;
private:

};