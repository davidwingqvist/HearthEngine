#pragma once
#define LUA LuaHandler::Get()

class LuaHandler
{
private:

	lua_State* m_luaState;

	LuaHandler();
	~LuaHandler();

	std::vector<std::string> m_scriptNames;
	std::unordered_map<std::string, size_t> m_scriptNameToId;
	std::unordered_map<size_t, std::string> m_scriptIdToName;
	std::hash<std::string> m_hasher;

private:

	// Helper function for function registry.
	void RegisterFunction(lua_CFunction func, const std::string& funcName);

	// All functions for lua.
	void LoadInFunctions();

	// Debug functions for lua.
	void LoadInDebugFunctions();


	void LoadInEngineScripts();

public:

	recs::recs_registry* m_currentRegistry;

	static auto& Get()
	{
		static LuaHandler instance;
		return instance;
	}

	// Get the Lua State
	static lua_State* State();

	// Dump everything from the stack onto the console window.
	static void DumpStack();

	// Clear the stack of everything, create a fresh clean stack.
	static void ClearStack();

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

	static std::string& GetScriptNameFromId(const size_t& id);
	static size_t& GetIdFromScriptName(const std::string& script_name);
};

