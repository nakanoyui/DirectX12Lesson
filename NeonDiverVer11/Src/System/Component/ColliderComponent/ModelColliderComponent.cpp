#include "ModelColliderComponent.h"

ModelColliderComponent::ModelColliderComponent()
{
	m_name = "ModelColliderComponent";
}

void ModelColliderComponent::Init()
{
	ColliderComponent::Init();
}

void ModelColliderComponent::Start()
{
	ColliderComponent::Start();

	m_wpModelComponent = m_wpGameObject.lock()->GetComponent<ModelComponent>();
}

void ModelColliderComponent::ImGuiUpdateInspecter()
{
	if (ImGui::TreeNode(m_name.c_str()))
	{
		if (ImGui::Button("Reset"))
		{
			ReferenceJson();
		}
		ImGui::Separator();

		ImGui::TreePop();
	}
}

void ModelColliderComponent::PrepareJsonParameter(nlohmann::json& json)
{
}

void ModelColliderComponent::ReferenceJson()
{
}

void ModelColliderComponent::CreateCollision()
{
	m_upCollider = nullptr;

	m_upCollider = std::make_unique<Collider>(m_wpTransformComponent.lock());

	m_upCollider->AddCollisionShape(std::make_shared<ModelCollision>(m_wpModelComponent.lock()->GetModelWork(), Collider::Type::TypeBump));
}