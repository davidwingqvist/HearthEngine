#include "Header.h"
#include "BasicPass.h"
#include "Debugger.h"
#include "D3D11Context.h"

BasicPass::BasicPass(PipelineManager* pipe)
	:IRenderpass(pipe)
{
}

BasicPass::~BasicPass()
{

}

void BasicPass::Prepass()
{
	D3D11Core::Get().Context()->VSSetShader(m_pipeline->m_baseVertexShader.Get(), nullptr, 0);
	D3D11Core::Get().Context()->PSSetShader(m_pipeline->m_basePixelShader.Get(), nullptr, 0);
	D3D11Core::Get().Context()->IASetInputLayout(m_pipeline->m_defaultInputLayout);
	D3D11Core::Get().Context()->OMSetRenderTargets(1, &m_pipeline->m_backBuffer, m_pipeline->m_depthStencilView);
}

void BasicPass::Pass(Scene* currentScene)
{
	currentScene->Draw();
}

void BasicPass::Postpass()
{

}