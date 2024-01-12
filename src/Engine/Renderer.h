#pragma once
#include "Renderpass.h"
#include "PipelineManager.h"
#include "BasicPass.h"
#include "LightPass.h"

class Renderer
{
private:

	BasicPass m_basicPass; // G-Pass before Light
	LightPass m_lightPass; // Light pass, based on basic pass.


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