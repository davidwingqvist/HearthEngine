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
		return sm::Matrix::CreateWorld(pos, pos.Forward, sm::Vector3(0, 1, 0));
	}
};
static int transform_get(lua_State* L)
{
	Transform* inst = *(Transform**)luaL_checkudata(L, 1, tname);
	switch (luaL_checkoption(L, 2, NULL, tmap_get))
	{
	case 0: // position
		lua_pushnumber(L, inst->pos.z);
		lua_pushnumber(L, inst->pos.y);
		lua_pushnumber(L, inst->pos.x);
		break;
	case 1: // rotation
		lua_pushnumber(L, inst->rotation.z);
		lua_pushnumber(L, inst->rotation.y);
		lua_pushnumber(L, inst->rotation.x);
		break;
	case 2: // scale
		lua_pushnumber(L, inst->scale.z);
		lua_pushnumber(L, inst->scale.y);
		lua_pushnumber(L, inst->scale.x);
		break;
	}

	return 3;
}

static int transform_set(lua_State* L)
{
	Transform* inst = *(Transform**)luaL_checkudata(L, 1, tname);

	int option = luaL_checkoption(L, 2, NULL, tmap_set);
	switch (option)
	{
	case 0: // position.x
		inst->pos.x = (float)luaL_checkinteger(L, 3);
		break;
	case 1: // position.y
		inst->pos.y = (float)luaL_checkinteger(L, 3);
		break;
	case 2: // position.z
		inst->pos.z = (float)luaL_checkinteger(L, 3);
		break;
	case 3: // rotation.x
		inst->rotation.x = (float)luaL_checkinteger(L, 3);
		break;
	case 4: // rotation.y
		inst->rotation.y = (float)luaL_checkinteger(L, 3);
		break;
	case 5: // rotation.z
		inst->rotation.z = (float)luaL_checkinteger(L, 3);
		break;
	case 6: // scale.x
		inst->scale.x = (float)luaL_checkinteger(L, 3);
		break;
	case 7: // scale.y
		inst->scale.y = (float)luaL_checkinteger(L, 3);
		break;
	case 8: // scale.z
		inst->scale.z = (float)luaL_checkinteger(L, 3);
		break;
	default:
		break;
	}

	return 0;
}

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

struct Script
{
	std::vector<std::string> scripts;
};

struct model
{
	Model3D* data;
};
