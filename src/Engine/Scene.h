#pragma once
#include "DrawManager.h"
#include "Camera.h"
#include "DXPointer.h"
#include "InternalScene.h"

class Scene : public InternalScene
{
private:

	//recs::recs_registry m_registry;
	DrawManager m_drawManager;
	DXPointer<ID3D11Buffer> m_publicBuffer;
	recs::Entity entity;
	Camera m_camera;

	bool CreatePublicBuffer();
	void PreDraw();

private:

	void SetupComponents();
	void RegisterComponentsToLua();

public:

	Scene();
	~Scene();

	void Update() override;

	void Awake() override;

	void Draw() override;

	recs::recs_registry& GetRegistry();
};