#include "Header.h"
#include "LuaState.h"
#include "Debugger.h"
#include <Windows.h>
#include <shellapi.h>
#include <filesystem>
#include "CppToLua.h"

// Open up a fresh lua state.
LuaHandler::LuaHandler()
{
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);
	m_currentRegistry = nullptr;

	this->LoadInFunctions();
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
}

void LuaHandler::LoadInDebugFunctions()
{
	this->RegisterFunction(Debug_LogInfo, "DEBUG_INFO");
}

lua_State* LuaHandler::State()
{
	return Get().m_luaState;
}

void LuaHandler::DumpStack()
{
	std::cout << "------- STACK DUMP -------\n";
	for (int i = lua_gettop(Get().m_luaState); i > 0; i--)
	{
		std::cout << "Index " << i << ": " << lua_typename(Get().m_luaState, lua_type(Get().m_luaState, i));

		//Print out more info about the data in the stack
		switch (lua_type(Get().m_luaState, i))
		{
		case LUA_TNUMBER:
			std::cout << " '" << lua_tonumber(Get().m_luaState, i) << "'";
			break;
		case LUA_TSTRING:
			std::cout << " '" << lua_tostring(Get().m_luaState, i) << "'";
			break;
		case LUA_TBOOLEAN:
			std::cout << " '" << lua_toboolean(Get().m_luaState, i) << "'";
			break;
		default:
			break;
		}
		std::cout << '\n';
	}
	std::cout << "--------------------------\n";
}

void LuaHandler::ClearStack()
{
	while (lua_gettop(Get().m_luaState) > 0)
	{
		lua_pop(Get().m_luaState, -1);
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



	//Debugger::Get().Print("Successfully loaded script: " + std::string(script_name) + "\n", Debugger::COLOR_GREEN);

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

	outfile << "local " << std::string(script_name) + "={}\n\n\n--This function runs when object is created.\nlocal function OnAwake()\n\n\n\nend\n\n\n--This function runs each Update cycle\nlocal function OnUpdate()\n\n\n\nend\n";

	outfile.close();
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
	std::filesystem::path currPath = std::filesystem::current_path();
	std::string strPath = currPath.generic_string() + "/" + SCRIPTPATH;

	for (const auto& entry : std::filesystem::directory_iterator(strPath))
	{
		Get().m_scriptNames.push_back(entry.path().filename().string());
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
