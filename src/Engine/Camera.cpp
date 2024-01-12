#include "Header.h"
#include "Camera.h"
#include "D3D11Context.h"
#include "Debugger.h"

void Camera::Update()
{
	m_matData.viewMatrix = sm::Matrix::CreateLookAt(m_position, m_position + m_position.Forward, sm::Vector3::Up);
	UpdateBuffer();
}

bool Camera::SetUpBuffer()
{
	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof camera_data;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = &m_matData;

	return !FAILED(D3D11Core::Get().Device()->CreateBuffer(&desc, &data, &m_buffer));
}

bool Camera::UpdateBuffer()
{
	return false;
}

Camera::Camera()
{
	m_position = { 0.0f, 0.0f, -1.0f };
	m_matData.viewMatrix = sm::Matrix::CreateLookAt(m_position, m_position + m_position.Forward, sm::Vector3::Up);
	m_matData.projectionMatrix = sm::Matrix::CreatePerspectiveFieldOfView(3.1415 / 4.0f, ((float)D3D11Core::Get().GetWindow()->GetWidth() / D3D11Core::Get().GetWindow()->GetHeight()), 0.1f, 1000.f);
	if (!SetUpBuffer())
	{
		DEBUG_ERROR("Failed to setup buffer for Camera!\n")
	}
}

Camera::~Camera()
{
	if (m_buffer)
		m_buffer->Release();
}

void Camera::SetPosition(sm::Vector3 pos)
{
	m_position = pos;
	Update();
}

void Camera::Activate()
{
	D3D11Core::Get().Context()->VSSetConstantBuffers(1, 1, &m_buffer);
}
