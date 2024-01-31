#include "Header.h"
#include "Scene.h"
#include "D3D11Context.h"
#include "ResourceManager.h"
#include "EngineGUI.h"
#include "LuaState.h"
#include "Time.h"
#include "Texture.h"

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
	m_registry.RegisterComponent<GameObject>();
	m_registry.RegisterComponent<Transform>();
	m_registry.RegisterComponent<Script>();

	// Temporary testing entities...

	recs::Entity entity = m_registry.CreateEntity();
	Transform* transf = m_registry.AddComponent<Transform>(entity);
	GameObject* gameObject = m_registry.AddComponent<GameObject>(entity);
	gameObject->name = "Trunk";
	
	Model* model = m_registry.AddComponent<Model>(entity);
	model->model_data = ResourceManager::Get().GetResource<Model3D>("Chest.obj").get();
	model->model_texture = ResourceManager::Get().GetResource<Texture>("Chest_Albedo.png").get();
	Script* scr = m_registry.AddComponent<Script>(entity);
	transf->pos = { 0, 0, -225.5 };

	recs::Entity entity2 = m_registry.CreateEntity();
	recs::Entity entity3 = m_registry.CreateEntity();

	Transform* transf2 = m_registry.AddComponent<Transform>(entity2);
	transf = m_registry.AddComponent<Transform>(entity3);

	LUA.m_currentRegistry = &m_registry;

	pushTransform(LUA.State(), transf);
	lua_setglobal(LUA.State(), "Transform");
	m_camera.Activate();
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
	// Public buffer is set to the first slot in Vertex Shader
	D3D11Core::Get().Context()->VSSetConstantBuffers(0, 1, &m_publicBuffer);
	m_camera.Move();

	m_registry.Group<Model, Transform>().ForEach([&](Model& model, Transform& transform){

		if (model.isVisible)
		{
			UpdatePublicBuffer(m_publicBuffer, GetMatrix(transform));

			model.model_texture->SetAsTexture();
			// draw each model.
			model.model_data->Draw();
		}

		});

	//m_drawManager.Draw();

	
}

recs::recs_registry& Scene::GetRegistry()
{
	return m_registry;
}
