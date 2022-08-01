#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	m_name = "TransformComponent";
}

void TransformComponent::Init()
{
	ReferenceJson();
}

void TransformComponent::Update()
{
	m_transform.MatrixComposition();
}

void TransformComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		m_transform.Positon(ImGuiHelper::InputFloat3("Position", Math::Vector3(m_transform.Positon())));

		m_transform.RotationEuler(ImGuiHelper::SliderFloat3("Rotation", m_euler, static_cast<float>(-M_PI * ToDegrees), static_cast<float>(M_PI * ToDegrees)));

		m_transform.Scale(ImGuiHelper::InputFloat3("Scale", Math::Vector3(m_transform.Scale())));

		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void TransformComponent::PrepareJsonParameter(nlohmann::json& json)
{
	auto array = nlohmann::json::array();

	array.clear();
	array.push_back(m_transform.Positon().x);
	array.push_back(m_transform.Positon().y);
	array.push_back(m_transform.Positon().z);
	json["Position"] = array;
	array.clear();
	array.push_back(m_euler.x);
	array.push_back(m_euler.y);
	array.push_back(m_euler.z);
	json["Rotation"] = array;
	array.clear();
	array.push_back(m_transform.Scale().x);
	array.push_back(m_transform.Scale().y);
	array.push_back(m_transform.Scale().z);
	json["Scale"] = array;
	array.clear();
}

void TransformComponent::ReferenceJson()
{
	m_transform.Positon(ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(),
		Math::Vector3(m_transform.Positon().x, m_transform.Positon().y, m_transform.Positon().z), "Position", Math::Vector3()));

	m_transform.RotationEuler(ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(),
		m_euler, "Rotation", Math::Vector3()));

	m_transform.Scale(ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(),
		Math::Vector3(m_transform.Scale().x, m_transform.Scale().y, m_transform.Scale().z), "Scale", Math::Vector3(Constant::One_F)));
}