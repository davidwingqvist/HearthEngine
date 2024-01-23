#include "Header.h"
#include "WireFramePass.h"
#include "Debugger.h"
#include "D3D11Context.h"

void WireFramePass::BuildGrid(const sm::Vector3& midPoint, const sm::Vector2& size, const float& offset)
{
	// Initialize the working values.
	m_middlePoint = midPoint;
	m_size = size;
	m_offset = offset;

	// Generate points.
	sm::Vector3 startPoint = { m_middlePoint.x - (m_size.x / 2.0f), m_middlePoint.y, m_middlePoint.z - (m_size.y / 2.0f) };
	UINT xSteps = (UINT)m_size.x / (UINT)m_offset;
	UINT zSteps = (UINT)m_size.y / (UINT)m_offset;

	float currentX = startPoint.x;
	UINT currentID = 0;
	for (UINT i = 0; i < xSteps; i++)
	{
		m_points.push_back({ { currentX, m_middlePoint.y, startPoint.z}, {0} });
		m_points.push_back({{ currentX, m_middlePoint.y, startPoint.z + m_size.y - m_offset},{0}});
		currentX += m_offset;
		m_indices.push_back(currentID);
		currentID++;
		m_indices.push_back(currentID);
		currentID++;
	}

	currentX = startPoint.x;
	float currentZ = startPoint.z;
	for (UINT i = 0; i < zSteps; i++)
	{
		m_points.push_back({ { startPoint.x, m_middlePoint.y, currentZ}, {0} });
		m_points.push_back({ { startPoint.x + m_size.x - m_offset, m_middlePoint.y, currentZ},{0} });
		currentZ += m_offset;
		m_indices.push_back(currentID);
		currentID++;
		m_indices.push_back(currentID);
		currentID++;
	}
}

bool WireFramePass::CreateShaders()
{
	if (!m_vertexShader.Create("WireFrameVertexShader"))
		return false;

	if (!m_pixelShader.Create("WireFramePixelShader"))
		return false;

	return true;
}

bool WireFramePass::CreateInput()
{
	HRESULT hr = S_FALSE;

	// Create m_defaultInputLayout.
	std::string shaderByteCode = m_vertexShader.GetShaderByteCode();
	D3D11_INPUT_ELEMENT_DESC defaultVertexShaderDesc[] =
	{
		{"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                0,                   D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(hr = D3D11Core::Get().Device()->CreateInputLayout(defaultVertexShaderDesc, ARRAYSIZE(defaultVertexShaderDesc), shaderByteCode.c_str(), shaderByteCode.length(), &m_inputLayout)))
	{
		DEBUG_ERROR("Failed creating input layout for WireFramePass\n");
		return false;
	}

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(point_data) * (UINT)m_points.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = &m_points[0];

	hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, &m_vertexBuffer);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Vertex buffer for Light pass!\n")
			return false;
	}

	return true;
}

inline bool WireFramePass::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC indDesc{};
	indDesc.ByteWidth = sizeof UINT * (UINT)m_indices.size();
	indDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indData{};
	indData.pSysMem = &m_indices[0];

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&indDesc, &indData, &m_indexBuffer);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Index buffer for Light pass!\n")
			return false;
	}

	return true;
}

inline bool WireFramePass::CreateColorBuffer()
{
	D3D11_BUFFER_DESC indDesc{};
	indDesc.ByteWidth = sizeof sm::Vector4;
	indDesc.Usage = D3D11_USAGE_DYNAMIC;
	indDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	indDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA indData{};
	indData.pSysMem = &m_gridColor;

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&indDesc, &indData, &m_gridColorBuffer);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Index buffer for Light pass!\n")
			return false;
	}

	return true;
}

inline bool WireFramePass::UpdateColorBuffer()
{
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT hr = D3D11Core::Get().Context()->Map(m_gridColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to update new color to the DirectX buffer\n");
		return false;
	}
	std::memcpy(sub.pData, &m_gridColor, sizeof sm::Vector4);
	D3D11Core::Get().Context()->Unmap(m_gridColorBuffer, 0);
	return true;
}

WireFramePass::WireFramePass()
{
	BuildGrid({ 0.0f, 0.0f, 0.0f }, { 1000.0f, 1000.0f }, 25);
	m_gridColor = { 0.f, 1.f, 0.f, 1.f }; 
}
  
WireFramePass::~WireFramePass()
{
	if (m_inputLayout)
		m_inputLayout->Release();
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
	if (m_gridColorBuffer)
		m_gridColorBuffer->Release();
}

void WireFramePass::Create()
{
	if (!CreateShaders())
	{
		DEBUG_ERROR("Creating Shaders for WireFramePass failed!\n")
	}

	CreateInput();

	CreateIndexBuffer();

	CreateColorBuffer();

	SetGridColor({ .8f, .1f, .75f });
}

void WireFramePass::Prepass()
{
	D3D11Core::Get().Context()->PSSetShader(m_pixelShader.Get(), NULL, 0);
	D3D11Core::Get().Context()->VSSetShader(m_vertexShader.Get(), NULL, 0);
	D3D11Core::Get().Context()->IASetInputLayout(m_inputLayout);

	const UINT stride = sizeof point_data;
	const UINT offset = 0;
	D3D11Core::Get().Context()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	D3D11Core::Get().Context()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D11Core::Get().Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D11Core::Get().Context()->PSSetConstantBuffers(0, 1, &m_gridColorBuffer);
}

void WireFramePass::Pass(Scene* currentScene)
{
	D3D11Core::Get().Context()->DrawIndexed((UINT)m_indices.size(), 0, 0);
}

void WireFramePass::Postpass()
{
}

void WireFramePass::SetGridColor(const sm::Vector3& newColor)
{
	m_gridColor = { newColor.x, newColor.y, newColor.z, 1.0f };
	UpdateColorBuffer();
}
