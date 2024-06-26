#pragma once

class Camera;

constexpr float GRAVITY = 9.82f;

/*
	Various utility functions that can be used for
	easier calculations.
*/
namespace utility
{
	template<typename T>
	T Lerp(const T& a, const T& b, const float& t)
	{
		return  a + t * (b - a);
	}

	/*
		Convert a world space position onto a position on the screen space.
	*/
	sm::Vector2 WorldSpaceToScreenSpace(const sm::Vector3& worldPos, Camera* cam);

	/*
		Convert a screen space position as ray to world position.
	*/
	sm::Vector3 ScreenRayToWorld(const sm::Vector2& screenPos, Camera* cam);

	// Convert radiant to degrees.
	float ConvertToDegrees(const float& radiant);

	// Convert degrees to radiant.
	float ConvertToRadiant(const float& degrees);

	// Calculate the angle between two vectors and return the angle in degrees.
	float CalcDegAngle(const sm::Vector2& a, const sm::Vector2& b);
	float CalcDegAngle(const sm::Vector3& a, const sm::Vector3& b);

	// Calculate the reflect vector of (d) vector and (n) normal vector.
	sm::Vector3 Reflect(const sm::Vector3& d, const sm::Vector3& n);
	sm::Vector2 Reflect(const sm::Vector2& d, const sm::Vector2& n);

	// Checks if two AABBs are colliding with each other.
	bool IsBoxColliding(const CollisionBox& a, const CollisionBox& b);

	bool RayAABBCollision(const CollisionBox& b, const Ray& r);
	bool RayAABBCollision(const InternalBox& b, const Ray& r, float& t);
	bool RayAABBCollision(const InternalBox& b, const Ray& r);

	// Update a public buffer used for rendering objects.
	void UpdatePublicBuffer(ID3D11Buffer** buffer, const sm::Matrix& matrix_data);
}