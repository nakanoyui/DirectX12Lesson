#include "ComponentFactory.h"

#include "Application/Application.h"

void ComponentFactory::Init()
{
	m_wpJsonManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager();

	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->WorkGeneralJson().at("Component").begin();
		it != m_wpJsonManager.lock()->WorkGeneralJson().at("Component").end(); ++it)
	{
		if (it.key() == "TransformComponent")
		{
			m_spConstructs["TransformComponent"] = []() {return std::make_shared<TransformComponent>(); };
		}
		else if (it.key() == "ModelComponent")
		{
			m_spConstructs["ModelComponent"] = []() {return std::make_shared<ModelComponent>(); };
		}
	}
}