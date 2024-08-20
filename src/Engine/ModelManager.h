#pragma once
#include "IDebugManager.h"

class ModelManager: public IDebugManager
{
protected:

	void InnerDraw() override;

public:

	void Draw() override;

	ModelManager();
	~ModelManager();

};