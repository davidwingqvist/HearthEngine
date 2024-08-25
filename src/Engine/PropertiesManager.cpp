#include "Header.h"
#include "PropertiesManager.h"
#include "EngineGUI.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "LuaState.h"
#include "D3D11Context.h"

/*
	Huge function that checks for each individual component and
	allows editing through ImGUI API.
*/
void PropertiesManager::InnerDraw()
{
	const ImVec2 winPos = {
		(float)WINDOW->GetWidth() - m_winWidth,
		(float)WINDOW->GetHeight() * 0.025f
	};

	ImGui::SetNextWindowPos(winPos);
	ImGui::SetNextWindowSize({ m_winWidth, m_winHeight });

	ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	recs::recs_registry* reg = EngineGUI::Get().m_sceneManagerRef->GetCurrentScene()->GetRegistry();
	//m_dragPointers.SetRegistry(reg);

	if (!reg)
	{
		ImGui::End();
		return;
	}

	GameObject* currGameObject = reg->GetComponent<GameObject>(EngineGUI::Get().m_currentEntity);

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

	Transform* currTransform = reg->GetComponent<Transform>(EngineGUI::Get().m_currentEntity);
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

		if (ImGui::Button("Reset###ResetTransform"))
		{
			currTransform->pos = { 0.0f, 0.0f, 0.0f };
			currTransform->rotation = { 0.0f, 0.0f, 0.0f };
			currTransform->scale = { 1.0f, 1.0f, 1.0f };
		}
		ImGui::EndGroup();

		//ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
		//if (ImGui::Button("Delete###transformdelete"))
		//{
		//	reg->RemoveComponent<Transform>(m_currentEntity);
		//}

		ImGui::EndChild();
	}

	Model* currModel = reg->GetComponent<Model>(EngineGUI::Get().m_currentEntity);
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
			EngineGUI::Get().m_showModelsTab = true;
		}
		ImGui::SameLine();

		if (ImGui::InputTextWithHint("###Insertmodelname", "Insert Model Name", EngineGUI::Get().m_modelinputField, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Model3D* newModel = ResourceManager::Get().GetResource<Model3D>(std::string(EngineGUI::Get().m_modelinputField)).get();
			if (newModel)
			{
				currModel->model_data = newModel;

				reg->GetComponent<ModelID>(EngineGUI::Get().m_currentEntity)->model_id = ResourceManager::Get().GetHashCode(EngineGUI::Get().m_modelinputField);
			}

			memset(EngineGUI::Get().m_modelinputField, 0, 100);
		}

		std::string textureName = "Current Texture: None";
		if (currModel->model_texture)
		{
			textureName = "Current Texture: " + currModel->model_texture->GetName();
		}
		ImGui::Text(textureName.c_str());

		if (ImGui::Button("..##TextureTabButton"))
		{
			EngineGUI::Get().m_showTextureTab = true;
		}

		ImGui::SameLine();

		if (ImGui::InputTextWithHint("###Inserttexturename", "Insert Texture Name", EngineGUI::Get().m_textureInputField, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Texture* newTexture = ResourceManager::Get().GetResource<Texture>(std::string(EngineGUI::Get().m_textureInputField)).get();
			if (newTexture)
			{
				currModel->model_texture = newTexture;

				reg->GetComponent<ModelID>(EngineGUI::Get().m_currentEntity)->texture_id = ResourceManager::Get().GetHashCode(EngineGUI::Get().m_textureInputField);
			}

			memset(EngineGUI::Get().m_textureInputField, 0, 100);
		}

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
		if (ImGui::Button("Delete###modeldelete"))
		{
			reg->RemoveComponent<Model>(EngineGUI::Get().m_currentEntity);
			reg->RemoveComponent<ModelID>(EngineGUI::Get().m_currentEntity);
		}

		ImGui::EndChild();
	}

	RigidBody* rigidBody = reg->GetComponent<RigidBody>(EngineGUI::Get().m_currentEntity);

	if (rigidBody)
	{
		ImGui::BeginChild(4, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Rigidbody");
		ImGui::DragFloat("Mass", &rigidBody->mass, 1, 1, 1000);
		ImGui::SliderFloat("Bounciness", &rigidBody->bounciness, 0.01, 1, "%.2f");
		ImGui::Checkbox("isStatic?", &rigidBody->isStatic);
		ImGui::Checkbox("Gravity?", &rigidBody->hasGravity);

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
		if (ImGui::Button("Delete###rigidbodydelete"))
		{
			reg->RemoveComponent<RigidBody>(EngineGUI::Get().m_currentEntity);
		}

		ImGui::EndChild();
	}

	Light* currLight = reg->GetComponent<Light>(EngineGUI::Get().m_currentEntity);

	if (currLight)
	{
		ImGui::BeginChild(5, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Light");

		if (ImGui::Button("Apply Updates###lightUpdates"))
		{
			EngineGUI::Get().m_lightPassRef->ForceLightUpdate();
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
			reg->RemoveComponent<Light>(EngineGUI::Get().m_currentEntity);
		}
		ImGui::EndChild();
	}

	Script* currScripts = reg->GetComponent<Script>(EngineGUI::Get().m_currentEntity);

	if (currScripts)
	{

		ImGui::BeginChild(6, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Scripts");
		std::string scriptId = "";

		if (ImGui::Button("..##ScriptSelectWindow"))
			EngineGUI::Get().m_showScriptsTab = !EngineGUI::Get().m_showScriptsTab;

		ImGui::SameLine();

		if (ImGui::InputTextWithHint("###InputScriptNameProps", "Input script name", EngineGUI::Get().m_createScriptPathProp, sizeof EngineGUI::Get().m_createScriptPathProp, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (LUA.LookUpScript(EngineGUI::Get().m_createScriptPathProp))
			{
				for (int i = 0; i < MAX_SCRIPTS; i++)
				{
					if (currScripts->script_id[i] == NULL_SCRIPT)
					{
						currScripts->script_id[i] = LUA.GetIdFromScriptName(EngineGUI::Get().m_createScriptPathProp);
						break;
					}
				}
			}
			memset(EngineGUI::Get().m_createScriptPathProp, '\0', sizeof EngineGUI::Get().m_createScriptPathProp);
		}

		for (int i = 0; i < MAX_SCRIPTS; i++)
		{
			if (currScripts->script_id[i] != NULL_SCRIPT)
			{
				ImGui::Text(LUA.GetScriptNameFromId(currScripts->script_id[i]).c_str());
				ImGui::SameLine();
				if (ImGui::Button(("Open###OpenCurrentScript" + std::to_string(i)).c_str()))
				{
					LUA.OpenScriptFile(LUA.GetScriptNameFromId(currScripts->script_id[i]).c_str(), true);
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove###RemoveCurrentScript" + std::to_string(i)).c_str()))
				{
					currScripts->script_id[i] = NULL_SCRIPT;
				}
			}
		}


		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
		if (ImGui::Button("Delete###scriptdelete"))
		{
			reg->RemoveComponent<Script>(EngineGUI::Get().m_currentEntity);
		}

		ImGui::EndChild();

	}

	CollisionBox* currCollBox = reg->GetComponent<CollisionBox>(EngineGUI::Get().m_currentEntity);

	if (currCollBox)
	{
		ImGui::BeginChild(7, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Collision Box");

		ImGui::DragFloat("Width(x)###WidthOfCollisionBoxDrag", &currCollBox->width, 0.3f, 0.1f, 100.0f, "%.1f");
		ImGui::DragFloat("Height(y)###HeightOfCollisionBoxDrag", &currCollBox->height, 0.3f, 0.1f, 100.0f, "%.1f");
		ImGui::DragFloat("Length(z)###LengthOfCollisionBoxDrag", &currCollBox->length, 0.3f, 0.1f, 100.0f, "%.1f");

		ImGui::Checkbox("Active?###collboxActivity", &currCollBox->isActive);

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
		if (ImGui::Button("Delete###cbdelete"))
		{
			reg->RemoveComponent<CollisionBox>(EngineGUI::Get().m_currentEntity);
		}

		ImGui::EndChild();
	}

	CameraPoint* currCameraPoint = reg->GetComponent<CameraPoint>(EngineGUI::Get().m_currentEntity);

	if (currCameraPoint)
	{
		ImGui::BeginChild(8, ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);

		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Camera Point");

		ImGui::InputInt("Camera ID", &currCameraPoint->id, 1, 100);

		if (ImGui::Checkbox("Active Camera?##SetCurrentActiveCameraBox", &currCameraPoint->currentActive))
		{
			if (currCameraPoint->currentActive)
			{
				EngineGUI::Get().m_sceneManagerRef->GetCurrentGameScene()->SetCameraPoint(currTransform, currCameraPoint);
			}
			else
			{
				EngineGUI::Get().m_sceneManagerRef->GetCurrentGameScene()->SetCameraPoint(nullptr, nullptr);
			}
		}

		ImGui::TextColored(ImVec4(255, 0, 255, 255), "Target");
		ImGui::DragFloat("X", &currCameraPoint->target.x, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Y", &currCameraPoint->target.y, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Z", &currCameraPoint->target.z, 1.0f, 0.0f, 0.0f, "%.2f");

		if (ImGui::Button("Update##UpdateVariablesofCamera"))
		{
			EngineGUI::Get().m_sceneManagerRef->GetCurrentGameScene()->UpdateCamera();
		}

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.45f));
		if (ImGui::Button("Delete###cameradelete"))
		{
			reg->RemoveComponent<CameraPoint>(EngineGUI::Get().m_currentEntity);
		}

		ImGui::EndChild();
	}

	if (EngineGUI::Get().m_currentEntity != recs::NULL_ENTITY)
	{
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.35f));
		if (ImGui::Button("+ Add Component"))
			EngineGUI::Get().m_showNewComponentTab = !EngineGUI::Get().m_showNewComponentTab;
	}

	ImGui::End();
}

PropertiesManager::PropertiesManager()
{
	m_winHeight = WINDOW->GetHeight() * 0.65f;
	m_winWidth = WINDOW->GetWidth() * 0.20f;
}

void PropertiesManager::Draw()
{
	this->InnerDraw();
}

void PropertiesManager::Update()
{
	m_winHeight = WINDOW->GetHeight() * 0.65f;
	m_winWidth = WINDOW->GetWidth() * 0.20f;
}
