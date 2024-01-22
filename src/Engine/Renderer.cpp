#include "Header.h"
#include "Renderer.h"

Renderer::Renderer()
	:m_basicPass(&m_pipelineManager), m_lightPass(&m_pipelineManager)
{
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	m_pipelineManager.Initialize();

	m_basicPass.Create();
	m_lightPass.Create();
	m_cubeMapPass.Create();

	m_renderPasses.push_back(&m_basicPass);
	m_renderPasses.push_back(&m_lightPass);
	m_renderPasses.push_back(&m_cubeMapPass);
}

void Renderer::Draw(Scene* currentScene)
{
	for (auto& pass : m_renderPasses)
	{
		if (pass->IsActive())
		{
			pass->Prepass();
			pass->Pass(currentScene);
			pass->Postpass();
		}
	}
}

PipelineManager& Renderer::GetPipelineManager()
{
	return m_pipelineManager;
}
