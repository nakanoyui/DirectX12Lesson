#include "RigidBodyComponent.h"

#include "Application/Application.h"

RigidBodyComponent::RigidBodyComponent()
{
	m_name = "RigidBodyComponent";
}

void RigidBodyComponent::Init()
{
	
}

void RigidBodyComponent::Start()
{
	m_wpMyColliderComponent = m_wpGameObject.lock()->GetComponent<SphereColliderComponent>();
}

void RigidBodyComponent::Update()
{
	m_wpTargetColliderComponent.clear();
	m_bumpResults.clear();

	for (const auto& spGameObject : Application::Instance().GetSceneManager()->GetCurrentScene()->GetGameObjects())
	{
		auto spCollider = spGameObject->GetComponent<ColliderComponent>();

		if (spCollider)
		{
			m_wpTargetColliderComponent.emplace_back(spCollider);
		}
	}

	for (const auto& wpCollider : m_wpTargetColliderComponent)
	{
		std::list<Collider::CollisionResult> collisionResults;

		if (wpCollider.lock()->GetCollider()->Intersects(Collider::Type::TypeBump, m_wpMyColliderComponent.lock()->GetBoundingSphere(), &collisionResults))
		{
			m_bumpResults[wpCollider.lock()->GetGameObject()->GetName()] = collisionResults;
		}
		else
		{
			m_bumpResults[wpCollider.lock()->GetGameObject()->GetName()] = collisionResults;
		}
	}
}

void RigidBodyComponent::ImGuiUpdateInspecter()
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

void RigidBodyComponent::PrepareJsonParameter(nlohmann::json& json)
{
}

void RigidBodyComponent::ReferenceJson()
{
}