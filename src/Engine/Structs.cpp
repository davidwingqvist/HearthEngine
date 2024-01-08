#include "Header.h"
#include "Structs.h"

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