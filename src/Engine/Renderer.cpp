#include "Header.h"
#include "Renderer.h"
#include "EngineGUI.h"

Renderer::Renderer()
	:m_basicPass(&m_pipelineManager, this), m_lightPass(&m_pipelineManager, this)
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

	EngineGUI::Get().m_lightPassRef = &m_lightPass;

	m_renderPasses.push_back(&m_basicPass);
	m_renderPasses.push_back(&m_cubeMapPass);
	m_renderPasses.push_back(&m_lightPass);
}

void Renderer::Draw(InternalScene* currentScene)
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
