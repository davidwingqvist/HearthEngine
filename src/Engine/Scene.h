#pragma once
#include "DrawManager.h"

class Scene
{
private:

	recs::recs_registry m_registry;
	std::function<void(recs::recs_registry&)> m_function;
	DrawManager m_drawManager;
	ID3D11Buffer* m_publicBuffer;

	bool CreatePublicBuffer();
	void PreDraw();

public:

	Scene();
	~Scene();

	void SetLogic(std::function<void(recs::recs_registry&)> function);

	void Update();

	void Draw();
};