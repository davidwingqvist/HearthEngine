#include "header.h"
#include "EngineGUI.h"
#include "D3D11Context.h"
#include "Debugger.h"
#include "LuaState.h"

constexpr ImGuiWindowFlags menuWindow = (ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
constexpr ImGuiWindowFlags bottomWindow = ImGuiWindowFlags_NoTitleBar;

/*

	Memory leaks in this singleton class.

*/

EngineGUI::EngineGUI()
{
#ifdef _DEBUG
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
#endif
}

EngineGUI::~EngineGUI()
{
#ifdef _DEBUG
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

void EngineGUI::RenderGUI()
{
#ifdef _DEBUG
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Get().RenderTopBar();
	Get().RenderBottomBar();
#endif
}

void EngineGUI::CommitGUI()
{
#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}

void EngineGUI::SetSceneManagerRef(SceneManager* ref_pointer)
{
#ifdef _DEBUG
	Get().m_sceneManagerRef = ref_pointer;
#endif
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
	if (ImGui::Button("Run"))
	{
		LUA.LoadScript("Test");
	}
	if (ImGui::Button("Dump Stack"))
	{
		LUA.DumpStack();
	}
	if (ImGui::Button("Clear Stack"))
	{
		LUA.ClearStack();
		DEBUG_INFO("LUA stack has been cleared.\n")
	}

	ImGui::EndMenuBar();

	ImGui::End();

	if (m_showEditTab)
	{
		ImGui::Begin("Edit Tab", &m_showEditTab, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button("Scripts", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
			m_showScriptsTab = !m_showScriptsTab;
		if (ImGui::Button("Entities", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
			m_showObjectsTab = !m_showObjectsTab;

		ImGui::End();
	}

	if (m_showScriptsTab)
	{
		ImGui::Begin("Scripts", &m_showScriptsTab, ImGuiWindowFlags_NoTitleBar);
		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Scripts");
		ImGui::InputTextWithHint("###ScriptNew", "Input script name", m_createScriptPath, sizeof m_createScriptPath);
		ImGui::SameLine();
		if (ImGui::Button("Create Script"))
		{
			LUA.CreateScriptFile(m_createScriptPath);
			LUA.ScanForScripts();
			memset(m_createScriptPath, '\0', sizeof m_createScriptPath);
		}
		
		if (ImGui::BeginListBox("###AllScripts"))
		{
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
		}
		ImGui::EndListBox();
		ImGui::End();
	}

	if (m_showObjectsTab)
	{
		ImGui::Begin("Objects Tab", NULL, ImGuiWindowFlags_NoTitleBar);
		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Objects");
		if (ImGui::BeginListBox("###AllObjects"))
		{
			recs::recs_registry& reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();

			const recs::Entity_Group grp = reg.GetEntities();

			for (int i = 0; i < grp.size(); i++)
			{
				std::string name = "Entity: " + std::to_string(i);
				ImGui::Text(name.c_str());

				ImGui::SameLine();
				name = "Open###Entity" + std::to_string(i);
				if (ImGui::Button(name.c_str()))
				{
					if (!m_showPropertiesTab)
					{
						m_showPropertiesTab = true;
					}

					m_currentEntity = grp[i];
				}
			}
		}
		ImGui::EndListBox();
		ImGui::End();
	}

	if (m_showPropertiesTab)
	{
		ImGui::Begin("Properties", &m_showPropertiesTab);

		recs::recs_registry& reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();
		
		GameObject* currGameObject = reg.GetComponent<GameObject>(m_currentEntity);

		if (currGameObject)
		{
			ImGui::BeginChild(1, ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "GameObject");
			std::string gameObjectName = "Name: " + currGameObject->name;
			ImGui::Text(gameObjectName.c_str());
			ImGui::SameLine();
			char input[32] = { '\0' };
			std::string name = "###PropInputName";
			if (ImGui::InputTextWithHint(name.c_str(), std::string("Input new name").c_str(), input, sizeof input, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				currGameObject->name = input;
			}
			ImGui::EndChild();
		}

		Transform* currTransform = reg.GetComponent<Transform>(m_currentEntity);
		if (currTransform)
		{
			ImGui::BeginChild(2, ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Transform");
			ImGui::BeginGroup();
			ImGui::Text("Position");
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 3.6f);
			ImGui::InputFloat("x###Posx", &currTransform->pos.x, 1, 10, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 3.6f);
			ImGui::InputFloat("y###Posy", &currTransform->pos.y, 1, 10, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 3.6f);
			ImGui::InputFloat("z###Posz", &currTransform->pos.z, 1, 10, "%.2f");

			ImGui::Text("Rotation");
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("x###Rotx", &currTransform->rotation.x, 0, 360, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("y###Roty", &currTransform->rotation.y, 0, 360, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("z###Rotz", &currTransform->rotation.z, 0, 360, "%.2f");

			ImGui::Text("Scale");
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("x###Scalex", &currTransform->scale.x, 0.1, 10, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("y###Scaley", &currTransform->scale.y, 0.1, 10, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("z###Scalez", &currTransform->scale.z, 0.1, 10, "%.2f");
			ImGui::EndGroup();
			ImGui::EndChild();
		}

		Script* currScripts = reg.GetComponent<Script>(m_currentEntity);

		if (currScripts)
		{
			ImGui::BeginChild(3, ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Scripts");
			std::string scriptId = "";

			ImGui::InputTextWithHint("###InputScriptNameProps", "Input script name", m_createScriptPathProp, sizeof m_createScriptPathProp);
			ImGui::SameLine();
			if (ImGui::Button("Add Script"))
			{
				if (LUA.LookUpScript(m_createScriptPathProp))
				{
					currScripts->scripts.push_back(std::string(m_createScriptPathProp) + ".lua");
				}
				memset(m_createScriptPathProp, '\0', sizeof m_createScriptPathProp);
			}

			for (int i = 0; i < currScripts->scripts.size(); i++)
			{
				scriptId = "Open###Script" + std::to_string(i);
				ImGui::Text(currScripts->scripts[i].c_str());
				ImGui::SameLine();
				if (ImGui::Button(scriptId.c_str()))
				{
					LUA.OpenScriptFile(currScripts->scripts[i].c_str());
				}
				ImGui::SameLine();
				scriptId = "Delete###Script" + std::to_string(i);
				ImGui::Button(scriptId.c_str());
			}
			
			ImGui::EndChild();
		}

		ImGui::End();
	}

}

void EngineGUI::RenderBottomBar()
{
	ImGui::SetNextWindowPos(ImVec2(0, D3D11Core::Get().GetWindow()->GetHeight() / 1.5f));
	ImGui::SetNextWindowSize(ImVec2(D3D11Core::Get().GetWindow()->GetWidth(), D3D11Core::Get().GetWindow()->GetHeight() / 3.0));

	ImGui::Begin("BottomBar", NULL, bottomWindow);

	ImGui::BeginChild("Sidebar", {ImGui::GetWindowWidth() / 5.0f, ImGui::GetContentRegionAvail().y }, ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar);

	ImGui::Button("Entities", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 10.0f });

	ImGui::EndChild();

	ImGui::End();
}
