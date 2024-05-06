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

void GameScene::Update()
{
	m_registry.Update();


	m_registry.Group<RigidBody, Transform>().ForEach([&](const recs::Entity& e, RigidBody& rb, Transform& transform) {

		if (rb.hasGravity)
		{
			transform.pos.y -= GRAVITY * Time::Get().GetDeltaTime();
		}

		});

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
				// Get scripts...
				Script* scriptOne = m_registry.GetComponent<Script>(ent1);
				Script* scriptTwo = m_registry.GetComponent<Script>(ent2);

				if (scriptOne)
				{
					// Go through each possible script, and launch the OnCollision...
					for (auto& id : scriptOne->script_id)
					{
						if (id != NULL_SCRIPT)
						{
							LUA_GAME.CollisionObjectFromScript(id, ent1);
						}
					}
				}
				if (scriptTwo)
				{
					for (auto& id : scriptTwo->script_id)
					{
						if (id != NULL_SCRIPT)
						{
							LUA_GAME.CollisionObjectFromScript(id, ent2);
						}
					}
				}
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

		m_registry.Group<Model, Transform>().ForEach([&](Model& model, Transform& transform) {

			if (model.isVisible)
			{
				utility::UpdatePublicBuffer(m_publicBuffer.GetAddressOf(), GetMatrix(transform));

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
