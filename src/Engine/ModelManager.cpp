#include "Header.h"
#include "ModelManager.h"
#include "ResourceManager.h"

ModelManager::ModelManager()
{
	m_visible = false;
}

ModelManager::~ModelManager()
{
}

void ModelManager::Draw()
{
	if (this->m_visible)
		this->InnerDraw();
}

void ModelManager::Update()
{

}

void ModelManager::InnerDraw()
{
	ImGui::Begin("Model Manager", &m_visible);

	const auto& a = ResourceManager::Get().GetResourceMap();

	int id = 1;
	ImGui::TextColored(ImVec4(255, 0, 255, 255), "List of Models");
	for (const auto& r : a)
	{
		if (dynamic_cast<Model3D*>(r.second.get()))
		{
			ImGui::BeginChild(id, { 0, 0 }, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
			ImGui::Text(r.second->GetName().c_str());
			ImGui::EndChild();
		}

		id++;
	}

	ImGui::End();
}
