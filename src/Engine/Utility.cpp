#include "Header.h"
#include "Utility.h"
#include "Camera.h"
#include "D3D11Context.h"

sm::Vector2 utility::WorldSpaceToScreenSpace(const sm::Vector3& worldPos, Camera* cam)
{
    const sm::Vector4 fixedPos = { worldPos.x, worldPos.y, worldPos.z, 1.0f };
    sm::Vector4 pos = sm::Vector4::Transform(fixedPos, cam->GetData().viewMatrix);
    pos = sm::Vector4::Transform(pos, cam->GetData().projectionMatrix);
    pos.x /= pos.x;
    pos.y /= pos.y;

    // Conversion from NDC space [-1, 1] to Window space
    float new_x = (((pos.x + 1) * (D3D11Core::Get().GetWindow()->GetWidth())) / (2));
    float new_y = D3D11Core::Get().GetWindow()->GetHeight() - (((pos.y + 1) * (D3D11Core::Get().GetWindow()->GetHeight())) / (2));

    return { new_x, new_y };
}

float utility::ConvertToDegrees(const float& radiant)
{
    return radiant * 180.0f / 3.14159265f;
}

float utility::CalcDegAngle(const sm::Vector2& a, const sm::Vector2& b)
{
    float deg = std::acos(a.Dot(b) / (a.Length() * b.Length()));
    return utility::ConvertToDegrees(deg);
}

float utility::CalcDegAngle(const sm::Vector3& a, const sm::Vector3& b)
{
    float deg = std::acos(a.Dot(b) / (a.Length() * b.Length()));
    return utility::ConvertToDegrees(deg);
}

sm::Vector2 utility::Reflect(const sm::Vector2& d, const sm::Vector2& n)
{
    return d - (((2 * d.Dot(n)) / n.Length()) * n);
}

sm::Vector3 utility::Reflect(const sm::Vector3& d, const sm::Vector3& n)
{
    return d - (((2*d.Dot(n)) / n.Length()) * n);
}
