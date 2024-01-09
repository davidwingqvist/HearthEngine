#include "Header.h"
#include "Structs.h"
#include "LuaState.h"

void pushTransform(lua_State* L, Transform* inst)
{
	Transform** ud = (Transform**)lua_newuserdata(L, sizeof(*ud));
	*ud = inst;

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

static int transform_get(lua_State* L)
{
	Transform* inst = *(Transform**)luaL_checkudata(L, 1, tname);
	*inst = *LUA.m_currentRegistry->GetComponent<Transform>(0);

	switch (luaL_checkoption(L, 2, NULL, tmap_get))
	{
	case 0: // position
		lua_pushnumber(L, inst->pos.x);
		lua_pushnumber(L, inst->pos.y);
		lua_pushnumber(L, inst->pos.z);
		break;
	case 1: // rotation
		lua_pushnumber(L, inst->rotation.x);
		lua_pushnumber(L, inst->rotation.y);
		lua_pushnumber(L, inst->rotation.z);
		break;
	case 2: // scale
		lua_pushnumber(L, inst->scale.x);
		lua_pushnumber(L, inst->scale.y);
		lua_pushnumber(L, inst->scale.z);
		break;
	}

	return 3;
}

static int transform_set(lua_State* L)
{
	Transform* inst = *(Transform**)luaL_checkudata(L, 1, tname);
	*inst = *LUA.m_currentRegistry->GetComponent<Transform>(0);

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