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
#include "InputManager.h"

void Scene::AssignEdit(InternalScene* scene)
{
	LUA.m_currentRegistry = scene->GetRegistry();
	m_sceneReg = scene->GetRegistry();
	//EngineGUI::Get().m_editCamera = &m_camera;
	m_camera.SetRegistry(m_sceneReg);

	EngineGUI::Get().GetDragPointers()->SetCamera(&m_camera);
	EngineGUI::Get().GetDragPointers()->SetRegistry(scene->GetRegistry());
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

	EngineGUI::Get().m_editCamera = &m_camera;
}

Scene::~Scene()
{
}

void Scene::Assign()
{
	m_camera.Activate();
	LUA.m_currentRegistry = &m_registry;
}

void Scene::Update()
{
	if (m_sceneReg)
	{
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

		m_sceneReg->Group<Transform, InternalBox>().ForEach([&](Transform& transform, InternalBox& coll) {

			coll.min = transform.pos;
			coll.min.x -= 2.0f * transform.scale.x;
			coll.min.z += 2.0f * transform.scale.z;
			coll.min.y -= 2.0f * transform.scale.y;

			coll.max = transform.pos;
			coll.max.x += 2.0f * transform.scale.x;
			coll.max.z -= 2.0f * transform.scale.z;
			coll.max.y += 2.0f * transform.scale.y;


			});


		// Left click, if object is in path of mouse ray, select object.
		if(InputManager::Get().CheckMouseKey(MouseKey::LEFT))
		{
			const Ray r = { m_camera.GetPosition(),
				utility::ScreenRayToWorld(
				{(float)InputManager::Get().GetMouseX(), 
				(float)InputManager::Get().GetMouseY()}, &m_camera),
				};


			// Hit the closest object to the camera.
			float dist = FLT_MAX;
			m_sceneReg->View<InternalBox>().ForEach(
				[&](const recs::Entity entity, InternalBox& colbox)
				{

					float hitDist = FLT_MAX;

				if (utility::RayAABBCollision(colbox, r, hitDist))
				{
					if (hitDist < dist)
					{
						dist = hitDist;
						EngineGUI::SetActiveEntity(entity);
					}

					return;
				}

				});
		}

	}

	if (InputManager::Get().CheckMouseKey(MouseKey::RIGHT, key_state::HOLD))
		m_camera.MoveWithMouse();
	else if (InputManager::Get().CheckMouseKey(MouseKey::MIDDLE))
		m_camera.ToggleLock();
	else
	{
		m_camera.MoveAroundLockedPosition();
		m_camera.MoveWithScrollWheel();
		m_camera.ResetValues();
	}


	if (InputManager::Get().CheckKey(dx::Keyboard::Escape))
		EngineGUI::SetActiveEntity(recs::NULL_ENTITY);


	m_camera.Move();
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

		m_sceneReg->Group<Model, Transform>().ForEach([&](Model& model, Transform& transform) {

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

	
}

recs::recs_registry* Scene::GetRegistry()
{
	return m_sceneReg;
}
