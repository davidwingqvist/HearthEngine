#include "header.h"
#include "EngineGUI.h"
#include "D3D11Context.h"
#include "Debugger.h"

EngineGUI::EngineGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	if (!ImGui_ImplWin32_Init(D3D11Core::Get().GetWindow()))
	{
		Debugger::Get().Print("Error Implementing Win32 for ImGUI", Debugger::COLOR_RED);
	}

	if (!ImGui_ImplDX11_Init(D3D11Core::Get().Device(), D3D11Core::Get().Context()))
	{
		Debugger::Get().Print("Error Implementing DX11 for ImGUI", Debugger::COLOR_RED);
	}
}

EngineGUI::~EngineGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EngineGUI::Test()
{


}

void EngineGUI::TestRender()
{

}
