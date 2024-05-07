#include "Header.h"
#include "DragPointer.h"
#include "ResourceManager.h"
#include "D3D11Context.h"
#include "Utility.h"

DragPointer::DragPointer()
{
	m_transforms[0].rotation = { 0.0f, 0.0f, 0.0f };
	m_transforms[1].rotation = { dx::XM_1DIV2PI, 0.0f, 0.0f };
	m_transforms[2].rotation = { 0.0f, 0.0f, dx::XM_1DIV2PI };
}

void DragPointer::Draw(const recs::Entity& currentEntity)
{
	if (m_currentRegistry && currentEntity != recs::NULL_ENTITY)
	{
		const Transform& modTransf = *m_currentRegistry->GetComponent<Transform>(currentEntity);

		m_transforms[0].pos = modTransf.pos;
		m_transforms[1].pos = modTransf.pos;
		m_transforms[2].pos = modTransf.pos;

		utility::UpdatePublicBuffer(m_modelBuffer.GetAddressOf(), 
			GetMatrix(m_transforms[0]));


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
