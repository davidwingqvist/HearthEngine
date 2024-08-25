#include "header.h"
#include "EngineGUI.h"
#include "D3D11Context.h"
#include "Debugger.h"
#include "LuaState.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Time.h"
#include "Icon.h"
#include "GameScene.h"

constexpr ImGuiWindowFlags menuWindow = (ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
constexpr ImGuiWindowFlags bottomWindow = ImGuiWindowFlags_NoTitleBar;

EngineGUI::EngineGUI()
{
	IMGUI_CHECKVERSION();
	// Setup ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(D3D11Core::Get().GetWindow()->GetHWnd());
	ImGui_ImplDX11_Init(D3D11Core::Get().Device(), D3D11Core::Get().Context());
	ImGui::StyleColorsDark();
	ImGui_ImplDX11_CreateDeviceObjects(); // uses device, therefore has to be called before render thread starts

	m_overallManager.push_back(&m_modelManager);
	m_overallManager.push_back(&m_propertiesManager);
}

EngineGUI::~EngineGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EngineGUI::ClearConsoleWindowText()
{
	m_consoleLogs.clear();
}

void EngineGUI::ClearConsoleLogToLimit()
{
	while(m_consoleLogs.size() > m_consoleLogLimit)
	{
		m_consoleLogs.erase(m_consoleLogs.begin());
	}
}

void EngineGUI::PutEditTabsToFalse()
{
	m_showScriptsTab = false;
	m_showFileKeeper = false;
	m_showScriptsHandlerTab = false;
}

void EngineGUI::BottomBarPutToFalse()
{
	m_showBottomConsole = false;
	m_showBottomFiles = false;
	m_showBottomStatistics = false;
	m_showBottomScenes = false;
}

void EngineGUI::RenderGUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Get().RenderTopBar();
	Get().RenderBottomBar();
	Get().RenderHierarchy();
	Get().RenderFileKeepingWindow();
	Get().RenderNewComponentTab();
	Get().RenderModelsTab();
	Get().RenderTextureTab();
	Get().RenderScriptsTab();
	Get().RenderScenesTab();

	for (auto manager : Get().m_overallManager)
	{
		manager->Draw();
	}
}

void EngineGUI::CommitGUI()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

DragPointer* EngineGUI::GetDragPointers()
{
	return &m_dragPointers;
}

void EngineGUI::StartUp()
{
	Get().m_dragPointers.StartUp();
}

void EngineGUI::DrawDragPointers()
{
	Get().m_dragPointers.Draw(Get().m_currentEntity);
}

void EngineGUI::SetSceneManagerRef(SceneManager* ref_pointer)
{
	Get().m_sceneManagerRef = ref_pointer;
}

void EngineGUI::RegisterConsoleLog(const ConsoleLogEvent& event)
{
	Get().m_consoleLogs.push_back(event);
}

ConsoleLogEvent& EngineGUI::AdjustConsoleLog(const size_t& pos)
{
	return Get().m_consoleLogs[pos];
}

const size_t& EngineGUI::GetConsoleLogSize()
{
	return Get().m_consoleLogs.size();
}

void EngineGUI::SetActiveEntity(const recs::Entity& entity)
{
	Get().m_currentEntity = entity;
}

void EngineGUI::UpdateManagers()
{
	for (auto manager : Get().m_overallManager)
	{
		manager->Update();
	}
}

void EngineGUI::RenderTopBar()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(
		ImVec2(D3D11Core::Get().GetWindow()->GetWidth(),
			(float)D3D11Core::Get().GetWindow()->GetHeight() * 0.05f)
	);
	
	ImGui::Begin("Menu", NULL, menuWindow);

	ImGui::BeginMenuBar();

	if (ImGui::Button("File"))
	{
		m_showEditTab = false;
		m_showFileKeeper = !m_showFileKeeper;
		m_showScriptsHandlerTab = false;
	}

	if (ImGui::Button("Edit"))
	{
		PutEditTabsToFalse();
		m_showFileKeeper = false;

		m_showEditTab = !m_showEditTab;
	}

	if (ImGui::Button("Run"))
	{
		m_sceneManagerRef->StartGameScene(m_sceneManagerRef->GetCurrentSceneName());
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

	ImGui::Text(("Current Scene: " + m_sceneManagerRef->GetCurrentSceneName()).c_str());

	ImGui::EndMenuBar();

	ImGui::End();

	if (m_showEditTab)
	{
		ImGui::Begin("Edit Tab", &m_showEditTab, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button("Scenes", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
		{
			this->PutEditTabsToFalse();
			m_showScenesTab = !m_showScenesTab;
		}
		if (ImGui::Button("Scripts", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
		{
			this->PutEditTabsToFalse();
			m_showScriptsHandlerTab = !m_showScriptsHandlerTab;
		}
		if (ImGui::Button("Models", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
		{
			this->PutEditTabsToFalse();
			m_modelManager.SetVisiblity();
		}
		if (ImGui::Button("Textures", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
		{
			this->PutEditTabsToFalse();
		}
		if (ImGui::Button("Sounds", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
		{
			this->PutEditTabsToFalse();
		}
		if (ImGui::Button("Shaders", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
		{
			this->PutEditTabsToFalse();
		}
		if (ImGui::Button("Effects", ImVec2(ImGui::GetWindowContentRegionMax().x, 0)))
		{
			this->PutEditTabsToFalse();
		}

		ImGui::End();
	}

	if (m_showScriptsHandlerTab)
	{
		ImGui::Begin("Scripts", &m_showScriptsHandlerTab, ImGuiWindowFlags_NoTitleBar);
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
			recs::recs_registry* reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();

			const recs::Entity_Group grp = reg->GetEntities();

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

	//this->RenderProperties();
}

void EngineGUI::RenderBottomBar()
{
	ImGui::SetNextWindowPos(ImVec2(0, D3D11Core::Get().GetWindow()->GetHeight() / 1.5f));
	ImGui::SetNextWindowSize(ImVec2(D3D11Core::Get().GetWindow()->GetWidth(), D3D11Core::Get().GetWindow()->GetHeight() / 3.0));

	ImGui::Begin("BottomBar", NULL, bottomWindow);

	ImGui::BeginChild("Sidebar", {ImGui::GetWindowWidth() / 5.0f, ImGui::GetContentRegionAvail().y }, ImGuiChildFlags_Border);

	if (ImGui::Button("Items", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 10.0f }))
	{
		BottomBarPutToFalse();
		m_showBottomFiles = true;
	}

	if (ImGui::Button("Scenes", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 10.0f }))
	{
		BottomBarPutToFalse();
		m_showBottomScenes = true;
	}

	if (ImGui::Button("Console", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 10.0f }))
	{
		BottomBarPutToFalse();
		m_showBottomConsole = true;
	}

	if (ImGui::Button("Statistics & Options", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 10.0f }))
	{
		BottomBarPutToFalse();
		m_showBottomStatistics = true;
	}

	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("SideWindow", { ImGui::GetWindowWidth() / 1.3f, ImGui::GetContentRegionAvail().y }, ImGuiChildFlags_Border);

	if (m_showBottomConsole)
	{
		this->RenderConsole();
	}
	else if (m_showBottomStatistics)
	{
		this->RenderStatistics();
	}
	else if (m_showBottomScenes)
	{
		this->RenderScenes();
	}

	ImGui::EndChild();

	ImGui::End();
}

void EngineGUI::RenderHierarchy()
{
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetWindowSize().y * 0.05f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.6f, 
		WINDOW->GetHeight() * 0.65f));
	ImGui::Begin("Object View", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);
	if (m_sceneManagerRef)
	{
		auto reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();

		if (!reg)
		{
			ImGui::End();
			return;
		}

		ImGui::BeginMenuBar();
		if (ImGui::Button("Create New Entity"))
		{
			auto e = reg->CreateEntity();
			// should always have a gameobject and transform component.
			reg->AddComponent<GameObject>(e);
			reg->AddComponent<Transform>(e);
			reg->AddComponent<InternalBox>(e);
		}

		if (ImGui::Button("Delete All"))
		{
			auto& e = reg->GetEntities();
			while (!e.empty())
			{
				reg->DestroyEntity(e.back());
			}
		}

		ImGui::EndMenuBar();

		auto& ent = reg->GetEntities();

		for (auto& e : ent)
		{
			std::string ent_string = "Entity: " + std::to_string(e);
			auto gameObject = reg->GetComponent<GameObject>(e);
			if (gameObject)
			{
				ent_string = gameObject->name;
			}
			ImGui::TextWrapped(ent_string.c_str());
			if (ImGui::IsItemClicked())
			{
				m_currentEntity = e;
				m_showPropertiesTab = true;
			}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.8f);
			std::string tag = "X###deleteEntity" + std::to_string(e);
			if (ImGui::Button(tag.c_str(), {ImGui::GetWindowWidth() * 0.15f, 0.0}))
			{
				reg->DestroyEntity(e);

				// set null entity if deleted currently selected entity.
				if(e == m_currentEntity)
					m_currentEntity = recs::NULL_ENTITY;
			}
		}

	}
	ImGui::End();
}

void EngineGUI::RenderConsole()
{
	// Upper line button bar.
	if (ImGui::Button("Clear Console"))
	{
		this->ClearConsoleWindowText();
	}

	ImGui::SameLine();
	ImGui::Text("Display:");
	ImGui::SameLine();
	if (ImGui::Button("All"))
	{
		m_filter = (UINT)ConsoleLogEventType::ALL;
	}
	ImGui::SameLine();
	if (ImGui::Button("None"))
	{
		m_filter = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Info"))
	{
		m_filter = 0;
		m_filter |= (UINT)ConsoleLogEventType::LOG_INFO;
	}
	ImGui::SameLine();
	if (ImGui::Button("Error"))
	{
		m_filter = 0;
		m_filter |= (UINT)ConsoleLogEventType::LOG_ERROR;
	}
	ImGui::SameLine();
	if (ImGui::Button("Success"))
	{
		m_filter = 0;
		m_filter |= (UINT)ConsoleLogEventType::LOG_SUCCESS;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.1f);
	ImGui::InputInt("Log limit", &m_consoleLogLimit, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue);

	this->ClearConsoleLogToLimit();

	// Upper line button bar.

	// Console list.
	ImGui::BeginListBox("###consoleLog", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowSize().y * 0.72f));
	for (int i = m_consoleLogs.size() - 1; i >= 0; i--)
	{
		if(m_filter & (UINT)m_consoleLogs[i].type)
			ImGui::TextColored(m_consoleLogs[i].color, m_consoleLogs[i].data.c_str());
	}
	ImGui::EndListBox();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.93);
	ImGui::InputText("###consoleWindowInput", m_consoleWindowInput, 100);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.07);
	ImGui::Button("Enter###consoleWindowEnter");
}

void EngineGUI::RenderStatistics()
{
	std::string framesText = "FPS: " + std::to_string(1 / Time::Get().GetDeltaTime());
	ImGui::Text(framesText.c_str());


	if (m_toggleCollisionBoxDraw != nullptr)
	{
		ImGui::Checkbox("Render Collision Boxes?", m_toggleCollisionBoxDraw);
	}

	if (m_editCamera)
	{
		ImGui::DragFloat("Camera Look Sensitity###rightClickCameraSens", &m_editCamera->GetSensitivty(), .05f, 0.01f, 10.0f);
		ImGui::DragFloat("Camera Sphere Look Range##middleClickSphereRadius", &m_editCamera->GetSphereRadius(), 1.0f, 0.1f, 1000.0f);
		ImGui::DragFloat("Camera Move Speed##CamMoveSpeed", &m_editCamera->GetSpeed(), .05f, 0.01f, 10.0f);
	}
}

void EngineGUI::RenderScenes()
{
	Icon* sceneIcon = ResourceManager::Get().GetResource<Icon>("MapIcon.png").get();

	auto& sceneNames = m_sceneManagerRef->GetSceneNames();

	ImVec2 size = { ImGui::GetWindowSize().x * 0.1f, ImGui::GetWindowSize().x * 0.1f };

	int id = 1;
	for (auto& scene : sceneNames)
	{
		ImGui::BeginChild(id++);
		ImGui::Image(*sceneIcon->GetShaderView(), size);
		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x * 0.1f);
		ImGui::Text(scene.c_str());
		ImGui::SameLine();
		if (ImGui::Button(("Select###sceneSelecterid" + std::to_string(id)).c_str()))
		{
			m_sceneManagerRef->SetSceneForEdit(scene);
		}
		ImGui::EndChild();
	}

}

void EngineGUI::RenderFileKeepingWindow()
{
	if (m_showFileKeeper)
	{
		ImGui::Begin("File Tab", &m_showFileKeeper, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();

		if (ImGui::Button("Save"))
		{
			if (m_sceneManagerRef)
			{
				auto& sceneNames = m_sceneManagerRef->GetSceneNames();
				for (auto& scene : sceneNames)
				{
					m_sceneManagerRef->GetScene(scene)->GetRegistry()->SaveData();
				}
				m_sceneManagerRef->SaveScenes();
			}
		}

		if (ImGui::Button("Load"))
		{
			if (m_sceneManagerRef)
			{
				m_currentEntity = recs::NULL_ENTITY;
				m_sceneManagerRef->ResetScenes();
				m_sceneManagerRef->LoadScenes();
				auto& sceneNames = m_sceneManagerRef->GetSceneNames();
				for (auto& scene : sceneNames)
				{
					m_sceneManagerRef->GetScene(scene)->GetRegistry()->LoadData();
				}
				
			}
		}

		if (ImGui::Button("Clear##ClearScenesButton"))
		{
			if (m_sceneManagerRef)
			{
				m_sceneManagerRef->ResetScenes();
			}
		}

		ImGui::EndMenuBar();

		ImGui::End();
	}
}

void EngineGUI::RenderNewComponentTab()
{
	if (m_showNewComponentTab)
	{
		ImGui::SetNextWindowPos({ ImGui::GetWindowContentRegionMax().x * 0.9f, ImGui::GetWindowSize().y * 0.05f});
		ImGui::SetNextWindowSize({ImGui::GetWindowSize().x, 0.0f});
		ImGui::Begin("Add Component##windowcomponentadd", &m_showNewComponentTab, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		auto reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();

		if (!reg->HasComponent<RigidBody>(m_currentEntity))
		{
			if (ImGui::Button("Rigidbody", { ImGui::GetWindowSize().x, 0 }))
			{
				reg->AddComponent<RigidBody>(m_currentEntity);
			}
		}

		if (!reg->HasComponent<Model>(m_currentEntity))
		{
			if (ImGui::Button("Model", { ImGui::GetWindowSize().x, 0 }))
			{
				reg->AddComponent<Model>(m_currentEntity);
				reg->AddComponent<ModelID>(m_currentEntity);
			}
		}

		if (!reg->HasComponent<Light>(m_currentEntity))
		{
			if (ImGui::Button("Light", { ImGui::GetWindowSize().x, 0 }))
			{
				reg->AddComponent<Light>(m_currentEntity);
			}
		}

		if (!reg->HasComponent<Script>(m_currentEntity))
		{
			if (ImGui::Button("Script", { ImGui::GetWindowSize().x, 0 }))
			{
				reg->AddComponent<Script>(m_currentEntity);
			}
		}

		if (!reg->HasComponent<CollisionBox>(m_currentEntity))
		{
			if (ImGui::Button("Collision Box", { ImGui::GetWindowSize().x, 0 }))
			{
				reg->AddComponent<CollisionBox>(m_currentEntity);
			}
		}

		if (!reg->HasComponent<CameraPoint>(m_currentEntity))
		{
			if (ImGui::Button("Camera Point", { ImGui::GetWindowSize().x, 0 }))
			{
				reg->AddComponent<CameraPoint>(m_currentEntity);
			}
		}


		ImGui::End();
	}
}

void EngineGUI::RenderScriptsTab()
{
	if (m_showScriptsTab)
	{
		ImGui::Begin("Scripts###WindowsScripts", &m_showScriptsTab);

		const auto& scripts = LUA.GetScriptNames();
		recs::recs_registry* reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();

		int id = 1;
		for (const auto& script : scripts)
		{
			ImGui::BeginChild(id, { 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);

			ImGui::Text(script.c_str());
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.82f);
			if (ImGui::Button(("Select###scriptselect" + std::to_string(id)).c_str(), { ImGui::GetWindowWidth() * 0.15f , 0 }))
			{
				Script* s = reg->GetComponent<Script>(m_currentEntity);
				if (s)
				{
					for (int i = 0; i < MAX_SCRIPTS; i++)
					{
						if (s->script_id[i] == NULL_SCRIPT)
						{
							s->script_id[i] = LUA.GetIdFromScriptName(script.substr(0, script.size() - 4));
							m_showScriptsTab = false;
						}
						break;
					}
				}
			}

			ImGui::EndChild();
			id++;
		}

		ImGui::End();
	}
}

void EngineGUI::RenderModelsTab()
{
	if (m_showModelsTab)
	{
		ImGui::Begin("Models##WindowModels", &m_showModelsTab, ImGuiWindowFlags_MenuBar);

		//ImGui::BeginMenuBar();

		////if (ImGui::Button("Add Model"))
		////{

		////}
		////if (ImGui::Button("Remove Model"))
		////{

		////}

		//ImGui::EndMenuBar();

		recs::recs_registry* reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();
		const auto& a = ResourceManager::Get().GetResourceMap();

		// Identification for child windows.
		int id = 1;
		for (const auto& r : a)
		{
			// Check if model3d object.
			if (dynamic_cast<Model3D*>(r.second.get()))
			{
				ImGui::BeginChild(id, { 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
				ImGui::Text(r.second->GetName().c_str());
				ImGui::SameLine();

				ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.82f);
				if (ImGui::Button(std::string("Select###" + std::to_string(id)).c_str(), {ImGui::GetWindowWidth() * 0.15f , 0}))
				{
					reg->GetComponent<Model>(m_currentEntity)->model_data = dynamic_cast<Model3D*>(r.second.get());
					reg->GetComponent<ModelID>(m_currentEntity)->model_id = r.first;
					m_showModelsTab = false;
				}
				ImGui::EndChild();

			}
			id++;
		}

		ImGui::End();
	}
}

void EngineGUI::RenderTextureTab()
{
	if (m_showTextureTab)
	{
		ImGui::Begin("Textures##WindowTextures", &m_showTextureTab, ImGuiWindowFlags_MenuBar);

		//ImGui::BeginMenuBar();

		////if (ImGui::Button("Add Texture"))
		////{

		////}
		////if (ImGui::Button("Remove Texture"))
		////{

		////}

		//ImGui::EndMenuBar();

		recs::recs_registry* reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();
		const auto& a = ResourceManager::Get().GetResourceMap();

		// Identification for child windows.
		int id = 1;
		for (const auto& r : a)
		{
			// Check if model3d object.
			if (dynamic_cast<Texture*>(r.second.get()))
			{
				ImGui::BeginChild(id, { 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
				ImGui::Text(r.second->GetName().c_str());
				ImGui::SameLine();

				ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.82f);
				if (ImGui::Button(std::string("Select###" + std::to_string(id)).c_str(), { ImGui::GetWindowWidth() * 0.15f , 0 }))
				{
					reg->GetComponent<Model>(m_currentEntity)->model_texture = dynamic_cast<Texture*>(r.second.get());
					reg->GetComponent<ModelID>(m_currentEntity)->texture_id = r.first;
					m_showTextureTab = false;
				}
				ImGui::EndChild();

			}
			id++;
		}

		ImGui::End();
	}
}

void EngineGUI::RenderScenesTab()
{
	if (m_showScenesTab)
	{
		const ImVec2 wSize = {ImGui::GetWindowSize().x * 2.0f, ImGui::GetWindowSize().y};
		ImGui::SetNextWindowSize(wSize);
		ImGui::Begin("Scenes", &m_showScenesTab);

		/*
		
			Create or Delete Scene.
		
		*/
		int id = 1;

		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Create new Scene");
		ImGui::BeginChild(id, { 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
		
		if (ImGui::InputTextWithHint("###SceneInputHandler", "Input scene name...", m_sceneInputField, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_sceneManagerRef->AddScene(std::string(m_sceneInputField));
			
			memset(m_sceneInputField, '\0', 100);
		}

		id++;
		ImGui::EndChild();

		/*
		
			Display all available Scenes.
			* Delete also available.
		*/

		ImGui::TextColored(ImVec4(255, 0, 255, 255), "List of Scenes");


		ImGui::BeginChild(id, { 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
		auto& scenes = m_sceneManagerRef->GetSceneNames();
		for (auto& scene : scenes)
		{
			ImGui::Text(scene.c_str());
			ImGui::SameLine();
			if (ImGui::Button(("Select###SceneSelectButton" + std::to_string(id)).c_str()))
			{
				m_sceneManagerRef->SetSceneForEdit(scene);
				m_currentEntity = recs::NULL_ENTITY;
			}
			ImGui::SameLine();
			if (ImGui::Button(("Delete###SceneDeleteButton" + std::to_string(id)).c_str()))
			{
				// Delete scene, move edit scene to other scene, otherwise move to blank scene.

				m_sceneManagerRef->RemoveScene(scene);
				break;

			}

			id++;
		}



		ImGui::EndChild();

		ImGui::End();
	}
}
