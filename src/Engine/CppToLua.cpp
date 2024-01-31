#include "Header.h"
#include "CppToLua.h"
#include "Debugger.h"

int Debug_LogInfo(lua_State* L)
{
	const std::string log = luaL_checkstring(L, -1);
	DEBUG_INFO(log);
	return 0;
}
