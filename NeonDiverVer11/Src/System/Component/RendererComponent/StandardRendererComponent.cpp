#include "StandardRendererComponent.h"

StandardRendererComponent::StandardRendererComponent()
{
	m_name = "StandardRendererComponent";
}

void StandardRendererComponent::Draw()
{
	if (!m_wpModelComponent.expired() && m_wpModelComponent.lock()->GetModelWork())
	{
		m_wpAssetManager.lock()->GetStandardShader(m_shaderName)->DrawModel(*m_wpModelComponent.lock()->GetModelWork(), m_wpTransformComponent.lock()->GetMatrix());
	}
}

void StandardRendererComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		for (const auto& spShader : m_wpAssetManager.lock()->GetStandardShaders())
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

void StandardRendererComponent::PrepareJsonParameter(nlohmann::json& json)
{
	json["StandardRenderer"] = m_shaderName;
}

void StandardRendererComponent::ReferenceJson()
{
	std::string temp = "StandardShader";
	ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_shaderName, "StandardRenderer", temp);
}