#include "Header.h"
#include "Structs.h"
#include "LuaState.h"
#include "LuaGameState.h"

void pushTransform(lua_State* L)
{
	Transform** ud = (Transform**)lua_newuserdata(L, sizeof(*ud));

	if (luaL_newmetatable(L, tname))
	{
		lua_pushcfunction(L, transform_set);
		lua_setfield(L, -2, "__newindex");
		lua_pushcfunction(L, transform_get);
		lua_setfield(L, -2, "__index");
		lua_pushstring(L, tname);
		lua_setfield(L, -2, "__metatable");
	}
	lua_setmetatable(L, -2);
}

sm::Matrix GetMatrix(const Transform& transform)
{
	sm::Matrix mat = sm::Matrix::CreateScale(transform.scale);
	mat *= sm::Matrix::CreateFromQuaternion(transform.rotation);
	mat.Translation(transform.pos);
	return mat;
}

static int transform_get(lua_State* L)
{
	if (LUA_GAME.GetCurrentEntity() == recs::NULL_ENTITY)
	{
		return 0;
	}

	Transform* inst = *(Transform**)luaL_checkudata(L, 1, tname);
	*inst = *LUA.m_currentRegistry->GetComponent<Transform>(LUA_GAME.GetCurrentEntity());

	switch (luaL_checkoption(L, 2, NULL, tmap_set))
	{
	case 0: // position x
		lua_pushnumber(L, inst->pos.x);
		break;
	case 1: // position y
		lua_pushnumber(L, inst->pos.y);
		break;
	case 2: // position z
		lua_pushnumber(L, inst->pos.z);
		break;
	case 3: // rotation x
		lua_pushnumber(L, inst->rotation.x);
		break;
	case 4: // rotation y
		lua_pushnumber(L, inst->rotation.y);
		break;
	case 5: // rotation z
		lua_pushnumber(L, inst->rotation.z);
		break;
	case 6: // scale x
		lua_pushnumber(L, inst->scale.x);
		break;
	case 7: // scale y
		lua_pushnumber(L, inst->scale.y);
		break;
	case 8: // scale z
		lua_pushnumber(L, inst->scale.z);
		break;
	}

	return 1;
}

static int transform_set(lua_State* L)
{
	if (LUA_GAME.GetCurrentEntity() == recs::NULL_ENTITY)
	{
		return 0;
	}

	Transform* inst = *(Transform**)luaL_checkudata(L, 1, tname);
	*inst = *LUA.m_currentRegistry->GetComponent<Transform>(LUA_GAME.GetCurrentEntity());

	int option = luaL_checkoption(L, 2, NULL, tmap_set);
	switch (option)
	{
	case 0: // position.x
		inst->pos.x = (float)luaL_checknumber(L, 3);
		break;
	case 1: // position.y
		inst->pos.y = (float)luaL_checknumber(L, 3);
		break;
	case 2: // position.z
		inst->pos.z = (float)luaL_checknumber(L, 3);
		break;
	case 3: // rotation.x
		inst->rotation.x = (float)luaL_checknumber(L, 3);
		break;
	case 4: // rotation.y
		inst->rotation.y = (float)luaL_checknumber(L, 3);
		break;
	case 5: // rotation.z
		inst->rotation.z = (float)luaL_checknumber(L, 3);
		break;
	case 6: // scale.x
		inst->scale.x = (float)luaL_checknumber(L, 3);
		break;
	case 7: // scale.y
		inst->scale.y = (float)luaL_checknumber(L, 3);
		break;
	case 8: // scale.z
		inst->scale.z = (float)luaL_checknumber(L, 3);
		break;
	default:
		break;
	}

	return 0;
}