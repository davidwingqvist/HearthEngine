#pragma once
#include "InternalScene.h"

// Used for internal editing scene for editor.
class Scene : public InternalScene
{
private:

	//recs::recs_registry m_registry;
	DrawManager m_drawManager;
	DXPointer<ID3D11Buffer> m_publicBuffer;

	bool CreatePublicBuffer();

private:

	//void SetupComponents();
	void RegisterComponentsToLua();

public:

	Scene();
	~Scene();

	void PreDraw() override;

	void PostDraw() override;

	void Update() override;

	void Awake() override;

	void Draw() override;

	recs::recs_registry& GetRegistry();
};