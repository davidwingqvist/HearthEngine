#pragma once

class IDebugManager
{
protected:

	bool m_visible;

	// width of the window.
	float m_winWidth = 0;
	// height of the window.
	float m_winHeight = 0;

	// Called within the Draw function, for cleaner code.
	virtual void InnerDraw() = 0;

public:

	// Called by the overall manager, should contain drawing calls.
	virtual void Draw() = 0;

	// Toggles the visible.
	void SetVisiblity();

	// Process any updates related to the window.
	virtual void Update() = 0;

	// Set visibility to directly.
	void SetVisiblity(const bool& visible);

	IDebugManager();
	virtual ~IDebugManager() = 0 {};
};