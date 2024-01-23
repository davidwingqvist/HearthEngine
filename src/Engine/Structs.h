#pragma once

class Model3D;

enum class LIGHTTYPE : UINT
{
	DIRECTIONAL = 1U,
	SPOTLIGHT = 2U,
	POINTLIGHT = 3U,

	NONE = 0U
};

struct object2D
{
	float x;
	float y;
	float width;
	float height;

	object2D()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	object2D(const float& x, const float& y, const float& width, const float& height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	object2D(const std::string& filename)
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
};

struct GameObject
{
	std::string name = "Object";
};

/*
	-----------------------------------
	-------------TRANSFORM-------------
	-----------------------------------
*/


static const char* const tname = "Transform";
static const char* const tmap_set[] = { "xPosition", "yPosition", "zPosition", "xRotation", "yRotation", "zRotation", "xScale", "yScale", "zScale" };
struct Transform
{
	sm::Vector3 pos = {0.0f, 0.0f, 0.0f};
	sm::Vector3 rotation = {0.0f, 0.0f, 0.0f};
	sm::Vector3 scale = {1.0f, 1.0f, 1.0f};

	const sm::Matrix GetMatrix() const
	{
		return sm::Matrix::CreateTranslation(pos) * sm::Matrix::CreateFromYawPitchRoll(rotation) * sm::Matrix::CreateScale(scale);
	}
};
static int transform_get(lua_State* L);
static int transform_set(lua_State* L);

void pushTransform(lua_State* L, Transform* inst);

/*
	-----------------------------------
	-------------TRANSFORM-------------
	-----------------------------------
*/

struct vertex_data
{
	sm::Vector3 position;
	sm::Vector2 texCoords;
	sm::Vector3 normal;
};

struct camera_data
{
	sm::Matrix viewMatrix;
	sm::Matrix projectionMatrix;
};

struct Physics
{
	float mass;
};

/*

	This component is used for easily creating different kinds of lights.
*/
struct Light
{
	sm::Vector3 position = {};
	LIGHTTYPE type = LIGHTTYPE::NONE;
};

struct Script
{
	std::vector<std::string> scripts;
};

struct Model
{
	Model3D* data;
	bool isVisible = true;
};
