#include "Header.h"
#include "LuaState.h"
#include "Debugger.h"
#include <Windows.h>
#include <shellapi.h>
#include <filesystem>

// Open up a fresh lua state.
LuaHandler::LuaHandler()
{
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);
}

LuaHandler::~LuaHandler()
{
	lua_close(m_luaState);
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

void LuaHandler::RegisterFunction(const char* func_name, lua_CFunction func)
{
	lua_register(Get().m_luaState, func_name, func);
}

bool LuaHandler::LoadScript(const char* script_name)
{
	//No name on the file...
	if (!script_name) return false;

	std::string script = SCRIPTPATH + script_name;

	if (luaL_dofile(Get().m_luaState, script.c_str()) != LUA_OK)
	{
		Debugger::Get().Print("Error loading script: " + std::string(script_name) + " = " + lua_tostring(Get().m_luaState, -1) + "\n", Debugger::COLOR_RED);
		return false;
	}



	Debugger::Get().Print("Successfully loaded script: " + std::string(script_name) + "\n", Debugger::COLOR_GREEN);

	return true;
}

void LuaHandler::OpenScriptFile(const char* script_name)
{
	std::filesystem::path currPath = std::filesystem::current_path();
	std::string strPath = currPath.generic_string() + "/" + SCRIPTPATH + script_name;

	Debugger::Get().Print("Opening path: " + strPath);

	ShellExecute(0, 0, std::wstring(strPath.begin(), strPath.end()).c_str(), 0, 0, SW_SHOW);
}
