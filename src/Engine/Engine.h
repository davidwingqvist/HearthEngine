#pragma once
#include "D3D11Context.h"
#include "Direct2DContext.h"
#include "Debugger.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Renderer.h"
#include "EngineGUI.h"

class Engine
{
private:

	Window m_window;
	Renderer m_renderer;
	bool shutdown = false;
	SceneManager m_sceneManager;


	void Update();
	void Draw();

public:

	Engine();
	Engine(const std::string& splashScreen);
	~Engine();

	// Run this function to build the engine.
	void Build();

	/*
	
		Run this function to start the whole engine operation.
	
	*/
	void Start();

	void SetSplashScreen(const std::string& fileName);

	void Shutdown();
	
};

