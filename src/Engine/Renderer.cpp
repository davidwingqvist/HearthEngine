#include "Header.h"
#include "Renderer.h"


Renderer::Renderer()
	:m_basicPass(&m_pipelineManager)
{
	m_renderPasses.push_back(&m_basicPass);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	m_pipelineManager.Initialize();
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
