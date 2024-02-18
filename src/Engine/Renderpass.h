#pragma once
#include "Scene.h"
#include "PipelineManager.h"

class Renderer;

class IRenderpass
{
private:

	bool m_isActive = true;

protected:

	PipelineManager* m_pipeline = nullptr;
	Renderer* m_renderer = nullptr;

public:
	IRenderpass() = default;
	IRenderpass(PipelineManager* pipe, Renderer* render);
	virtual ~IRenderpass() = 0 {};

	void SetActive(const bool& tog);
	const bool& IsActive() const;

	virtual void Create() = 0;
	virtual void Prepass() = 0;
	virtual void Pass(Scene* currentScene) = 0;
	virtual void Postpass() = 0;
};