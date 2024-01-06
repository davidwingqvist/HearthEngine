#include "header.h"
#include "EngineGUI.h"
#include "D3D11Context.h"
#include "Debugger.h"
#include "LuaState.h"

constexpr ImGuiWindowFlags menuWindow = (ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

EngineGUI::EngineGUI()
{
	IMGUI_CHECKVERSION();
	auto test = ImGui::CreateContext();
	// Setup ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(D3D11Core::Get().GetWindow()->GetHWnd());
	ImGui_ImplDX11_Init(D3D11Core::Get().Device(), D3D11Core::Get().Context());
	ImGui::StyleColorsDark();
	ImGui_ImplDX11_CreateDeviceObjects(); // uses device, therefore has to be called before render thread starts
}

EngineGUI::~EngineGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EngineGUI::RenderGUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Get().RenderTopBar();
}

void EngineGUI::CommitGUI()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EngineGUI::SetSceneManagerRef(SceneManager* ref_pointer)
{
	Get().m_sceneManagerRef = ref_pointer;
}

void EngineGUI::RenderTopBar()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(D3D11Core::Get().GetWindow()->GetWidth(), 0));
	
	ImGui::Begin("Menu", NULL, menuWindow);

	ImGui::BeginMenuBar();

	ImGui::Button("File");
	if (ImGui::Button("Edit"))
		m_showEditTab = !m_showEditTab;
	ImGui::Button("View");
	ImGui::Button("Run");

	ImGui::EndMenuBar();

	ImGui::End();

	if (m_showEditTab)
	{
		ImGui::Begin("Edit Tab", &m_showEditTab);
		if (ImGui::Button("Scripts", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
			m_showScriptsTab = !m_showScriptsTab;
		if (ImGui::Button("Objects", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
			m_showObjectsTab = !m_showObjectsTab;

		ImGui::End();
	}

	if (m_showScriptsTab)
	{
		ImGui::Begin("Scripts", &m_showScriptsTab);
		ImGui::InputTextWithHint(":", "Input script name", m_createScriptPath, sizeof m_createScriptPath);
		ImGui::SameLine();
		if (ImGui::Button("Create Script"))
		{
			LUA.CreateScriptFile(m_createScriptPath);
			LUA.ScanForScripts();
			memset(m_createScriptPath, '\0', sizeof m_createScriptPath);
		}
		ImGui::BeginListBox("All Scripts");
		for (int i = 0; i < LUA.GetScriptNames().size(); i++)
		{
			ImGui::Text(LUA.GetScriptNames()[i].c_str());
			ImGui::SameLine();
			std::string name = "Open###" + std::to_string(i);
			if (ImGui::Button(name.c_str()))
			{
				LUA.OpenScriptFile(LUA.GetScriptNames()[i].c_str());
			}
			ImGui::SameLine();
			name = "Remove##" + std::to_string(i);
			if (ImGui::Button(name.c_str()))
			{
				LUA.DeleteScriptFile(LUA.GetScriptNames()[i].c_str());
				LUA.ScanForScripts();
			}
		}
		ImGui::EndListBox();
		ImGui::End();
	}

	if (m_showObjectsTab)
	{
		ImGui::Begin("Objects Tab");
		ImGui::BeginListBox("All Objects");
		recs::recs_registry& reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();

		reg.View<GameObject>().ForEach([](const recs::Entity& entity, GameObject& gameObject) {

			char input[32] = { '\0' };
			std::string name = "Entity " + std::to_string(entity);
			if (ImGui::InputTextWithHint(name.c_str(), gameObject.name.c_str(), input, sizeof input, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				gameObject.name = input;
			}

			});

		ImGui::EndListBox();
		ImGui::End();
	}

}
