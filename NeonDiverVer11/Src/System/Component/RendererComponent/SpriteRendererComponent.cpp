#include "SpriteRendererComponent.h"

SpriteRendererComponent::SpriteRendererComponent()
{
	m_name = "SpriteRendererComponent";
}

void SpriteRendererComponent::Draw()
{
	if (!m_wpModelComponent.expired() && m_wpModelComponent.lock()->GetModelWork())
	{
		m_wpAssetManager.lock()->GetSpriteShader(m_shaderName)->DrawModel(*m_wpModelComponent.lock()->GetModelWork(), m_wpTransformComponent.lock()->GetMatrix());
	}
}

void SpriteRendererComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		for (const auto& spShader : m_wpAssetManager.lock()->GetSpriteShaders())
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

void SpriteRendererComponent::PrepareJsonParameter(nlohmann::json& json)
{
	json["SpriteRenderer"] = m_shaderName;
}

void SpriteRendererComponent::ReferenceJson()
{
	std::string temp = "SpriteShader";
	ImGuiHelper::CheckData(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(), m_shaderName, "SpriteRenderer", temp);
}