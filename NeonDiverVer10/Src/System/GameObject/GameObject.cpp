#include "GameObject.h"

#include "Application/Application.h"

#include "System/Component/ComponentFactory.h"

GameObject::GameObject(const std::string_view& name, const std::string_view& tag) :
	m_name(name), m_tag(tag), m_bEnable(true), m_bShowInspecter(false)
{
}

void GameObject::Init()
{
	m_wpJsonManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager();
	m_wpComponentFactory = Application::Instance().GetSceneManager()->GetCurrentScene()->GetComponentFactory();

	ReferenceJson(false);

	bool bTransformComponent = false;
	for (const auto& spComponent : m_spComponents)
	{
		if ("TransformComponent" == spComponent->GetName())
		{
			bTransformComponent = true;
		}
	}

	if (!bTransformComponent)
	{
		AddComponent("TransformComponent", m_name, false);
	}

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->Init();
	}
}

void GameObject::Update()
{
	for (const auto& spComponent : m_spComponents)
	{
		spComponent->Update();
	}
}

void GameObject::Draw()
{
	for (const auto& spComponent : m_spComponents)
	{
		spComponent->Draw();
	}
}

void GameObject::DrawLate()
{
	for (const auto& spComponent : m_spComponents)
	{
		spComponent->DrawLate();
	}
}

void GameObject::ImGuiUpdateHierarchy()
{
	if (ImGui::Button(m_name.c_str()))
	{
		for (const auto& spGameObject : Application::Instance().GetSceneManager()->GetCurrentScene()->GetGameObjects())
		{
			spGameObject->CheckShowInspecter(m_name);
		}
	}
}

void GameObject::ImGuiUpdateInspecter()
{
	if (!m_bShowInspecter)return;

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->ImGuiUpdateInspecter();
	}

	if (ImGui::Button("BakeJson"))
	{
		BakeJson();
	}

	int i = 0;
	m_pExistsComponents.clear();
	m_pNotExistsComponents.clear();
	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->WorkGeneralJson().at("Component").begin();
		it != m_wpJsonManager.lock()->WorkGeneralJson().at("Component").end(); it++)
	{
		bool bSameName = false;
		for (const auto& spComponent : m_spComponents)
		{
			if (spComponent->GetName() == it.key())
			{
				bSameName = true;
			}
		}

		if (!bSameName)
		{
			m_pExistsComponents.push_back(const_cast<char*>(it.key().c_str()));
		}
		else
		{
			m_pNotExistsComponents.push_back(const_cast<char*>(it.key().c_str()));
		}
	}

	if (ImGui::TreeNode("Add Component"))
	{
		int num;
		if (ImGui::ListBox("", &num, m_pExistsComponents.data(), static_cast<int>(m_pExistsComponents.size())))
		{
			switch (num)
			{
			case 0:
				AddComponent(m_pExistsComponents[0], m_name);
				break;
			case 1:
				AddComponent(m_pExistsComponents[1], m_name);
				break;
			default:
				break;
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Remove Component"))
	{
		int num;
		if (ImGui::ListBox("", &num, m_pNotExistsComponents.data(), static_cast<int>(m_pNotExistsComponents.size())))
		{
			switch (num)
			{
			case 0:
				RemoveComponent(m_pNotExistsComponents[0]);
				break;
			case 1:
				RemoveComponent(m_pNotExistsComponents[1]);
				break;
			default:
				break;
			}
		}
		ImGui::TreePop();
	}
}

void GameObject::CheckShowInspecter(const std::string_view& name)
{
	if (m_name == name)
	{
		m_bShowInspecter = true;
	}
	else
	{
		m_bShowInspecter = false;
	}
}

void GameObject::AddComponent(const std::string_view& componentName, const std::string_view& objectName, bool bInit)
{
	const auto it = m_wpComponentFactory.lock()->GetConstructs().find(componentName.data());

	if (it == m_wpComponentFactory.lock()->GetConstructs().end())
	{
		return;
	}

	std::shared_ptr<Component> spComponent = static_cast<std::shared_ptr<Component>>(it->second());

	m_spComponents.push_back(spComponent);
	spComponent->SetGameObject(this);
	spComponent->SetObjectName(objectName);
	if (bInit)
	{
		spComponent->Init();
	}
}

void GameObject::RemoveComponent(const std::string_view& componentName)
{
	for (auto& it = m_spComponents.begin(); it != m_spComponents.end(); it++)
	{
		if ((*it)->GetName() == componentName)
		{
			m_spComponents.erase(it);
			break;
		}
	}
}

void GameObject::ReferenceJson(bool bInit)
{
	m_name = m_wpJsonManager.lock()->GetJson(m_name)->GetConstJson().at("Name");
	m_tag = m_wpJsonManager.lock()->GetJson(m_name)->GetConstJson().at("Tag");

	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->GetJson(m_name)->WorkConstJson().at("Component").begin(); it != m_wpJsonManager.lock()->GetJson(m_name)->WorkConstJson().at("Component").end(); it++)
	{
		AddComponent(it.key(), m_name, bInit);
	}
}

void GameObject::BakeJson()
{
	auto json = nlohmann::json::object();

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->PrepareJsonParameter(json);
	}

	m_wpJsonManager.lock()->GetJson(m_name)->BakeJson(PrepareJsonParameter(json));
	m_wpJsonManager.lock()->Reference();

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->ReferenceJson();
	}
}

const nlohmann::json& GameObject::PrepareJsonParameter(nlohmann::json& json)
{
	json["Name"] = m_name;
	json["Tag"] = m_tag;

	auto array = nlohmann::json::object();

	array.clear();
	for (const auto& spComponent : m_spComponents)
	{
		array[spComponent->GetName()] = nullptr;
	}

	json["Component"] = array;

	return json;
}