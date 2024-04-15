#include "Header.h"
#include "CollisionBoxRenderPass.h"
#include "D3D11Context.h"
#include "Debugger.h"
#include "EngineGUI.h"

bool CollisionBoxRenderPass::CreateIndexBuffer()
{
	UINT indices[] = {

	0, 1,
	1, 2,
	2, 3,
	3, 0,
	3, 4,
	4, 5,
	0, 5,
	6, 1,
	7, 2,
	5, 6,
	6, 7,
	7, 4

	};

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(UINT) * 24;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &indices[0];

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, m_indexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create index buffer for collision box pass\n");
		return false;
	}

	return true;
}

bool CollisionBoxRenderPass::CreateVertexBuffer()
{

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(sm::Vector3) * 8;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &m_points[0];

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Couldnt create vertex buffer for collision box pass\n");
		return false;
	}

	return true;
}

bool CollisionBoxRenderPass::CreateColorBuffer()
{
	D3D11_BUFFER_DESC indDesc{};
	indDesc.ByteWidth = sizeof sm::Vector4;
	indDesc.Usage = D3D11_USAGE_DYNAMIC;
	indDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	indDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA indData{};
	indData.pSysMem = &m_color;

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&indDesc, &indData, m_colorBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed creating Index buffer for Light pass!\n")
			return false;
	}

	return true;
}

bool CollisionBoxRenderPass::CreateInputLayout()
{
	HRESULT hr = S_FALSE;

	// Create m_defaultInputLayout.
	std::string shaderByteCode = m_vertexShader.GetShaderByteCode();
	D3D11_INPUT_ELEMENT_DESC defaultVertexShaderDesc[] =
	{
		{"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                0,                   D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(hr = D3D11Core::Get().Device()->CreateInputLayout(defaultVertexShaderDesc, ARRAYSIZE(defaultVertexShaderDesc), shaderByteCode.c_str(), shaderByteCode.length(), m_inputLayout.GetAddressOf())))
	{
		DEBUG_ERROR("Failed creating input layout for collider box pass\n");
		return false;
	}

	return true;
}

bool CollisionBoxRenderPass::UpdateVertices(const CollisionBox& box)
{
	m_points[0] = box.min;
	m_points[1] = box.min; m_points[1].z -= box.length * 2;
	m_points[2] = m_points[1]; m_points[2].x += box.width * 2;
	m_points[3] = m_points[2]; m_points[3].z += box.length * 2;
	m_points[4] = m_points[3]; m_points[4].y += box.height * 2;
	m_points[5] = m_points[0]; m_points[5].y += box.height * 2;
	m_points[6] = m_points[1]; m_points[6].y += box.height * 2;
	m_points[7] = box.max;

	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT hr = D3D11Core::Get().Context()->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to update vertex buffer data for collision box pass\n");
	}
	std::memcpy(sub.pData, &m_points[0], sizeof(sm::Vector3) * 8);
	D3D11Core::Get().Context()->Unmap(m_vertexBuffer.Get(), 0);

	return !FAILED(hr);
}

void CollisionBoxRenderPass::SetGreenColor()
{
	m_color = { 0.0f, 1.0f, 0.0f, 1.0f };

	UpdateColorBuffer();
}

void CollisionBoxRenderPass::SetRedColor()
{
	m_color = { 1.0f, 0.0f, 0.0f, 1.0f };

	UpdateColorBuffer();
}

bool CollisionBoxRenderPass::UpdateColorBuffer()
{
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT hr = D3D11Core::Get().Context()->Map(m_colorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to update color data for collision box pass\n");
	}
	std::memcpy(sub.pData, &m_color, sizeof(sm::Vector4));
	D3D11Core::Get().Context()->Unmap(m_colorBuffer.Get(), 0);

	return !FAILED(hr);
}

CollisionBoxRenderPass::CollisionBoxRenderPass()
{
}

CollisionBoxRenderPass::~CollisionBoxRenderPass()
{
}

void CollisionBoxRenderPass::Create()
{
	CreateIndexBuffer();
	CreateVertexBuffer();
	m_pixelShader.Create("CollisionBoxPixelShader");
	m_vertexShader.Create("CollisionBoxVertexShader");
	CreateInputLayout();
	CreateColorBuffer();
	EngineGUI::Get().m_toggleCollisionBoxDraw = &m_isActive;
}

void CollisionBoxRenderPass::Prepass()
{
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	DC->VSSetShader(m_vertexShader.Get(), NULL, NULL);
	DC->PSSetShader(m_pixelShader.Get(), NULL, NULL);
	DC->PSSetConstantBuffers(0, 1, m_colorBuffer.GetAddressOf());

	DC->IASetInputLayout(m_inputLayout.Get());

	const UINT strides = sizeof sm::Vector3;
	const UINT offset = 0;

	DC->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &strides, &offset);
	DC->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CollisionBoxRenderPass::Pass(InternalScene* currentScene)
{
	// Draw individual collision box here..

	auto* reg = currentScene->GetRegistry();

	if (reg)
	{
		reg->View<CollisionBox>().ForEach([&](CollisionBox& collbox) {

			// Draw if active.
			if (collbox.isActive)
			{
				SetGreenColor();
			}
			else
				SetRedColor();

			UpdateVertices(collbox);
			DC->DrawIndexed(24, 0, 0);

			});
	}
}

void CollisionBoxRenderPass::Postpass()
{

}
