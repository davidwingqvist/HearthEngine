#pragma once
#include "Renderpass.h"
#include "PipelineManager.h"
#include "BasicPass.h"
#include "LightPass.h"
#include "CubeMapPass.h"

class Renderer
{
protected:

	BasicPass m_basicPass; // G-Pass before Light
	LightPass m_lightPass; // Light pass, based on basic pass.
	CubeMapPass m_cubeMapPass; // Cube map

private:

	std::vector<IRenderpass*> m_renderPasses;
	PipelineManager m_pipelineManager;

public:

	Renderer();
	~Renderer();

	void Initialize();

	void Draw(Scene* currentScene);

	PipelineManager& GetPipelineManager();

	friend class BasicPass;
	friend class LightPass;
	friend class CubeMapPass;
};