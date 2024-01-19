#include "Header.h"
#include "CubeMapPass.h"

CubeMapPass::~CubeMapPass()
{
	if (m_cubeMap)
		m_cubeMap->Release();
	if (m_cubeMapView)
		m_cubeMapView->Release();
}

void CubeMapPass::Create()
{
}

void CubeMapPass::Prepass()
{
}

void CubeMapPass::Pass(Scene* currentScene)
{
}

void CubeMapPass::Postpass()
{
}
