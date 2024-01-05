#pragma once

#define LUA LuaHandler::Get()

class LuaHandler
{
private:

	lua_State* m_luaState;

	LuaHandler();
	~LuaHandler();

public:

	static auto& Get()
	{
		static LuaHandler instance;
		return instance;
	}

	// Dump everything from the stack onto the console window.
	static void DumpStack();

	// Register a function from C to LUA.
	static void RegisterFunction(const char* func_name, lua_CFunction func);

	// Load a script into the engine.
	static bool LoadScript(const char* script_name);

	// Open a script in a new window.
	static void OpenScriptFile(const char* script_name);
};

