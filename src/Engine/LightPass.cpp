#include "Header.h"
#include "LightPass.h"
#include "D3D11Context.h"
#include "Debugger.h"

bool LightPass::SetUpScreenTriangles()
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(vertex_data) * 4;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = &m_screenVertexes[0];

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, &m_vertexBuffer);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Vertex buffer for Light pass!\n")
			return false;
	}

	D3D11_BUFFER_DESC indDesc{};
	indDesc.ByteWidth = sizeof UINT * 6;
	indDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indData{};
	indData.pSysMem = &m_screenIndices[0];

	hr = D3D11Core::Get().Device()->CreateBuffer(&indDesc, &indData, &m_indicesBuffer);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Index buffer for Light pass!\n")
			return false;
	}

	return true;
}

LightPass::LightPass(PipelineManager* pipe)
	:IRenderpass(pipe)
{
}

LightPass::~LightPass()
{
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indicesBuffer)
		m_indicesBuffer->Release();
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
	SetUpScreenTriangles();
}
