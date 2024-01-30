#pragma once

class Camera;

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
}