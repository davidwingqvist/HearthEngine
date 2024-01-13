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
	for (UINT i = 0; i < xSteps; i++)
	{
		float currentZ = startPoint.z;
		for (UINT j = 0; j < zSteps; j++)
		{
			m_points.push_back({ {currentX, m_middlePoint.y, currentZ},{currentID}});
			currentZ += m_offset;
			currentID++;
			//DEBUG_INFO("| " + std::to_string(currentX) + " " + std::to_string(currentZ) + " | ")
		}
		currentX += m_offset;

		//DEBUG_INFO("\n")
	}

	m_amount = currentID;
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
	desc.ByteWidth = sizeof(point_data) * m_amount;
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
}

void WireFramePass::Create()
{
	if (!CreateShaders())
	{
		DEBUG_ERROR("Creating Shaders for WireFramePass failed!\n")
	}

	CreateInput();
}

void WireFramePass::Prepass()
{
	D3D11Core::Get().Context()->PSSetShader(m_pixelShader.Get(), NULL, 0);
	D3D11Core::Get().Context()->VSSetShader(m_vertexShader.Get(), NULL, 0);
	D3D11Core::Get().Context()->IASetInputLayout(m_inputLayout);

	const UINT stride = sizeof point_data;
	const UINT offset = 0;
	D3D11Core::Get().Context()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	//D3D11Core::Get().Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	D3D11Core::Get().Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void WireFramePass::Pass(Scene* currentScene)
{
	D3D11Core::Get().Context()->Draw(m_amount, 0);
}

void WireFramePass::Postpass()
{
}
