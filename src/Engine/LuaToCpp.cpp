#include "Header.h"
#include "LuaToCpp.h"
#include "Debugger.h"
#include "Time.h"
#include "LuaState.h"
#include "LuaGameState.h"

int Debug_LogInfo(lua_State* L)
{
	const std::string log = luaL_checkstring(L, -1);
	DEBUG_INFO(log);
	return 0;
}

int Debug_LogError(lua_State* L)
{
	const std::string log = luaL_checkstring(L, -1);
	DEBUG_ERROR(log);
	return 0;
}

int Debug_LogSuccess(lua_State* L)
{
	const std::string log = luaL_checkstring(L, -1);
	DEBUG_SUCCESS(log);
	return 0;
}

int Translate_Model(lua_State* L)
{
	const float x = luaL_checknumber(L, -3);
	const float y = luaL_checknumber(L, -2);
	const float z = luaL_checknumber(L, -1);

	Transform* transform = LUA.m_currentRegistry->GetComponent<Transform>(LUA_GAME.GetCurrentEntity());

	if (!transform)
		return 0;

	transform->pos.x += x;
	transform->pos.y += y;
	transform->pos.z += z;

	return 0;
}

int SetPosition_Model(lua_State* L)
{
	const float x = luaL_checknumber(L, -3);
	const float y = luaL_checknumber(L, -2);
	const float z = luaL_checknumber(L, -1);

	Transform* transform = LUA.m_currentRegistry->GetComponent<Transform>(LUA_GAME.GetCurrentEntity());

	if (!transform)
		return 0;

	transform->pos = { x, y, z };
	return 0;
}

int GetDeltaTime(lua_State* L)
{
	lua_pushnumber(L, Time::Get().GetDeltaTime());
	return 1;
}
