#pragma once
#define LUA_GAME LuaGameState::Get()

class LuaGameState
{
private:

	LuaGameState();
	~LuaGameState();

public:

	static auto& Get()
	{
		static LuaGameState instance;
		return instance;
	}

	void CreateObjectFromScript(const size_t& scriptId, const size_t& objectId);
};