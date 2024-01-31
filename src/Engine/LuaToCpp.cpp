#include "Header.h"
#include "LuaToCpp.h"
#include "Debugger.h"

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
