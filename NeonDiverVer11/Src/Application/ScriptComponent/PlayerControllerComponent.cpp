#include "PlayerControllerComponent.h"

#include "Application/Application.h"

PlayerControllerComponent::PlayerControllerComponent():
	m_amountOfMovement(1)
{
	m_name = "PlayerControllerComponent";
}

void PlayerControllerComponent::Init()
{
	ReferenceJson();

	m_wpKeyManager = Application::Instance().GetKeyManager();
	m_wpCameraManager = Application::Instance().GetCameraManager();
}

void PlayerControllerComponent::Start()
{
	m_wpTransformComponent = m_wpGameObject.lock()->GetComponent<TransformComponent>();
}

void PlayerControllerComponent::Update()
{
	m_velocity = Math::Vector3::Zero;

	if (m_wpKeyManager.lock()->GetKeyState('W', true))
	{
		m_velocity.z = Constant::One_F;
	}

	if (m_wpKeyManager.lock()->GetKeyState('S', true))
	{
		m_velocity.z = -Constant::One_F;
	}

	if (m_wpKeyManager.lock()->GetKeyState('D', true))
	{
		m_velocity.x = Constant::One_F;
	}

	if (m_wpKeyManager.lock()->GetKeyState('A', true))
	{
		m_velocity.x = -Constant::One_F;
	}

	m_velocity = Math::Vector3::TransformNormal(m_velocity, m_wpCameraManager.lock()->GetNowCamera()->GetRotationYMatrix());

	m_velocity *= m_amountOfMovement;

	m_wpTransformComponent.lock()->SetPos(m_wpTransformComponent.lock()->GetPos() + m_velocity);
}

void PlayerControllerComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		ImGui::InputFloat("AmountOfMovement", &m_amountOfMovement);

		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void PlayerControllerComponent::PrepareJsonParameter(nlohmann::json& json)
{
	auto array = nlohmann::json::array();

	array.clear();
	array.push_back(m_velocity.x);
	array.push_back(m_velocity.y);
	array.push_back(m_velocity.z);
	json["Velocity"] = array;
}

void PlayerControllerComponent::ReferenceJson()
{
	m_velocity = ImGuiHelper::CheckDataVector3(m_wpGameObject.lock()->GetJsonManager().lock()->GetJson(m_objectName)->WorkConstJson(),
		m_velocity, "Velocity", Math::Vector3(Constant::One_F));
}