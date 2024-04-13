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

static void UpdatePublicBuffer(ID3D11Buffer** buffer, const sm::Matrix& matrix_data)
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

void Scene::AssignEdit(InternalScene* scene)
{
	LUA.m_currentRegistry = scene->GetRegistry();
	m_sceneReg = scene->GetRegistry();
}

void Scene::Reset()
{
	m_sceneReg = nullptr;
	LUA.m_currentRegistry = nullptr;
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
	if (m_sceneReg)
	{

		//m_registry.Update();

		//m_registry.Group<RigidBody, Transform>().ForEach([&](const recs::Entity& e, RigidBody& rb, Transform& transform) {

		//	if (rb.hasGravity)
		//	{
		//		transform.pos.y -= GRAVITY * Time::Get().GetDeltaTime();
		//	}

		//	});

		// Update the min and max positions relative to transform pos.
		m_sceneReg->Group<Transform, CollisionBox>().ForEach([&](Transform& transform, CollisionBox& coll) {

			coll.min = transform.pos;
			coll.min.x -= coll.width;
			coll.min.z += coll.length;
			coll.min.y -= coll.height;

			coll.max = transform.pos;
			coll.max.x += coll.width;
			coll.max.z -= coll.length;
			coll.max.y += coll.height;


			});

		//m_sceneReg->View<CollisionBox>().ForEach([&](const recs::Entity& ent1, CollisionBox& coll1) {

		//	

		//	m_sceneReg->View<CollisionBox>().ForEach([&](const recs::Entity& ent2, CollisionBox& coll2) {

		//		if (ent1 != ent2)
		//		{
		//			if (utility::IsBoxColliding(coll1, coll2))
		//			{
		//				// Collision, Do something.
		//			}
		//		}

		//		});

		//	});
	}
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

void Scene::RegisterComponentsToLua()
{

}

void Scene::Draw()
{
	if (m_sceneReg)
	{
		// Public buffer is set to the first slot in Vertex Shader
		D3D11Core::Get().Context()->VSSetConstantBuffers(0, 1, m_publicBuffer.GetAddressOf());
		m_camera.Move();

		m_sceneReg->Group<Model, Transform>().ForEach([&](Model& model, Transform& transform) {

			if (model.isVisible)
			{
				UpdatePublicBuffer(m_publicBuffer.GetAddressOf(), GetMatrix(transform));

				if (model.model_texture)
					model.model_texture->SetAsTexture();

				// draw each model.
				if (model.model_data)
					model.model_data->Draw();
			}

			});

		m_drawManager.Draw();
	}

	
}

recs::recs_registry* Scene::GetRegistry()
{
	return m_sceneReg;
}
