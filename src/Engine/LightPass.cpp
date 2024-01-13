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
	D3D11Core::Get().Context()->VSSetShader(m_lightVertex.Get(), nullptr, 0);
	D3D11Core::Get().Context()->PSSetShader(m_lightPixel.Get(), nullptr, 0);

	const UINT stride = sizeof(vertex_data);
	const UINT offset = 0;
	D3D11Core::Get().Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11Core::Get().Context()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	D3D11Core::Get().Context()->IASetIndexBuffer(m_indicesBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11Core::Get().Context()->OMSetRenderTargets(1, &m_pipeline->m_backBuffer, nullptr);

	D3D11Core::Get().Context()->PSSetSamplers(0, 1, &m_pipeline->m_anisotropicSamplerState);
}

void LightPass::Pass(Scene* currentScene)
{
	D3D11Core::Get().Context()->DrawIndexed(6, 0, 0);
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
