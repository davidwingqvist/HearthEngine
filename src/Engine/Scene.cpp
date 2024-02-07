#include "Header.h"
#include "Scene.h"
#include "D3D11Context.h"
#include "ResourceManager.h"
#include "EngineGUI.h"
#include "LuaState.h"
#include "Time.h"
#include "Texture.h"


/*

	HOW TO TRANSFER DATA FROM ONE STRUCT TO ANOTHER
	WITH THE USE OF CHAR POINTER

*/

//char* testcpy = new char[sizeof Transform];

//memcpy(testcpy, transf2, sizeof Transform);

//Transform testdest{};

//memcpy(&testdest, testcpy, sizeof Transform);

//delete[] testcpy;

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
	m_registry.RegisterComponent<GameObject>();
	m_registry.RegisterComponent<Transform>();
	m_registry.RegisterComponent<Script>();
	m_registry.RegisterComponent<Light>();
	m_registry.RegisterComponent<RigidBody>();

	m_registry.RegisterDataToState(GameObject());
	m_registry.RegisterDataToState(Transform());
	m_registry.RegisterDataToState(Light());
	m_registry.RegisterDataToState(RigidBody());

	// Temporary testing entities...

	recs::Entity entity = m_registry.CreateEntity();
	Transform* transf = m_registry.AddComponent<Transform>(entity);
	GameObject* gameObject = m_registry.AddComponent<GameObject>(entity);
	std::strcpy(gameObject->name, "Trunk");

	Model* model = m_registry.AddComponent<Model>(entity);
	model->model_data = ResourceManager::Get().GetResource<Model3D>("Chest.obj").get();
	model->model_texture = ResourceManager::Get().GetResource<Texture>("Chest_Albedo.png").get();
	Script* scr = m_registry.AddComponent<Script>(entity);

	recs::Entity entity2 = m_registry.CreateEntity();
	recs::Entity entity3 = m_registry.CreateEntity();

	m_registry.AddComponent<GameObject>(entity2);
	m_registry.AddComponent<GameObject>(entity3);

	model = m_registry.AddComponent<Model>(entity2);
	model->model_data = ResourceManager::Get().GetResource<Model3D>("throwingknife.obj").get();
	
	model = m_registry.AddComponent<Model>(entity3);
	model->model_data = ResourceManager::Get().GetResource<Model3D>("doublebarrelshotugn.obj").get();

	Transform* transf2 = m_registry.AddComponent<Transform>(entity2);
	transf = m_registry.AddComponent<Transform>(entity3);

	LUA.m_currentRegistry = &m_registry;

	recs::Entity lightEnt = m_registry.CreateEntity();
	std::strcpy(m_registry.AddComponent<GameObject>(lightEnt)->name, "Light");
	transf = m_registry.AddComponent<Transform>(lightEnt);
	Light* light = m_registry.AddComponent<Light>(lightEnt);

	pushTransform(LUA.State(), transf);
	lua_setglobal(LUA.State(), "Transform");
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
			if(model.model_data)
				model.model_data->Draw();
		}

		});

	m_drawManager.Draw();

	
}

recs::recs_registry& Scene::GetRegistry()
{
	return m_registry;
}
