#include "ModelComponent.h"

#include "Application/Application.h"

ModelComponent::ModelComponent()
{
	m_name = "ModelComponent";
}

void ModelComponent::Init()
{
	m_wpAssetManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetAssetManager();

	ReferenceJson();
}

void ModelComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		ImGui::InputText("FilePath", m_filePath, ImGuiHelper::InputTextSize);
		ImGui::Separator();

		if (!m_spModelWork)
		{
			m_spModelWork = m_wpAssetManager.lock()->GetModelWork(m_filePath);
		}

		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void ModelComponent::PrepareJsonParameter(nlohmann::json& json)
{
	json["FilePath"] = std::string(m_filePath);
}

void ModelComponent::ReferenceJson()
{
	std::string temp = "";
	auto str = ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), std::string(m_filePath), "FilePath", temp);

	std::copy(str.begin(), str.end(), m_filePath);

	m_spModelWork = nullptr;

	m_spModelWork = m_wpAssetManager.lock()->GetModelWork(m_filePath);
}