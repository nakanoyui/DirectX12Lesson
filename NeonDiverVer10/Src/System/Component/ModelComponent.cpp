#include "ModelComponent.h"

#include "Application/Application.h"

#include "../Manager/ImGuiManager/ImGuiHelper.h"

ModelComponent::ModelComponent()
{
	m_name = "ModelComponent";
}

void ModelComponent::Init()
{
	m_wpAssetManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetAssetManager();

	ReferenceJson();

	m_wpTransformComponent = m_pGameObject->GetComponent<TransformComponent>();
}

void ModelComponent::Draw()
{
	if (m_spModelWork)
	{
		m_wpAssetManager.lock()->GetShader(m_shaderName)->DrawModel(*m_spModelWork, m_wpTransformComponent.lock()->GetMatrix());
	}
}

void ModelComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		ImGui::Text("FilePath");
		char* buf =  m_filePath.data();
		ImGui::InputText(" ", buf, 64);
		ImGui::Separator();
		m_filePath = buf;

		for (const auto& spShader : m_wpAssetManager.lock()->GetShaders())
		{
			if (ImGui::RadioButton(spShader.first.c_str(), spShader.first == m_shaderName ? true : false))
			{
				m_shaderName = spShader.first;
			}
			ImGui::Separator();
		}

		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void ModelComponent::ReferenceJson()
{
	std::string temp = "StandardShader";
	ImGuiHelper::CheckData(m_pGameObject->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(),m_shaderName, "Shader", temp);

	temp = "";
	ImGuiHelper::CheckData(m_pGameObject->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_filePath, "FilePath", temp);

	m_spModelWork = nullptr;

	m_spModelWork = m_wpAssetManager.lock()->GetModelWork(m_filePath);
}

void ModelComponent::PrepareJsonParameter(nlohmann::json& json)
{
	json["Shader"] = m_shaderName;
	json["FilePath"] = m_filePath;
}