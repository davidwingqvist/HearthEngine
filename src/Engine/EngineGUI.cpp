#include "header.h"
#include "EngineGUI.h"
#include "D3D11Context.h"
#include "Debugger.h"
#include "LuaState.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Time.h"
#include "Icon.h"

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

void EngineGUI::RenderTopBar()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(D3D11Core::Get().GetWindow()->GetWidth(), 0));
	
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

	this->RenderProperties();
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

	if (ImGui::Button("Statistics", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 10.0f }))
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
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowSize().x * 0.6f, ImGui::GetWindowSize().y * 1.15f));
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

/*

	Huge function that checks for each individual component and
	allows editing through ImGUI API.
*/
void EngineGUI::RenderProperties()
{
	if (m_showPropertiesTab)
	{
		ImGui::Begin("Properties", &m_showPropertiesTab, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		recs::recs_registry* reg = m_sceneManagerRef->GetCurrentScene()->GetRegistry();

		if (!reg)
		{
			ImGui::End();
			return;
		}

		GameObject* currGameObject = reg->GetComponent<GameObject>(m_currentEntity);

		if (currGameObject)
		{
			ImGui::BeginChild(1, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "GameObject");
			std::string gameObjectName = "Name: " + std::string(currGameObject->name);
			ImGui::Text(gameObjectName.c_str());
			char input[28] = { '\0' };
			std::string name = "###PropInputName";
			if (ImGui::InputTextWithHint(name.c_str(), std::string("Input new name").c_str(), input, sizeof input, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::strncpy(currGameObject->name, input, 28);
			}
			ImGui::EndChild();
		}

		Transform* currTransform = reg->GetComponent<Transform>(m_currentEntity);
		if (currTransform)
		{
			ImGui::BeginChild(3, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Transform");
			ImGui::BeginGroup();
			ImGui::Text("Position");
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 3.6f);
			ImGui::DragFloat("x###Posx", &currTransform->pos.x, 1, 10, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 3.6f);
			ImGui::DragFloat("y###Posy", &currTransform->pos.y, 1, 10, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 3.6f);
			ImGui::DragFloat("z###Posz", &currTransform->pos.z, 1, 10, 0.0f, "%.2f");

			ImGui::Text("Rotation");
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("x###Rotx", &currTransform->rotation.x, -3.1415, 3.1415, "%.3f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("y###Roty", &currTransform->rotation.y, -3.1415, 3.1415, "%.3f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::SliderFloat("z###Rotz", &currTransform->rotation.z, -3.1415, 3.1415, "%.3f");

			ImGui::Text("Scale");
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::DragFloat("x###Scalex", &currTransform->scale.x, 0.1, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::DragFloat("y###Scaley", &currTransform->scale.y, 0.1, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionMax().x / 4.0f);
			ImGui::DragFloat("z###Scalez", &currTransform->scale.z, 0.1, 0.0f, 0.0f, "%.2f");
			ImGui::EndGroup();

			//ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
			//if (ImGui::Button("Delete###transformdelete"))
			//{
			//	reg->RemoveComponent<Transform>(m_currentEntity);
			//}

			ImGui::EndChild();
		}

		Model* currModel = reg->GetComponent<Model>(m_currentEntity);
		if (currModel)
		{
			ImGui::BeginChild(2, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Model");
			std::string modelName = "Current Model: None";

			if (currModel->model_data)
			{
				modelName = "Current Model: " + currModel->model_data->GetName();
			}

			ImGui::Text(modelName.c_str());
			if (ImGui::Button("..##ModelTabButton"))
			{
				m_showModelsTab = true;
			}
			ImGui::SameLine();

			if (ImGui::InputTextWithHint("###Insertmodelname", "Insert Model Name", m_modelinputField, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				Model3D* newModel = ResourceManager::Get().GetResource<Model3D>(std::string(m_modelinputField)).get();
				if (newModel)
				{
					currModel->model_data = newModel;

					reg->GetComponent<ModelID>(m_currentEntity)->model_id = ResourceManager::Get().GetHashCode(m_modelinputField);
				}

				memset(m_modelinputField, 0, 100);
			}

			std::string textureName = "Current Texture: None";
			if (currModel->model_texture)
			{
				textureName = "Current Texture: " + currModel->model_texture->GetName();
			}
			ImGui::Text(textureName.c_str());

			if (ImGui::Button("..##TextureTabButton"))
			{
				m_showTextureTab = true;
			}
			
			ImGui::SameLine();

			if (ImGui::InputTextWithHint("###Inserttexturename", "Insert Texture Name", m_textureInputField, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				Texture* newTexture = ResourceManager::Get().GetResource<Texture>(std::string(m_textureInputField)).get();
				if (newTexture)
				{
					currModel->model_texture = newTexture;

					reg->GetComponent<ModelID>(m_currentEntity)->texture_id = ResourceManager::Get().GetHashCode(m_textureInputField);
				}

				memset(m_textureInputField, 0, 100);
			}

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
			if (ImGui::Button("Delete###modeldelete"))
			{
				reg->RemoveComponent<Model>(m_currentEntity);
				reg->RemoveComponent<ModelID>(m_currentEntity);
			}

			ImGui::EndChild();
		}

		RigidBody* rigidBody = reg->GetComponent<RigidBody>(m_currentEntity);

		if (rigidBody)
		{
			ImGui::BeginChild(4, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Rigidbody");
			ImGui::DragFloat("Mass", &rigidBody->mass, 1, 0, 1000);
			ImGui::Checkbox("Collision?", &rigidBody->hasCollision);
			ImGui::Checkbox("Gravity?", &rigidBody->hasGravity);

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
			if (ImGui::Button("Delete###rigidbodydelete"))
			{
				reg->RemoveComponent<RigidBody>(m_currentEntity);
			}

			ImGui::EndChild();
		}

		Light* currLight = reg->GetComponent<Light>(m_currentEntity);

		if (currLight)
		{
			ImGui::BeginChild(5, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Light");

			if (ImGui::Button("Apply Updates###lightUpdates"))
			{
				m_lightPassRef->ForceLightUpdate();
			}

			ImGui::Text("Ambient");
			ImGui::SliderFloat4("###AmbientInput", (float*)&currLight->ambient, 0.0f, 1.0f, "%.2f");
			ImGui::Text("Diffuse");
			ImGui::SliderFloat4("###DiffuseInput", (float*)&currLight->diffuse, 0.0f, 1.0f, "%.2f");
			ImGui::Text("Specular");
			ImGui::SliderFloat4("###SpecularInput", (float*)&currLight->specular, 0.0f, 1.0f, "%.2f");
			if (ImGui::Button("Directional##LightType"))
				currLight->type = LIGHTTYPE::DIRECTIONAL;
			ImGui::SameLine();
			if (ImGui::Button("PointLight##LightType"))
				currLight->type = LIGHTTYPE::POINTLIGHT;
			switch (currLight->type)
			{
			case LIGHTTYPE::DIRECTIONAL:
				ImGui::Text("Direction");
				break;
			default:
				ImGui::Text("No type selected...");
				break;
			}
			
			ImGui::InputFloat3("###TypeSpecificInput", (float*)&currLight->data, "%.2f");
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
			
			if (ImGui::Button("Delete###lightdelete"))
			{
				reg->RemoveComponent<Light>(m_currentEntity);
			}
			ImGui::EndChild();
		}

		Script* currScripts = reg->GetComponent<Script>(m_currentEntity);

		if (currScripts)
		{
			
			ImGui::BeginChild(6, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Scripts");
			std::string scriptId = "";

			if (ImGui::Button("..##ScriptSelectWindow"))
				m_showScriptsTab = !m_showScriptsTab;

			ImGui::SameLine();

			if (ImGui::InputTextWithHint("###InputScriptNameProps", "Input script name", m_createScriptPathProp, sizeof m_createScriptPathProp, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (LUA.LookUpScript(m_createScriptPathProp))
				{
					for (int i = 0; i < MAX_SCRIPTS; i++)
					{
						if (currScripts->script_id[i] == NULL_SCRIPT)
						{
							currScripts->script_id[i] = LUA.GetIdFromScriptName(m_createScriptPathProp);
							break;
						}
					}
				}
				memset(m_createScriptPathProp, '\0', sizeof m_createScriptPathProp);
			}

			for (int i = 0; i < MAX_SCRIPTS; i++)
			{
				if (currScripts->script_id[i] != NULL_SCRIPT)
				{
					ImGui::Text(LUA.GetScriptNameFromId(currScripts->script_id[i]).c_str());
					ImGui::SameLine();
					if (ImGui::Button(("Remove###RemoveCurrentScript" + std::to_string(i)).c_str()))
					{
						currScripts->script_id[i] = NULL_SCRIPT;
					}
					ImGui::SameLine();
					if (ImGui::Button(("Open###OpenCurrentScript" + std::to_string(i)).c_str()))
					{
						LUA.OpenScriptFile(LUA.GetScriptNameFromId(currScripts->script_id[i]).c_str(), true);
					}
				}
			}


			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
			if (ImGui::Button("Delete###scriptdelete"))
			{
				reg->RemoveComponent<Script>(m_currentEntity);
			}

			ImGui::EndChild();
			
		}

		CollisionBox* currCollBox = reg->GetComponent<CollisionBox>(m_currentEntity);

		if (currCollBox)
		{
			ImGui::BeginChild(7, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
		
			ImGui::TextColored(ImVec4(255, 0, 255, 255), "Collision Box");

			ImGui::DragFloat("Size###SizeOfCollisionBoxDrag", &currCollBox->size, 0.3f, 0.0f, 0.0f, "%.1f");

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
			if (ImGui::Button("Delete###cbdelete"))
			{
				reg->RemoveComponent<CollisionBox>(m_currentEntity);
			}

			ImGui::EndChild();
		}

		if (m_currentEntity != recs::NULL_ENTITY)
		{
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.35f));
			if (ImGui::Button("+ Add Component"))
				m_showNewComponentTab = !m_showNewComponentTab;
		}

		ImGui::End();
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


		if (ImGui::Button("Rigidbody", { ImGui::GetWindowSize().x, 0 }))
		{
			reg->AddComponent<RigidBody>(m_currentEntity);
		}
		if (ImGui::Button("Model", { ImGui::GetWindowSize().x, 0 }))
		{
			reg->AddComponent<Model>(m_currentEntity);
			reg->AddComponent<ModelID>(m_currentEntity);
		}
		if (ImGui::Button("Light", { ImGui::GetWindowSize().x, 0 }))
		{
			reg->AddComponent<Light>(m_currentEntity);
		}
		if (ImGui::Button("Script", { ImGui::GetWindowSize().x, 0 }))
		{
			reg->AddComponent<Script>(m_currentEntity);
		}
		if (ImGui::Button("Collision Box", { ImGui::GetWindowSize().x, 0 }))
		{
			reg->AddComponent<CollisionBox>(m_currentEntity);
		}
		if (ImGui::Button("Camera Point", { ImGui::GetWindowSize().x, 0 }))
		{
			// To be implemented..
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

		ImGui::BeginMenuBar();

		if (ImGui::Button("Add Model"))
		{

		}
		if (ImGui::Button("Remove Model"))
		{

		}

		ImGui::EndMenuBar();

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

		ImGui::BeginMenuBar();

		if (ImGui::Button("Add Texture"))
		{

		}
		if (ImGui::Button("Remove Texture"))
		{

		}

		ImGui::EndMenuBar();

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
