#pragma once

/*
	LUA DEBUG FUNCTIONS 
*/
int Debug_LogInfo(lua_State* L);
int Debug_LogError(lua_State* L);
int Debug_LogSuccess(lua_State* L);
/*
	LUA DEBUG FUNCTIONS
*/


/*
	ENTITY FUNCTIONS
*/
int CreateEntity_Lua(lua_State* L);
int DeleteEntity_Lua(lua_State* L);
int DestroyEntity_Lua(lua_State* L);
/*
	ENTITY FUNCTIONS
*/


/*
	TRANSFORM FUNCTIONS
*/
int Translate_Model(lua_State* L);
int SetPosition_Model(lua_State* L);
int SetRotation_Model(lua_State* L);
int SetScale_Model(lua_State* L);
/*
	TRANSFORM FUNCTIONS
*/

/*
	GAMEOBJECT FUNCTIONS
*/
int SetTag_GameObject(lua_State* L);
/*
	GAMEOBJECT FUNCTIONS
*/


/*
	TIME FUNCTIONS
*/
int GetDeltaTime(lua_State* L);
/*
	TIME FUNCTIONS
*/