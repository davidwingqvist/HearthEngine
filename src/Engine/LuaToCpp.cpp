#include "Header.h"
#include "LuaToCpp.h"
#include "Debugger.h"
#include "Time.h"
#include "LuaState.h"
#include "Utility.h"
#include "LuaGameState.h"

int Debug_LogInfo(lua_State* L)
{
	const auto type = lua_type(L, -1);
	std::string log;
	switch (type)
	{
	case LUA_TNUMBER:
		log = std::to_string(lua_tonumber(L, -1));
		break;
	case LUA_TSTRING:
		log = luaL_checkstring(L, -1);
		break;
	}

	DEBUG_INFO(log);
	return 0;
}

int Debug_LogError(lua_State* L)
{
	const auto type = lua_type(L, -1);
	std::string log;
	switch (type)
	{
	case LUA_TNUMBER:
		log = std::to_string(lua_tonumber(L, -1));
		break;
	case LUA_TSTRING:
		log = luaL_checkstring(L, -1);
		break;
	}

	DEBUG_ERROR(log);
	return 0;
}

int Debug_LogSuccess(lua_State* L)
{
	const auto type = lua_type(L, -1);
	std::string log;
	switch (type)
	{
	case LUA_TNUMBER:
		log = std::to_string(lua_tonumber(L, -1));
		break;
	case LUA_TSTRING:
		log = luaL_checkstring(L, -1);
		break;
	}

	DEBUG_SUCCESS(log);
	return 0;
}

int CreateEntity_Lua(lua_State* L)
{
	recs::Entity entity = LUA.m_currentRegistry->CreateEntity();

	lua_pushnumber(L, entity);

	return 1;
}

int DeleteEntity_Lua(lua_State* L)
{
	recs::Entity entity = lua_tonumber(L, -1);

	LUA.m_currentRegistry->DestroyEntity(entity);

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

int SetRotation_Model(lua_State* L)
{
	const float x = utility::ConvertToRadiant(luaL_checknumber(L, -3));
	const float y = utility::ConvertToRadiant(luaL_checknumber(L, -2));
	const float z = utility::ConvertToRadiant(luaL_checknumber(L, -1));

	Transform* transform = LUA.m_currentRegistry->GetComponent<Transform>(LUA_GAME.GetCurrentEntity());

	if (!transform)
		return 0;

	transform->rotation = { x, y, z };

	return 0;
}

int GetDeltaTime(lua_State* L)
{
	lua_pushnumber(L, Time::Get().GetDeltaTime());
	return 1;
}
