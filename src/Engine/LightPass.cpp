#include "Header.h"
#include "LightPass.h"
#include "D3D11Context.h"
#include "Debugger.h"

constexpr UINT MAX_LIGHTS = 50;

bool LightPass::SetUpScreenTriangles()
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(vertex_data) * 4;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = &m_screenVertexes[0];

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf());
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

	hr = D3D11Core::Get().Device()->CreateBuffer(&indDesc, &indData, m_indicesBuffer.GetAddressOf());
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
	m_basicPassRef = nullptr;
}

LightPass::~LightPass()
{
}

void LightPass::GatherLights(recs::recs_registry* reg)
{
	m_lightDataVector.clear();
	m_lightDataVector.reserve(reg->GetComponentRegistry().GetEntityLinks<Light>().size());
	
	reg->Group<Light, Transform>().ForEach([&](Light& light, Transform& transf) {

		light.data = transf.pos;

		m_lightDataVector.push_back(light);

		});

	m_nrOfRegLights = m_lightDataVector.size();

	m_lightInfo.x = (float)m_nrOfRegLights;

	this->UpdateLightBuffer();

	// Set buffers to pipeline.
	DC->PSSetConstantBuffers(0, 1, m_lightData.GetAddressOf());
	DC->PSSetConstantBuffers(1, 1, m_lightInfoBuffer.GetAddressOf());
}

void LightPass::SetUpLightBuffer()
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(Light) * MAX_LIGHTS;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, 0, m_lightData.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Vertex buffer for Light pass!\n")
	}

	desc.ByteWidth = sizeof sm::Vector4;
	hr = DD->CreateBuffer(&desc, 0, m_lightInfoBuffer.GetAddressOf());
}

void LightPass::Prepass()
{
	m_basicPassRef->SetLightPassValues();

	D3D11Core::Get().Context()->VSSetShader(m_lightVertex.Get(), nullptr, 0);
	D3D11Core::Get().Context()->PSSetShader(m_lightPixel.Get(), nullptr, 0);
	D3D11Core::Get().Context()->IASetInputLayout(m_pipeline->m_defaultInputLayout.Get());

	const UINT stride = sizeof(vertex_data);
	const UINT offset = 0;
	D3D11Core::Get().Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11Core::Get().Context()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	D3D11Core::Get().Context()->IASetIndexBuffer(m_indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	D3D11Core::Get().Context()->OMSetRenderTargets(1, m_pipeline->m_backBuffer.GetAddressOf(), nullptr);
	D3D11Core::Get().Context()->OMSetBlendState(m_pipeline->m_blendStateAlphaBlending.Get(), NULL, 0xffffffff);
	D3D11Core::Get().Context()->RSSetState(m_pipeline->m_rasterState.Get());
	D3D11Core::Get().Context()->PSSetSamplers(0, 1, m_pipeline->m_linearSamplerState.GetAddressOf());
}

void LightPass::Pass(Scene* currentScene)
{
	if (m_nrOfRegLights != currentScene->GetRegistry().GetSize<Light>())
	{
		this->GatherLights(&currentScene->GetRegistry());
	}

	D3D11Core::Get().Context()->DrawIndexed(6, 0, 0);
}

void LightPass::UpdateLightBuffer()
{
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT hr = D3D11Core::Get().Context()->Map(m_lightData.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to update light data buffer\n");
	}
	std::memcpy(sub.pData, &m_lightDataVector[0], sizeof(Light) * m_lightDataVector.size());
	D3D11Core::Get().Context()->Unmap(m_lightData.Get(), 0);

	D3D11_MAPPED_SUBRESOURCE sub2;
	hr = D3D11Core::Get().Context()->Map(m_lightInfoBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub2);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to update light info buffer\n");
	}
	std::memcpy(sub2.pData, &m_lightInfo, sizeof(sm::Vector4));
	D3D11Core::Get().Context()->Unmap(m_lightInfoBuffer.Get(), 0);
}

void LightPass::Postpass()
{
	D3D11Core::Get().Context()->OMSetBlendState(nullptr, NULL, 0xffffffff);
}

void LightPass::Create()
{
	m_lightVertex.Create("LightVertexShader");
	m_lightPixel.Create("LightPixelShader");
	SetUpScreenTriangles();
	this->SetUpLightBuffer();
}
