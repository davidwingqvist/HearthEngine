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

	LUA.LoadScript(LUA.GetScriptNameFromId(scriptId).c_str());
}
