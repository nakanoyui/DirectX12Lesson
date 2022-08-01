#include "ComponentFactory.h"

#include "Application/Application.h"

#include "Application/ScriptComponent/PlayerControllerComponent.h"

void ComponentFactory::Init()
{
	m_wpJsonManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager();

	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("Component").begin();
		it != m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("Component").end(); ++it)
	{
		if (it.key() == "TransformComponent")
		{
			m_spConstructs["TransformComponent"] = []() {return std::make_shared<TransformComponent>(); };
		}
		else if (it.key() == "ModelComponent")
		{
			m_spConstructs["ModelComponent"] = []() {return std::make_shared<ModelComponent>(); };
		}
		else if (it.key() == "CameraComponent")
		{
			m_spConstructs["CameraComponent"] = []() {return std::make_shared<CameraComponent>(); };
		}
		else if (it.key() == "SpriteRendererComponent")
		{
			m_spConstructs["SpriteRendererComponent"] = []() {return std::make_shared<SpriteRendererComponent>(); };
		}
		else if (it.key() == "StandardRendererComponent")
		{
			m_spConstructs["StandardRendererComponent"] = []() {return std::make_shared<StandardRendererComponent>(); };
		}
		else if (it.key() == "SkinMeshRendererComponent")
		{
			m_spConstructs["SkinMeshRendererComponent"] = []() {return std::make_shared<SkinMeshRendererComponent>(); };
		}
		else if (it.key() == "SphereColliderComponent")
		{
			m_spConstructs["SphereColliderComponent"] = []() {return std::make_shared<SphereColliderComponent>(); };
		}
		else if (it.key() == "BoxColliderComponent")
		{
			m_spConstructs["BoxColliderComponent"] = []() {return std::make_shared<BoxColliderComponent>(); };
		}
		else if (it.key() == "ModelColliderComponent")
		{
			m_spConstructs["ModelColliderComponent"] = []() {return std::make_shared<ModelColliderComponent>(); };
		}	
		else if (it.key() == "RigidBodyComponent")
		{
			m_spConstructs["RigidBodyComponent"] = []() {return std::make_shared<RigidBodyComponent>(); };
		}
	}

	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("ScriptComponent").begin();
		it != m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("ScriptComponent").end(); ++it)
	{
		if (it.key() == "PlayerControllerComponent")
		{
			m_spScriptConstructs["PlayerControllerComponent"] = []() {return std::make_shared<PlayerControllerComponent>(); };
		}
	}
}