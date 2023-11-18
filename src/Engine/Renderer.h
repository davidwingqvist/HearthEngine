#pragma once
#include "Renderpass.h"
#include "PipelineManager.h"
#include "BasicPass.h"

class Renderer
{
private:

	BasicPass m_basicPass;

private:

	std::vector<IRenderpass*> m_renderPasses;
	PipelineManager m_pipelineManager;

public:

	Renderer();
	~Renderer();

	void Initialize();

	void Draw(Scene* currentScene);

	PipelineManager& GetPipelineManager();
};