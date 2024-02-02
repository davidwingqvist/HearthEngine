#pragma once
#include "DrawManager.h"
#include "Camera.h"
#include "DXPointer.h"

class Scene
{
private:

	recs::recs_registry m_registry;
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

	void Update();

	void Draw();

	recs::recs_registry& GetRegistry();
};