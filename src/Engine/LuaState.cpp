#include "Header.h"
#include "LuaState.h"
#include "Debugger.h"
#include <Windows.h>
#include <shellapi.h>
#include <filesystem>
#include "LuaToCpp.h"
#include "LuaGameState.h"

// Open up a fresh lua state.
LuaHandler::LuaHandler()
{
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);
	m_currentRegistry = nullptr;

	this->LoadInFunctions();
	LuaGameState::Get();
}

LuaHandler::~LuaHandler()
{
	lua_close(m_luaState);
}

void LuaHandler::RegisterFunction(lua_CFunction func, const std::string& funcName)
{
	lua_pushcfunction(m_luaState, func);
	lua_setglobal(m_luaState, funcName.c_str());
}

void LuaHandler::LoadInFunctions()
{
	this->LoadInDebugFunctions();
	this->LoadInComponentAffectingFunctions();
}

void LuaHandler::LoadInDebugFunctions()
{
	this->RegisterFunction(Debug_LogInfo, "DEBUG_INFO");
	this->RegisterFunction(Debug_LogError, "DEBUG_ERROR");
	this->RegisterFunction(Debug_LogSuccess, "DEBUG_SUCCESS");
}

void LuaHandler::LoadInComponentAffectingFunctions()
{
	this->RegisterFunction(GetDeltaTime, "GetDeltaTime");
	this->RegisterFunction(Translate_Model, "Translate");
	this->RegisterFunction(SetPosition_Model, "SetPosition");
	this->RegisterFunction(CreateEntity_Lua, "CreateEntity");
	this->RegisterFunction(DeleteEntity_Lua, "DestroyEntity");
}

lua_State* LuaHandler::State()
{
	return Get().m_luaState;
}

void LuaHandler::DumpStack()
{
	Debugger::Get().Print("------- STACK DUMP -------\n", Debugger::COLOR_PURPLE);
	std::string line;
	int i = lua_gettop(m_luaState);
	for (i; i > 0; i--)
	{
		Debugger::Get().Print("Index " + std::to_string(i) + ": " + lua_typename(m_luaState, lua_type(m_luaState, i)), Debugger::COLOR_PURPLE);

		//Print out more info about the data in the stack
		switch (lua_type(m_luaState, i))
		{
		case LUA_TNUMBER:
			DEBUG_SAMELINE(" '" + std::to_string(lua_tonumber(m_luaState, i)) + "'");
			break;
		case LUA_TSTRING:
			line = lua_tostring(m_luaState, i);
			DEBUG_SAMELINE(" '" + line + "'");
			break;
		default:
			break;
		}
	}
	Debugger::Get().Print("--------------------------\n", Debugger::COLOR_PURPLE);
}

void LuaHandler::ClearStack()
{
	while (lua_gettop(m_luaState) > 0)
	{
		lua_pop(m_luaState, -1);
	}
}

void LuaHandler::RegisterFunction(const char* func_name, lua_CFunction func)
{
	lua_register(Get().m_luaState, func_name, func);
}

bool LuaHandler::LoadScript(const char* script_name)
{
	//No name on the file...
	if (!script_name) return false;

	std::string script = SCRIPTPATH + script_name + ".lua";

	if (luaL_dofile(Get().m_luaState, script.c_str()) != LUA_OK)
	{
		Debugger::Get().Print("Error loading script: " + std::string(script_name) + " = " + lua_tostring(Get().m_luaState, -1) + "\n", Debugger::COLOR_RED);
		return false;
	}

	return true;
}

bool LuaHandler::LoadEngineScript(const char* script_name)
{
	//No name on the file...
	if (!script_name) return false;

	std::string script = SCRIPTPATH_INTERNAL + script_name + ".lua";

	if (luaL_dofile(Get().m_luaState, script.c_str()) != LUA_OK)
	{
		Debugger::Get().Print("Error loading script: " + std::string(script_name) + " = " + lua_tostring(Get().m_luaState, -1) + "\n", Debugger::COLOR_RED);
		return false;
	}

	return true;
}

void LuaHandler::OpenScriptFile(const char* script_name, const bool& addExtension)
{
	std::string ext = "";
	if (addExtension)
		ext = ".lua";

	std::filesystem::path currPath = std::filesystem::current_path();
	std::string strPath = currPath.generic_string() + "/" + SCRIPTPATH + script_name + ext;

	Debugger::Get().Print("Opening path: " + strPath + "\n");

	ShellExecute(0, 0, std::wstring(strPath.begin(), strPath.end()).c_str(), 0, 0, SW_SHOW);
}

void LuaHandler::CreateScriptFile(const char* script_name, const bool& addExtension)
{
	std::string ext = "";

	if (addExtension)
		ext = ".lua";

	std::ofstream outfile(SCRIPTPATH + script_name + ext);
	std::ofstream outfileSystem(SCRIPTPATH_INTERNAL + script_name + "_EngineObject" + ext);

	outfile << "local " << std::string(script_name) + "={}\n\n\n--This function runs when object is created.\nfunction " + std::string(script_name) + ":OnAwake()\n\n\n\nend\n\n\n--This function runs each Update cycle\nfunction " + std::string(script_name) + ":OnUpdate()\n\n\n\nend\n\nreturn " + std::string(script_name);
	outfileSystem << std::string(script_name) + "_Objects={}\n\nfunction Update" + std::string(script_name) + "Objects_Engine()\n\n\t\tfor k, v in pairs(" + std::string(script_name) + "_Objects) do\n\t\t\tv:OnUpdate()\n\t\tend\n\nend\n\nfunction UpdateSingle" + std::string(script_name) + "Object_Engine(key)\n\t" + std::string(script_name) + "_Objects[key]:OnUpdate()\nend\n\nfunction AwakeSingle" + std::string(script_name) + "Object_Engine(key)\n\t" + std::string(script_name) + "_Objects[key]:OnAwake()\nend";

	outfile.close();
	outfileSystem.close();
}

void LuaHandler::DeleteScriptFile(const char* script_name, const bool& addExtension)
{
	std::string ext = "";
	if (addExtension)
		ext = ".lua";

	std::filesystem::path currPath = std::filesystem::current_path();
	std::string strPath = currPath.generic_string() + "/" + SCRIPTPATH + script_name + ext;

	try {
		if (std::filesystem::remove(strPath))
		{
			DEBUG_INFO("Remove script file: " + std::string(script_name) + "\n");
		}
		else
		{
			DEBUG_ERROR("No file with the name: " + std::string(script_name) + "\n");
		}
	}
	catch (const std::filesystem::filesystem_error& err)
	{
		DEBUG_ERROR("Error occured in filesystem: " + std::string(err.what()));
		DEBUG_NEWLINE();
	}
}

void LuaHandler::ScanForScripts()
{
	Get().m_scriptNames.clear();
	Get().m_scriptIdToName.clear();
	Get().m_scriptNameToId.clear();
	std::filesystem::path currPath = std::filesystem::current_path();
	std::string strPath = currPath.generic_string() + "/" + SCRIPTPATH;

	for (const auto& entry : std::filesystem::directory_iterator(strPath))
	{
		Get().m_scriptNames.push_back(entry.path().filename().string());

		std::string scriptObjectName = entry.path().filename().string();
		scriptObjectName = scriptObjectName.substr(0, scriptObjectName.size() - 4);
		size_t hash = Get().m_hasher(scriptObjectName);

		Get().m_scriptNameToId[scriptObjectName] = hash;
		Get().m_scriptIdToName[hash] = scriptObjectName;
	}
}

std::vector<std::string>& LuaHandler::GetScriptNames()
{
	return Get().m_scriptNames;
}

bool LuaHandler::LookUpScript(const char* script_name, const bool& addExtenstion)
{
	std::string ext = "";

	if (addExtenstion)
		ext = ".lua";

	for (int i = 0; i < Get().m_scriptNames.size(); i++)
	{
		if (Get().m_scriptNames[i] == (std::string(script_name) + ext))
		{
			return true;
		}
	}
	return false;
}

std::string& LuaHandler::GetScriptNameFromId(const size_t& id)
{
	return Get().m_scriptIdToName[id];
}

size_t& LuaHandler::GetIdFromScriptName(const std::string& script_name)
{
	return Get().m_scriptNameToId[script_name];
}
