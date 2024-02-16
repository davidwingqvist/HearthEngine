#include "Header.h"
#include "Camera.h"
#include "D3D11Context.h"
#include "Debugger.h"
#include "InputManager.h"
#include "Time.h"

constexpr float limit = dx::XM_PIDIV2 - 0.01f;

void Camera::Update()
{
	m_matData.viewMatrix = sm::Matrix::CreateLookAt(m_position, m_lookAt, sm::Vector3::Up);
	UpdateBuffer();
}

void Camera::UpdateRotation()
{
	// Limiting pitch and yaw to not cause trouble.
	m_pitch = max(-limit, m_pitch);
	m_pitch = std::min(+limit, m_pitch);

	if (m_yaw > dx::XM_PI)
	{
		m_yaw -= dx::XM_2PI;
	}
	else if (m_yaw < -dx::XM_PI)
	{
		m_yaw += dx::XM_2PI;
	}

	m_lookAt = sm::Vector3::Transform(m_position.Forward, sm::Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.f)) + m_position;
	m_right = sm::Vector3::Transform(m_position.Right, sm::Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.f)) + m_position;
	Update();
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
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT hr = D3D11Core::Get().Context()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	if (FAILED(hr))
	{
		DEBUG_ERROR("Failed to map public buffer for scene!\n");
		return false;
	}
	std::memcpy(sub.pData, &m_matData, sizeof camera_data);
	D3D11Core::Get().Context()->Unmap(m_buffer, 0);
	return true;
}

Camera::Camera()
{
	m_position = { 0.0f, 10.0f, .0f };
	m_lookAt = m_position + m_position.Forward;
	m_right = m_position + m_position.Right;
	m_matData.viewMatrix = sm::Matrix::CreateLookAt(m_position, m_lookAt, sm::Vector3::Up);
	m_matData.projectionMatrix = sm::Matrix::CreatePerspectiveFieldOfView(3.1415f / 4.0f, ((float)D3D11Core::Get().GetWindow()->GetWidth() / D3D11Core::Get().GetWindow()->GetHeight()), 0.1f, 5000.f);
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

void Camera::SetPosition(const sm::Vector3& pos)
{
	m_position += pos;
}

void Camera::SetPosition(const float& x, const float& y, const float& z)
{
	sm::Vector3 vec_forward = m_lookAt - m_position;
	sm::Vector3 vec_right = m_right - m_position;
	vec_forward.Normalize();
	vec_right.Normalize();

	// Move forward and backward based on rotation.
	m_position += vec_forward * z * m_speed;
	m_position += vec_right * x * m_speed;

	UpdateRotation();
}

void Camera::Move()
{
	if (InputManager::Get().CheckKey(kb_key::W, key_state::HOLD))
	{
		SetPosition(0, 0, 20.f * Time::Get().GetDeltaTime());
	}
	else if (InputManager::Get().CheckKey(kb_key::S, key_state::HOLD))
	{
		SetPosition(0, 0, -20.f * Time::Get().GetDeltaTime());
	}

	if (InputManager::Get().CheckKey(kb_key::A, key_state::HOLD))
	{
		SetPosition(-20.f * Time::Get().GetDeltaTime(), 0, 0);
	}
	else if (InputManager::Get().CheckKey(kb_key::D, key_state::HOLD))
	{
		SetPosition(20.f * Time::Get().GetDeltaTime(), 0, 0);
	}

	
	if (InputManager::Get().CheckKey(kb_key::Up, key_state::HOLD))
	{
		m_pitch += 1.3f * Time::Get().GetDeltaTime();
		UpdateRotation();
	}
	else if(InputManager::Get().CheckKey(kb_key::Down, key_state::HOLD))
	{
		m_pitch -= 1.3f * Time::Get().GetDeltaTime();
		UpdateRotation();
	}

	if (InputManager::Get().CheckKey(kb_key::Left, key_state::HOLD))
	{
		m_yaw += 1.3f * Time::Get().GetDeltaTime();
		UpdateRotation();
	}
	else if (InputManager::Get().CheckKey(kb_key::Right, key_state::HOLD))
	{
		m_yaw -= 1.3f * Time::Get().GetDeltaTime();
		UpdateRotation();
	}

	if (InputManager::Get().CheckKey(kb_key::LeftShift, key_state::HOLD))
	{
		m_speed = 5.0f;
	}
	else
		m_speed = 2.0f;
}

void Camera::Activate() const
{
	D3D11Core::Get().Context()->VSSetConstantBuffers(1, 1, &m_buffer);
}

const camera_data& Camera::GetData() const
{
	return m_matData;
}
