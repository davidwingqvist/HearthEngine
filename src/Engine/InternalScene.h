#pragma once

class InternalScene
{
public:

	virtual void Update() = 0;
	virtual void Awake() = 0;
	virtual void Draw() = 0;

	recs::recs_registry& GetRegistry();

protected:

	recs::recs_registry m_registry;
};