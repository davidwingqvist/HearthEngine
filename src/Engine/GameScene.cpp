#include "Header.h"
#include "GameScene.h"
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

void GameScene::Update()
{
	m_registry.Update();

	m_registry.Group<Transform, CollisionBox>().ForEach([&](Transform& transform, CollisionBox& coll) {

		coll.min = transform.pos;
		coll.min.x -= coll.width;
		coll.min.z += coll.length;
		coll.min.y -= coll.height;

		coll.max = transform.pos;
		coll.max.x += coll.width;
		coll.max.z -= coll.length;
		coll.max.y += coll.height;


		});

	m_registry.View<CollisionBox>().ForEach([&](const recs::Entity& ent1, CollisionBox& coll1) {

	

		m_registry.View<CollisionBox>().ForEach([&](const recs::Entity& ent2, CollisionBox& coll2) {

		if (ent1 != ent2)
		{
			if (utility::IsBoxColliding(coll1, coll2))
			{
				// Collision, Do something.
			}
		}

		});

	});
}

void GameScene::Awake()
{
	m_registry.View<Script>().ForEach([&](const recs::Entity& entity, Script& script) {

		LUA_GAME.SetCurrentEntity(entity);
		for (int i = 0; i < 5; i++)
		{
			LUA_GAME.AwakeObjectFromScript(script.script_id[i], entity);
		}

		});

	m_gameCamera.Activate();
}

void GameScene::Draw()
{
		// Public buffer is set to the first slot in Vertex Shader
		D3D11Core::Get().Context()->VSSetConstantBuffers(0, 1, m_publicBuffer.GetAddressOf());
		m_camera.Move();

		m_registry.Group<Model, Transform>().ForEach([&](Model& model, Transform& transform) {

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

void GameScene::PreDraw()
{
}

void GameScene::PostDraw()
{
}

void GameScene::SetCameraPoint(Transform* t, CameraPoint* p)
{
	if (t && p)
	{
		m_gameCamera.UpdateInfo(t, p);
		m_transform = t;
		m_cameraPoint = p;
	}
}

void GameScene::UpdateCamera()
{
	if (m_transform && m_cameraPoint)
	{
		m_gameCamera.UpdateInfo(m_transform, m_cameraPoint);
	}
}

GameScene::GameScene()
	:InternalScene()
{
	this->SetupComponents();
	this->CreatePublicBuffer();
}

bool GameScene::CreatePublicBuffer()
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
