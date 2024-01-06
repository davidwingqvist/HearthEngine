#pragma once

class EngineGUI
{
private:
	EngineGUI();
	~EngineGUI();
public:
	static auto& Get()
	{
		static EngineGUI instance;
		return instance;
	}

	static void Test();
	static void TestRender();
};

