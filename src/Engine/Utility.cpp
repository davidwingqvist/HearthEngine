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
    const float new_x = (((pos.x + 1) * (D3D11Core::Get().GetWindow()->GetWidth())) / (2));
    const float new_y = D3D11Core::Get().GetWindow()->GetHeight() - (((pos.y + 1) * (D3D11Core::Get().GetWindow()->GetHeight())) / (2));

    return { new_x, new_y };
}

sm::Vector3 utility::ScreenRayToWorld(const sm::Vector2& screenPos, Camera* cam)
{
    // Conversion to NDC space
    float clipX = (screenPos.x / D3D11Core::Get().GetWindow()->GetWidth()) * 2 - 1;
    float clipY = 1 - (screenPos.y / D3D11Core::Get().GetWindow()->GetHeight()) * 2;

    // Inverse step towards World Space
    sm::Vector3 ray_position = { clipX, clipY, 0.0f };
    ray_position = sm::Vector3::Transform(ray_position, cam->GetData().projectionMatrix.Invert());
    ray_position = sm::Vector3::Transform(ray_position, cam->GetData().viewMatrix.Invert());

    // Make into a ray.
    sm::Vector3 ray_dir = ray_position - sm::Vector3(cam->GetData().position.x, cam->GetData().position.y, cam->GetData().position.z);
    ray_dir.Normalize();

    return ray_dir;
}

float utility::ConvertToDegrees(const float& radiant)
{
    return radiant * 180.0f / 3.14159265f;
}

float utility::ConvertToRadiant(const float& degrees)
{
    return degrees / 180.0f * 3.14159265f;
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

bool utility::IsBoxColliding(const CollisionBox& a, const CollisionBox& b)
{
    return  a.max.x > b.min.x && 
            a.min.x < b.max.x &&
            a.max.y > b.min.y &&
            a.min.y < b.max.y &&
            a.max.z < b.min.z &&
            a.min.z > b.max.z;
}

sm::Vector3 utility::Reflect(const sm::Vector3& d, const sm::Vector3& n)
{
    return d - (((2*d.Dot(n)) / n.Length()) * n);
}
