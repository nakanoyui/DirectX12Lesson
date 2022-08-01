#include "RendererComponent.h"

#include "Application/Application.h"

RendererComponent::RendererComponent()
{
	m_name = "RendererComponent";
}

void RendererComponent::Init()
{
	m_wpAssetManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetAssetManager();
	m_wpDrawManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetDrawManager();

	ReferenceJson();
}

void RendererComponent::Start()
{
	m_wpTransformComponent = m_wpGameObject.lock()->GetComponent<TransformComponent>();
	m_wpModelComponent = m_wpGameObject.lock()->GetComponent<ModelComponent>();
}

void RendererComponent::Update()
{
	m_wpDrawManager.lock()->GetDrawList(m_shaderName)->AddDrawList(m_wpGameObject.lock());
}