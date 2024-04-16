#pragma once
#include "DrawManager.h"
#include "DXPointer.h"
#include "Camera.h"

class InternalScene
{
public:

	virtual void Update() = 0;
	virtual void Awake() = 0;
	virtual void Draw() = 0;
	virtual void PreDraw() = 0;
	virtual void PostDraw() = 0;

	// Assign this scene as the current scene.
	virtual void Assign();

	// Assign this scene for editing.
	virtual void AssignEdit(InternalScene* scene);

	// Set the scene name reference.
	void SetSceneName(const std::string& sceneName);

	void SetupComponents();

	virtual recs::recs_registry* GetRegistry();

	InternalScene();

protected:

	recs::recs_registry m_registry;
	DrawManager m_drawManager;
	std::string m_sceneName;
};