#pragma once
#include "Scene.h"
#include "PipelineManager.h"

class Renderer;

class IRenderpass
{
protected:

	PipelineManager* m_pipeline = nullptr;
	Renderer* m_renderer = nullptr;
	bool m_isActive = true;

public:
	IRenderpass() = default;
	IRenderpass(PipelineManager* pipe, Renderer* render);
	virtual ~IRenderpass() = 0 {};

	void SetActive(const bool& tog);
	const bool& IsActive() const;

	virtual void Create() = 0;
	virtual void Prepass() = 0;
	virtual void Pass(InternalScene* currentScene) = 0;
	virtual void Postpass() = 0;
};