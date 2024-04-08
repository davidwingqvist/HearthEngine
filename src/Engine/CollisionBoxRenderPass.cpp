#include "Header.h"
#include "CollisionBoxRenderPass.h"
#include "D3D11Context.h"
#include "Debugger.h"

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
	6, 2,
	7, 3,
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

	HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf());
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

bool CollisionBoxRenderPass::UpdateVertices(const CollisionBox& box)
{
	m_points[0] = box.min;
	m_points[1] = box.min; m_points[1].z -= box.length;
	m_points[2] = m_points[1]; m_points[2].x += box.width;
	m_points[3] = m_points[2]; m_points[3].z += box.length;
	m_points[4] = m_points[3]; m_points[4].y += box.height;
	m_points[5] = m_points[0]; m_points[5].y += box.height;
	m_points[6] = m_points[2]; m_points[6].y += box.height;
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
}

void CollisionBoxRenderPass::Prepass()
{
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void CollisionBoxRenderPass::Pass(InternalScene* currentScene)
{
	// Draw individual collision box here..

	//auto* reg = currentScene->GetRegistry();

	//if (reg)
	//{
	//	reg->View<CollisionBox>().ForEach([&](CollisionBox& collbox) {

	//		UpdateVertices(collbox);

	//		});
	//}
}

void CollisionBoxRenderPass::Postpass()
{

}
