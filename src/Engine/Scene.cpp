#include "Header.h"
#include "Scene.h"
#include "D3D11Context.h"
#include "ResourceManager.h"

void UpdatePublicBuffer(ID3D11Buffer*& buffer, const sm::Matrix& matrix_data)
{
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT hr = D3D11Core::Get().Context()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to map public buffer for scene!\n");
	}
	std::memcpy(sub.pData, &matrix_data, sizeof(float) * 16);
	D3D11Core::Get().Context()->Unmap(buffer, 0);

}

Scene::Scene()
{
	m_publicBuffer = nullptr;
	if (!this->CreatePublicBuffer())
	{
		DEBUG_ERROR("Creating public buffer failed for scene!\n")
	}


	recs::Entity entity = m_registry.CreateEntity();
	transform* transf = m_registry.AddComponent<transform>(entity);
	m_registry.AddComponent<model>(entity)->data = ResourceManager::Get().GetResource<Model3D>("Tree1.obj").get();

	transf->pos = { 5, 3.5, 25.5 };
	//UpdatePublicBuffer(m_publicBuffer, transf->GetMatrix());
}

Scene::~Scene()
{
	if (m_publicBuffer != nullptr)
		m_publicBuffer->Release();
}

bool Scene::CreatePublicBuffer()
{
	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(float) * 16;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	return !FAILED(D3D11Core::Get().Device()->CreateBuffer(&desc, NULL, &m_publicBuffer));
}

void Scene::SetLogic(std::function<void(recs::recs_registry&)> function)
{
	m_function = function;
}

void Scene::Update()
{
	if (m_function)
		m_function(m_registry);
}

void Scene::PreDraw()
{

}

void Scene::Draw()
{
	//this->PreDraw();
	// Public buffer is set to the first slot in Vertex Shader
	D3D11Core::Get().Context()->VSSetConstantBuffers(0, 1, &m_publicBuffer);

	m_registry.Group<model, transform>().ForEach([&](model& model, transform& pos){


		UpdatePublicBuffer(m_publicBuffer, pos.GetMatrix());

		// draw each model.
		model.data->Draw();

		});
	m_drawManager.Draw();
}
