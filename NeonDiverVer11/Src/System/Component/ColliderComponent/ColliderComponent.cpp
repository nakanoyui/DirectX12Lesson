#include "ColliderComponent.h"

#include "Application/Application.h"

ColliderComponent::ColliderComponent()
{
	m_name = "ColliderComponent";
}

void ColliderComponent::Init()
{
	m_wpAssetManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetAssetManager();
	m_wpDrawManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetDrawManager();
}

void ColliderComponent::Start()
{
	m_wpTransformComponent = m_wpGameObject.lock()->GetComponent<TransformComponent>();
}

void ColliderComponent::DrawLate()
{
	if (m_upDebugWireFrame)
	{
		m_wpAssetManager.lock()->GetStandardShader("EffectShader")->DrawEffect(m_upDebugWireFrame->GetMesh().get(), Math::Matrix::Identity);
	}
}