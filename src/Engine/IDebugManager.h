#pragma once

class IDebugManager
{
protected:

	bool m_visible;

	// Called within the Draw function, for cleaner code.
	virtual void InnerDraw() = 0;

public:

	// Called by the overall manager, should contain drawing calls.
	virtual void Draw() = 0;

	// Toggles the visible.
	void SetVisiblity();

	// Set visibility to directly.
	void SetVisiblity(const bool& visible);

	IDebugManager();
	virtual ~IDebugManager() = 0 {};
};