#include "Header.h"
#include "LuaGameState.h"
#include "LuaState.h"

LuaGameState::LuaGameState()
{
}

LuaGameState::~LuaGameState()
{
}

const recs::Entity& LuaGameState::GetCurrentEntity() const
{
	return m_currentEntity;
}

void LuaGameState::SetCurrentEntity(const recs::Entity& entity)
{
	m_currentEntity = entity;
}

void LuaGameState::CreateObjectFromScript(const size_t& scriptId, const size_t& objectId)
{
	if (scriptId == 0 || objectId == recs::NULL_ENTITY)
		return;

	const std::string& name = LUA.GetScriptNameFromId(scriptId);
	const std::string& name_internal = name + "_EngineObject";
	LUA.LoadEngineScript(name_internal.c_str());

	LUA.ClearStack();

	lua_getglobal(LUA.State(), (name + "_Objects").c_str());
	lua_pushnumber(LUA.State(), objectId);
	LUA.LoadScript(name.c_str());
	lua_settable(LUA.State(), 1);

	LUA.ClearStack();
}

void LuaGameState::UpdateObjectFromScript(const size_t& scriptId, const size_t& objectId)
{
	if (scriptId == 0 || objectId == recs::NULL_ENTITY)
		return;

	const std::string scriptName = LUA.GetScriptNameFromId(scriptId);
	std::string functionCall = "UpdateSingle" + scriptName + "Object_Engine";

	lua_getglobal(LUA.State(), functionCall.c_str());
	lua_pushnumber(LUA.State(), objectId);
	lua_pcall(LUA.State(), 1, 0, 0);
}

void LuaGameState::AwakeObjectFromScript(const size_t& scriptId, const size_t& objectId)
{
	if (scriptId == 0 || objectId == recs::NULL_ENTITY)
		return;

	const std::string scriptName = LUA.GetScriptNameFromId(scriptId);
	std::string functionCall = "AwakeSingle" + scriptName + "Object_Engine";

	lua_getglobal(LUA.State(), functionCall.c_str());
	lua_pushnumber(LUA.State(), objectId);
	lua_pcall(LUA.State(), 1, 0, 0);
}

void LuaGameState::CollisionObjectFromScript(const size_t& scriptId, const size_t& objectId)
{
	if (scriptId == 0 || objectId == recs::NULL_ENTITY)
		return;

	const std::string scriptName = LUA.GetScriptNameFromId(scriptId);
	std::string functionCall = "CollisionSingle" + scriptName + "Object_Engine";

	lua_getglobal(LUA.State(), functionCall.c_str());
	lua_pushnumber(LUA.State(), objectId);
	lua_pcall(LUA.State(), 1, 0, 0);
}
