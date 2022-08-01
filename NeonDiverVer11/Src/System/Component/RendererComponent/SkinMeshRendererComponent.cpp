#include "SkinMeshRendererComponent.h"

SkinMeshRendererComponent::SkinMeshRendererComponent()
{
	m_name = "SkinMeshRendererComponent";
}

void SkinMeshRendererComponent::Draw()
{
	if (!m_wpModelComponent.expired() && m_wpModelComponent.lock()->GetModelWork())
	{
		m_wpAssetManager.lock()->GetSkinMeshShader(m_shaderName)->DrawModel(*m_wpModelComponent.lock()->GetModelWork(), m_wpTransformComponent.lock()->GetMatrix());
	}
}

void SkinMeshRendererComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		for (const auto& spShader : m_wpAssetManager.lock()->GetSkinMeshShaders())
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

void SkinMeshRendererComponent::PrepareJsonParameter(nlohmann::json& json)
{
	json["SkinMeshRenderer"] = m_shaderName;
}

void SkinMeshRendererComponent::ReferenceJson()
{
	std::string temp = "SkinMeshShader";
	ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_shaderName, "SkinMeshRenderer", temp);
}