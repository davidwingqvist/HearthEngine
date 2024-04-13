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

	void SetCameraPoint(Transform* t, CameraPoint* p);

	void UpdateCamera();

	GameScene();
private:
	Transform* m_transform = nullptr;
	CameraPoint* m_cameraPoint = nullptr;
	UINT m_activeCameraID = -1;
	GameCamera m_gameCamera;
};