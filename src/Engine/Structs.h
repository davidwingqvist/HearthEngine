#pragma once

class Model3D;
class Texture;

enum class LIGHTTYPE : UINT
{
	DIRECTIONAL = 1U,
	POINTLIGHT = 2U,

	NONE = 0U
};

enum class ACTIVITY : UINT
{
	ACTIVE = 0b1,
	INACTIVE = 0b10,

	NONE = 0b0
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
	// Maximum 28 characters
	char name[28] = "Object";
	ACTIVITY activity = ACTIVITY::ACTIVE;
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
};

sm::Matrix GetMatrix(const Transform& transform);

static int transform_get(lua_State* L);
static int transform_set(lua_State* L);

void pushTransform(lua_State* L);

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
	sm::Vector4 position;
};

struct Ray
{
	sm::Vector3 origin;
	sm::Vector3 dir;
};

struct CameraPoint 
{
	sm::Vector3 target;
	int id = 0;
	bool currentActive = false;
};

struct RigidBody
{
	float mass = 0;
	bool hasCollision = false;
	bool hasGravity = false;
};

/*

	This component is used for easily creating different kinds of lights.
*/
struct Light
{
	sm::Vector4 ambient = {};
	sm::Vector4 diffuse = {};
	sm::Vector4 specular = {};
	sm::Vector3 data = {}; // Type specific data.
	LIGHTTYPE type = LIGHTTYPE::DIRECTIONAL; // Type
};

constexpr int MAX_SCRIPTS = 8;
constexpr int NULL_SCRIPT = 0;
struct Script
{
	size_t script_id[MAX_SCRIPTS] = {};
};

struct ModelID
{
	size_t model_id = 0;
	size_t texture_id = 0;
};

struct Model
{
	Model3D* model_data;
	Texture* model_texture;
	bool isVisible = true;
};

struct InternalBox
{
	sm::Vector3 min = {};
	sm::Vector3 max = {};
};

struct CollisionBox
{
	sm::Vector3 min = {};
	sm::Vector3 max = {};
	float width = 1;
	float height = 1;
	float length = 1;
	bool isActive = true;
};

struct CollisionSphere
{
	float radius = 1.0f;
};

/*

	DEBUG STRUCTS

*/
enum class ConsoleLogEventType : UINT
{
	ALL = 0b111, // DO NOT USE!!!


	LOG_ERROR = 0b1,
	LOG_INFO = 0b10,
	LOG_SUCCESS = 0b100
};

struct ConsoleLogEvent
{
	ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	ConsoleLogEventType type = ConsoleLogEventType::LOG_INFO;
	std::string data;
};
