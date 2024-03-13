#include "Header.h"
#include "Scene.h"
#include "D3D11Context.h"
#include "ResourceManager.h"
#include "EngineGUI.h"
#include "LuaState.h"
#include "Time.h"
#include "Texture.h"
#include "Utility.h"
#include "InputManager.h"
#include "LuaGameState.h"

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
	:InternalScene()
{
	m_publicBuffer = nullptr;
	if (!this->CreatePublicBuffer())
	{
		DEBUG_ERROR("Creating public buffer failed for scene!\n")
	}
	this->SetupComponents();
	this->RegisterComponentsToLua();

	sm::Vector3 d = { 5.0f, -10.0f, 0.0f };
	sm::Vector3 n = { 0.0f, 1.0f, 0.0f };

	sm::Vector3 r = utility::Reflect(d, n);

}

Scene::~Scene()
{
}

void Scene::Update()
{
	//m_registry.Update();

	//m_registry.Group<RigidBody, Transform>().ForEach([&](const recs::Entity& e, RigidBody& rb, Transform& transform) {

	//	if (rb.hasGravity)
	//	{
	//		transform.pos.y -= GRAVITY * Time::Get().GetDeltaTime();
	//	}

	//	});
}

void Scene::Awake()
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
	EngineGUI::Get().RenderGUI();
}

void Scene::PostDraw()
{
	EngineGUI::Get().CommitGUI();
}

//void Scene::SetupComponents()
//{
//	m_registry.RegisterComponent<Model>();
//
//	m_registry.RegisterDataToState(GameObject());
//	m_registry.RegisterDataToState(Transform());
//	m_registry.RegisterDataToState(Light());
//	m_registry.RegisterDataToState(RigidBody());
//	m_registry.RegisterDataToState(ModelID());
//	m_registry.RegisterDataToState(Script());
//
//	m_registry.RegisterOnCreate<ModelID>([&](const recs::Entity& entity, ModelID& id)
//		{
//			if ((!m_registry.HasComponent<Model>(entity) && ((id.model_id != 0) || (id.texture_id != 0))))
//			{
//				Model* model = m_registry.AddComponent<Model>(entity);
//
//				if (id.model_id != 0)
//				{
//					model->model_data = ResourceManager::Get().GetResource<Model3D>(id.model_id).get();
//				}
//				if (id.texture_id != 0)
//				{
//					model->model_texture = ResourceManager::Get().GetResource<Texture>(id.texture_id).get();
//				}
//			}
//		});
//
//	m_registry.RegisterOnCreate<Script>([&](const recs::Entity& entity, Script& script) {
//
//		LUA_GAME.SetCurrentEntity(entity);
//		for (int i = 0; i < 5; i++)
//		{
//			LUA_GAME.CreateObjectFromScript(script.script_id[i], entity);
//			LUA_GAME.AwakeObjectFromScript(script.script_id[i], entity);
//		}
//
//		});
//
//	m_registry.RegisterOnUpdate<Script>([&](const recs::Entity& entity, Script& script) {
//			
//		LUA_GAME.SetCurrentEntity(entity);
//		for (int i = 0; i < 5; i++)
//		{
//			LUA_GAME.UpdateObjectFromScript(script.script_id[i], entity);
//		}
//
//		});
//
//	pushTransform(LUA.State());
//}

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
