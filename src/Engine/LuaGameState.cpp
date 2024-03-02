#include "Header.h"
#include "LuaGameState.h"
#include "LuaState.h"

LuaGameState::LuaGameState()
{
}

LuaGameState::~LuaGameState()
{
}

void LuaGameState::CreateObjectFromScript(const size_t& scriptId)
{
	if (scriptId == 0)
		return;

	const std::string& name = LUA.GetScriptNameFromId(scriptId);
	const std::string& name_internal = name + "_EngineObject";
	LUA.LoadEngineScript(name_internal.c_str());

	lua_getglobal(LUA.State(), (name + "_Objects").c_str());

	/*
	
		TODO: ADD IDENTIFICATION FOR PUSH NUMBER!
	
	*/

	lua_pushnumber(LUA.State(), 0);
	LUA.LoadScript(name.c_str());


	lua_settable(LUA.State(), 1);

	lua_getglobal(LUA.State(), "UpdateHealthScriptObjects_Engine");
	lua_pcall(LUA.State(), 0, 0, 0);
}
