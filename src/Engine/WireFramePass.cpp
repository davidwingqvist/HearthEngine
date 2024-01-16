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
	FLOAT currentID = 0;
	for (int i = 0; i < xSteps; i++)
	{
		float currentZ = startPoint.z;
		m_points.push_back({ { currentX, m_middlePoint.y, currentZ}, {0} });
		m_points.push_back({{ currentX, m_middlePoint.y, currentZ + m_size.y - m_offset},{0}});
		currentZ += m_offset;
		currentX += m_offset;
		m_indices.push_back(currentID);
		currentID++;
		m_indices.push_back(currentID);
		currentID++;
	}

	currentX = startPoint.x;
	float currentZ = startPoint.z;
	for (int i = 0; i < zSteps; i++)
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
	desc.ByteWidth = sizeof(point_data) * m_points.size();
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
	indDesc.ByteWidth = sizeof UINT * m_indices.size();
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

WireFramePass::WireFramePass()
{
	BuildGrid({ 0.0f, 0.0f, 0.0f }, { 1000.0f, 1000.0f }, 25);
}
  
WireFramePass::~WireFramePass()
{
	if (m_inputLayout)
		m_inputLayout->Release();
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
}

void WireFramePass::Create()
{
	if (!CreateShaders())
	{
		DEBUG_ERROR("Creating Shaders for WireFramePass failed!\n")
	}

	CreateInput();

	CreateIndexBuffer();
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
}

void WireFramePass::Pass(Scene* currentScene)
{
	D3D11Core::Get().Context()->DrawIndexed(m_indices.size(), 0, 0);
}

void WireFramePass::Postpass()
{
}
