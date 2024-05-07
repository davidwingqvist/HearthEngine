#include "Header.h"
#include "DragPointer.h"
#include "ResourceManager.h"
#include "D3D11Context.h"
#include "Utility.h"

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
	if (m_currentRegistry && currentEntity != recs::NULL_ENTITY)
	{
		D3D11Core::Get().Context()->VSSetConstantBuffers(0, 1, m_modelBuffer.GetAddressOf());

		const Transform& modTransf = *m_currentRegistry->GetComponent<Transform>(currentEntity);

		m_transforms[0].pos = modTransf.pos;
		m_transforms[0].pos.x += 5.0f;
		m_transforms[1].pos = modTransf.pos;
		m_transforms[1].pos.y += 5.0f;
		m_transforms[2].pos = modTransf.pos;
		m_transforms[2].pos.z += 5.0f;

		utility::UpdatePublicBuffer(m_modelBuffer.GetAddressOf(), 
			GetMatrix(m_transforms[0]));
		m_pointerModel->Draw();

		utility::UpdatePublicBuffer(m_modelBuffer.GetAddressOf(),
			GetMatrix(m_transforms[1]));
		m_pointerModel->Draw();

		utility::UpdatePublicBuffer(m_modelBuffer.GetAddressOf(),
			GetMatrix(m_transforms[2]));
		m_pointerModel->Draw();
	}
}

void DragPointer::SetRegistry(recs::recs_registry* registry)
{
	m_currentRegistry = registry;
}

void DragPointer::StartUp()
{
	m_pointerModel = ResourceManager::Get().GetResource<Model3D>("pointer.obj");
	
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
