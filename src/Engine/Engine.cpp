#include "Header.h"
#include "Engine.h"
#include "Debugger.h"
#include "LuaState.h"
#include "InputManager.h"

Engine::Engine()
{
	srand(NULL);
	Debugger::Get();

	Window::Desc config;
	config.height = 720;
	config.width = 1280;
	config.title = L"Hearth Engine";
	if (!m_window.Initialize(config))
	{
		Debugger::Get().Print("Couldnt create Window!\n", Debugger::COLOR_RED);
		return;
	}

	D3D11Core::Get().Initialize(&m_window);
	/*if(!D2D1Core::Get().Initialize())
		DEBUG_ERROR("D2D1Core couldnt initialize!\n")*/

	this->m_renderer.Initialize();

	m_sceneManager.AddInternalScene("Internal_EditScene");

	DEBUG_INFO("The basics of the Engine is now up and running.\n");
	EngineGUI::Get().SetSceneManagerRef(&m_sceneManager);
	LUA.ScanForScripts();

	// Create the resource manager.
	ResourceManager::Get();
	EngineGUI::StartUp();
}

Engine::Engine(const std::string& splashScreen)
	:Engine()
{
	//this->SetSplashScreen(splashScreen);
}

Engine::~Engine()
{
}

void Engine::Update()
{
	InputManager::Get().Update();
	MSG msg = { nullptr };

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			shutdown = true;
		}
	}


	if (m_sceneManager.GetCurrentScene())
		m_sceneManager.GetCurrentScene()->Update();
}

void Engine::Draw()
{
	m_sceneManager.GetCurrentScene()->PreDraw();
	{ // Ingame rendering.
		this->m_renderer.GetPipelineManager().ClearScreen();
		/*D2D1Core::Get().Begin();*/

		m_renderer.Draw(m_sceneManager.GetCurrentScene());

		/*D2D1Core::Get().Commit();*/
	}
	m_sceneManager.GetCurrentScene()->PostDraw();

	D3D11Core::Get().Present();
}

void Engine::Build()
{

}

void Engine::Start()
{
	while (!shutdown)
	{
		auto start = std::chrono::high_resolution_clock::now();
		Update();

		Draw();
		
		auto end = std::chrono::high_resolution_clock::now();
		Time::Get().SetDeltaTime(std::chrono::duration<float, std::milli>(end - start).count() * 0.001f);
	}
}

void Engine::SetSplashScreen(const std::string& fileName)
{
	
	object2D splash(0.0f,0.0f, (float)D3D11Core::Get().GetWindow()->GetWidth(), (float)D3D11Core::Get().GetWindow()->GetHeight());
	
	Image2D* image = ResourceManager::Get().GetResource<Image2D>(fileName).get();

	this->m_renderer.GetPipelineManager().ClearScreen();
	D2D1Core::Get().Begin();
	D2D1Core::Get().DrawP(splash, image->GetImage());
	D2D1Core::Get().Commit();
	D3D11Core::Get().Present();
}

void Engine::Shutdown()
{
	Debugger::Get().Print("Engine has started shutdown sequence.\n", Debugger::COLOR_WHITE);
	shutdown = true;
}

