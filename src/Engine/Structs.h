#pragma once

class Model3D;

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
static const char* const tmap_get[] = { "position", "rotation", "scale"};
static const char* const tmap_set[] = { "xPosition", "yPosition", "zPosition", "xRotation", "yRotation", "zRotation", "xScale", "yScale", "zScale" };
struct Transform
{
	sm::Vector3 pos;
	sm::Vector3 rotation;
	sm::Vector3 scale;

	sm::Matrix GetMatrix()
	{
		return sm::Matrix::CreateWorld(pos, pos.Forward, sm::Vector3::Up);
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
};

struct Camera_data
{
	sm::Matrix viewMatrix;
	sm::Matrix projectionMatrix;
};

struct Script
{
	std::vector<std::string> scripts;
};

struct model
{
	Model3D* data;
};
