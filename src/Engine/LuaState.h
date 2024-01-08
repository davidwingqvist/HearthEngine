#pragma once

#define LUA LuaHandler::Get()

class LuaHandler
{
private:

	lua_State* m_luaState;

	LuaHandler();
	~LuaHandler();

	std::vector<std::string> m_scriptNames;

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
	static void OpenScriptFile(const char* script_name, const bool& addExtension = false);

	// Create a script with the specified name. If it already exists, open it.
	static void CreateScriptFile(const char* script_name, const bool& addExtension = true);

	// Delete an existing script file in the script folder.
	static void DeleteScriptFile(const char* script_name, const bool& addExtension = false);

	// Run this to scan for scripts located in the script folder.
	static void ScanForScripts();

	// Get the list of names of scripts available.
	static std::vector<std::string>& GetScriptNames();

	// Search for a saved script, return true if it exists
	static bool LookUpScript(const char* script_name, const bool& addExtenstion = true);
};

