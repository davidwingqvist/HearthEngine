#include "Header.h"
#include "DragPointer.h"
#include "ResourceManager.h"
#include "D3D11Context.h"
#include "Utility.h"
#include "Texture.h"
#include "InputManager.h"
#include "Camera.h"
#include "Time.h"

DragPointer::DragPointer()
{
	m_transforms[0].rotation = { 0.0f, 0.0f, 0.0f };
	m_transforms[1].rotation = { 0.0f, 0.0f, dx::XM_PIDIV2 };
	m_transforms[2].rotation = { 0.0f, -dx::XM_PIDIV2, 0.0f};

	sm::Vector3 scale = { 10.0f, 5.0f, 5.0f };

	m_transforms[0].scale = scale;
	m_transforms[1].scale = scale;
	m_transforms[2].scale = scale;
}

void DragPointer::Draw(const recs::Entity& currentEntity)
{
	m_currMouseX = InputManager::Get().GetMouseX();
	m_currMouseY = InputManager::Get().GetMouseY();


	if (m_currentRegistry && currentEntity != recs::NULL_ENTITY
		&& m_camera != nullptr)
	{
		D3D11Core::Get().Context()->VSSetConstantBuffers(0, 1, m_modelBuffer.GetAddressOf());

		Transform& modTransf = *m_currentRegistry->GetComponent<Transform>(currentEntity);

		// Move pointers to their spots, related to the object position.
		m_transforms[0].pos = modTransf.pos;
		m_transforms[0].pos.x += 5.0f;
		m_transforms[1].pos = modTransf.pos;
		m_transforms[1].pos.y += 5.0f;
		m_transforms[2].pos = modTransf.pos;
		m_transforms[2].pos.z += 5.0f;

		// Move hitboxes to cover the pointers.
		m_hitBoxes[0].min = modTransf.pos;
		m_hitBoxes[1].min = modTransf.pos;
		m_hitBoxes[2].min = modTransf.pos;

		m_hitBoxes[0].max = { modTransf.pos.x + 20.0f, modTransf.pos.y + 2.5f, modTransf.pos.z - 2.5f };
		m_hitBoxes[1].max = { modTransf.pos.x + 2.5f, modTransf.pos.y + 20.0f, modTransf.pos.z - 2.5f};
		m_hitBoxes[2].max = { modTransf.pos.x + 2.5f, modTransf.pos.y + 2.5f, modTransf.pos.z + 20.0f};


		// Draw pointers...
		utility::UpdatePublicBuffer(m_modelBuffer.GetAddressOf(), 
			GetMatrix(m_transforms[0]));
		m_pointerTextures[0]->SetAsTexture();
		m_pointerModel->Draw();

		utility::UpdatePublicBuffer(m_modelBuffer.GetAddressOf(),
			GetMatrix(m_transforms[1]));
		m_pointerTextures[1]->SetAsTexture();
		m_pointerModel->Draw();

		utility::UpdatePublicBuffer(m_modelBuffer.GetAddressOf(),
			GetMatrix(m_transforms[2]));
		m_pointerTextures[2]->SetAsTexture();
		m_pointerModel->Draw();

		const Ray r = { m_camera->GetPosition(),
			utility::ScreenRayToWorld(
			{(float)InputManager::Get().GetMouseX(),
			(float)InputManager::Get().GetMouseY()}, m_camera),
		};

		if (InputManager::Get().CheckMouseKey(MouseKey::LEFT, key_state::HOLD))
		{
			if (utility::RayAABBCollision(m_hitBoxes[0], r))
			{
				modTransf.pos.x += Time::Get().GetDeltaTime();
			}
			else if (utility::RayAABBCollision(m_hitBoxes[1], r))
			{
				modTransf.pos.y += Time::Get().GetDeltaTime();
			}
			else if (utility::RayAABBCollision(m_hitBoxes[2], r))
			{
				modTransf.pos.z += Time::Get().GetDeltaTime();
			}
		}

	}


	m_prevMouseX = m_currMouseX;
	m_prevMouseY = m_currMouseY;
}

void DragPointer::SetRegistry(recs::recs_registry* registry)
{
	m_currentRegistry = registry;
}

void DragPointer::SetCamera(Camera* camera)
{
	m_camera = camera;
}

void DragPointer::StartUp()
{
	m_pointerModel = ResourceManager::Get().GetResource<Model3D>("pointer.obj");
	
	m_pointerTextures[0] = ResourceManager::Get().GetResource<Texture>("red.png");
	m_pointerTextures[1] = ResourceManager::Get().GetResource<Texture>("green.png");
	m_pointerTextures[2] = ResourceManager::Get().GetResource<Texture>("blue.png");

	CreateModelBuffer();
}

bool DragPointer::CreateModelBuffer()
{
	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(float) * 16;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	return !FAILED(D3D11Core::Get().Device()->CreateBuffer(&desc, NULL, m_modelBuffer.GetAddressOf()));
}
