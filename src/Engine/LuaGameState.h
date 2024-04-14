#pragma once
#define LUA_GAME LuaGameState::Get()

class LuaGameState
{
private:

	LuaGameState();
	~LuaGameState();

	recs::Entity m_currentEntity = recs::NULL_ENTITY;

public:

	static auto& Get()
	{
		static LuaGameState instance;
		return instance;
	}

	const recs::Entity& GetCurrentEntity() const;
	void SetCurrentEntity(const recs::Entity& entity);

	void CreateObjectFromScript(const size_t& scriptId, const size_t& objectId);
	void UpdateObjectFromScript(const size_t& scriptId, const size_t& objectId);
	void AwakeObjectFromScript(const size_t& scriptId, const size_t& objectId);
	void CollisionObjectFromScript(const size_t& scriptId, const size_t& objectId);
};