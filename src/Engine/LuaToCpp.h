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

	TRANSFORM FUNCTIONS

*/
int Translate_Model(lua_State* L);
int SetPosition_Model(lua_State* L);
/*

	TRANSFORM FUNCTIONS

*/

/*

	TIME FUNCTIONS

*/
int GetDeltaTime(lua_State* L);
/*

	TIME FUNCTIONS

*/