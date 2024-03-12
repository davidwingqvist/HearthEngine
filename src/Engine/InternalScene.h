#pragma once

class InternalScene
{
public:

	virtual void Update() = 0;
	virtual void Awake() = 0;
	virtual void Draw() = 0;

protected:

	recs::recs_registry m_registry;
};