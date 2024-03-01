#pragma once
#define LUA_GAME LuaGameState::Get()

class LuaGameState
{
private:

	LuaGameState();
	~LuaGameState();

	std::hash<std::string> m_hasher;

public:



	static auto& Get()
	{
		static LuaGameState instance;
		return instance;
	}
};