#include "Header.h"
#include "Scene.h"
#include "D3D11Context.h"
#include "ResourceManager.h"
#include "EngineGUI.h"
#include "LuaState.h"
#include "Time.h"
#include "Texture.h"

void UpdatePublicBuffer(ID3D11Buffer** buffer, const sm::Matrix& matrix_data)
{
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT hr = D3D11Core::Get().Context()->Map(*buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to map public buffer for scene!\n");
	}
	std::memcpy(sub.pData, &matrix_data, sizeof(float) * 16);
	D3D11Core::Get().Context()->Unmap(*buffer, 0);

}

Scene::Scene()
{
	m_publicBuffer = nullptr;
	if (!this->CreatePublicBuffer())
	{
		DEBUG_ERROR("Creating public buffer failed for scene!\n")
	}
	this->SetupComponents();
	this->RegisterComponentsToLua();
	m_camera.Activate();
}

Scene::~Scene()
{
}

void Scene::Update()
{
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

	return !FAILED(D3D11Core::Get().Device()->CreateBuffer(&desc, NULL, m_publicBuffer.GetAddressOf()));
}

void Scene::PreDraw()
{

}

void Scene::SetupComponents()
{
	m_registry.RegisterComponent<Script>();
	m_registry.RegisterComponent<Model>();

	m_registry.RegisterDataToState(GameObject());
	m_registry.RegisterDataToState(Transform());
	m_registry.RegisterDataToState(Light());
	m_registry.RegisterDataToState(RigidBody());
	m_registry.RegisterDataToState(ModelID());

	m_registry.RegisterOnCreate<ModelID>([&](const recs::Entity& entity, ModelID& id)
		{
			if ((!m_registry.HasComponent<Model>(entity) && ((id.model_id != 0) || (id.texture_id != 0))))
			{
				Model* model = m_registry.AddComponent<Model>(entity);

				if (id.model_id != 0)
				{
					model->model_data = ResourceManager::Get().GetResource<Model3D>(id.model_id).get();
				}
				if (id.texture_id != 0)
				{
					model->model_texture = ResourceManager::Get().GetResource<Texture>(id.texture_id).get();
				}
			}
		});

	ResourceManager::Get().GetResource<Model3D>("Chest.obj");
	ResourceManager::Get().GetResource<Texture>("Chest_Albedo.png");
	ResourceManager::Get().GetResource<Model3D>("throwingknife.obj");
	ResourceManager::Get().GetResource<Model3D>("doublebarrelshotugn.obj");

	//pushTransform(LUA.State(), transf);
	//lua_setglobal(LUA.State(), "Transform");
}

void Scene::RegisterComponentsToLua()
{

}

void Scene::Draw()
{
	// Public buffer is set to the first slot in Vertex Shader
	D3D11Core::Get().Context()->VSSetConstantBuffers(0, 1, m_publicBuffer.GetAddressOf());
	m_camera.Move();

	m_registry.Group<Model, Transform>().ForEach([&](Model& model, Transform& transform){

		if (model.isVisible)
		{
			UpdatePublicBuffer(m_publicBuffer.GetAddressOf(), GetMatrix(transform));

			if(model.model_texture)
				model.model_texture->SetAsTexture();

			// draw each model.
			if (model.model_data)
				model.model_data->Draw();
		}

		});

	m_drawManager.Draw();

	
}

recs::recs_registry& Scene::GetRegistry()
{
	return m_registry;
}
