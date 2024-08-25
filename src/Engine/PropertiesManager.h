#pragma once
#include "IDebugManager.h"

class PropertiesManager : public IDebugManager
{
private:
	// Inherited via IDebugManager
	void InnerDraw() override;

public:

	PropertiesManager();

	void Draw() override;

	void Update() override;
};