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

	// Convert radiant to degrees.
	float ConvertToDegrees(const float& radiant);

	// Calculate the angle between two vectors and return the angle in degrees.
	float CalcDegAngle(const sm::Vector2& a, const sm::Vector2& b);
	float CalcDegAngle(const sm::Vector3& a, const sm::Vector3& b);
}