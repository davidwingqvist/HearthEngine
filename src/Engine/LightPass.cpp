#include "Header.h"
#include "LightPass.h"

LightPass::LightPass(PipelineManager* pipe)
	:IRenderpass(pipe)
{
}

LightPass::~LightPass()
{
}

void LightPass::Prepass()
{
}

void LightPass::Pass(Scene* currentScene)
{
}

void LightPass::Postpass()
{
}

void LightPass::Create()
{
	m_lightVertex.Create("LightVertexShader");
	m_lightPixel.Create("LightPixelShader");
}
