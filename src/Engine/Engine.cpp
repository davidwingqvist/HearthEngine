#include "Header.h"
#include "Engine.h"
#include "Debugger.h"

Engine::Engine()
{
	srand(NULL);
	Debugger::Get();

	Window::Desc config;
	config.title = L"Game";
	if (!m_window.Initialize(config))
	{
		Debugger::Get().Print("Couldnt create Window!\n", Debugger::COLOR_RED);
		return;
	}

	D3D11Core::Get().Initialize(&m_window);
	if(!D2D1Core::Get().Initialize())
		DEBUG_ERROR("D2D1Core couldnt initialize!\n")

	this->m_renderer.Initialize();

	m_sceneManager.AddScene("Test");
	//m_sceneManager.SetScene("Test");

	DEBUG_INFO("The basics of the Engine is now up and running.\n");
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
	this->m_renderer.GetPipelineManager().ClearScreen();

	//D2D1Core::Get().Begin();

	m_renderer.Draw(m_sceneManager.GetCurrentScene());

	//D2D1Core::Get().Commit();

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

		Time::Get().SetDeltaTime(std::chrono::duration<float, std::milli>(end - start).count());
	}
}

void Engine::SetSplashScreen(const std::string& fileName)
{
	
	object2D splash(0,0, D3D11Core::Get().GetWindow()->GetWidth(), D3D11Core::Get().GetWindow()->GetHeight());
	
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

