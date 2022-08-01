#include "GameObject.h"

#include "Application/Application.h"

#include "System/Component/ComponentFactory.h"

GameObject::GameObject(const std::string_view& name, const std::string_view& tag) :
	m_name(name), m_tag(tag), m_bEnable(true), m_bShowInspecter(false)
{
}

void GameObject::Create()
{
	m_wpJsonManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager();
	m_wpComponentFactory = Application::Instance().GetSceneManager()->GetCurrentScene()->GetComponentFactory();

	m_wpJsonManager.lock()->BakeJson(m_name);

	auto json = nlohmann::json::object();

	PrepareJsonParameter(json, JsonType::Instance);

	m_wpJsonManager.lock()->WorkInstanceJson().BakeJson(json);
}

void GameObject::Remove(const std::string& objectName)
{
	std::filesystem::remove_all("Asset/Json/" + objectName);

	auto json = nlohmann::json::object();

	PrepareJsonParameter(json, JsonType::Instance, objectName);

	m_wpJsonManager.lock()->WorkInstanceJson().BakeJson(json);
}

void GameObject::Init()
{
	m_wpJsonManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetJsonManager();
	m_wpComponentFactory = Application::Instance().GetSceneManager()->GetCurrentScene()->GetComponentFactory();

	ReferenceJson();

	//bool bTransformComponent = false;
	//for (const auto& spComponent : m_spComponents)
	//{
	//	if ("TransformComponent" == spComponent->GetName())
	//	{
	//		bTransformComponent = true;
	//	}
	//}

	//if (!bTransformComponent)
	//{
	//	AddComponent("TransformComponent", m_name);
	//}

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->Start();
	}

	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		spScriptComponent->Start();
	}
}

void GameObject::Update()
{
	for (const auto& spComponent : m_spComponents)
	{
		spComponent->Update();
	}

	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		spScriptComponent->Update();
	}
}

void GameObject::Draw()
{
	for (const auto& spComponent : m_spComponents)
	{
		spComponent->Draw();
	}

	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		spScriptComponent->Draw();
	}
}

void GameObject::DrawLate()
{
	for (const auto& spComponent : m_spComponents)
	{
		spComponent->DrawLate();
	}

	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		spScriptComponent->DrawLate();
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

	ImGui::Checkbox(m_name.c_str(), &m_bEnable);
	ImGui::InputText("Tag", m_tag.data(), ImGuiHelper::InputTextSize);

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->ImGuiUpdateInspecter();
	}

	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		spScriptComponent->ImGuiUpdateInspecter();
	}

	if (ImGui::Button("BakeJson"))
	{
		BakeJson();
	}

	m_pExistsComponents.clear();
	m_pNotExistsComponents.clear();
	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("Component").begin();
		it != m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("Component").end(); it++)
	{
		if (it.key() == "TransformComponent")continue;

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
		int i;
		if (ImGui::ListBox("", &i, m_pExistsComponents.data(), static_cast<int>(m_pExistsComponents.size())))
		{
			AddComponent(m_pExistsComponents[i], m_name);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Remove Component"))
	{
		int i;
		if (ImGui::ListBox("", &i, m_pNotExistsComponents.data(), static_cast<int>(m_pNotExistsComponents.size())))
		{
			RemoveComponent(m_pNotExistsComponents[i]);
		}

		ImGui::TreePop();
	}

	m_pExistsComponents.clear();
	m_pNotExistsComponents.clear();

	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("ScriptComponent").begin();
		it != m_wpJsonManager.lock()->WorkGeneralJson().WorkConstJson().at("ScriptComponent").end(); it++)
	{
		bool bSameName = false;
		for (const auto& spComponent : m_spScriptComponents)
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

	if (ImGui::TreeNode("Add ScriptComponent"))
	{
		int i;
		if (ImGui::ListBox("", &i, m_pExistsComponents.data(), static_cast<int>(m_pExistsComponents.size())))
		{
			AddScriptComponent(m_pExistsComponents[i], m_name);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Remove ScriptComponent"))
	{
		int i;
		if (ImGui::ListBox("", &i, m_pNotExistsComponents.data(), static_cast<int>(m_pNotExistsComponents.size())))
		{
			RemoveScriptComponent(m_pNotExistsComponents[i]);
		}

		ImGui::TreePop();
	}
}

void GameObject::CheckDeleteComponent()
{
	for (auto& it = m_spComponents.begin(); it != m_spComponents.end(); it++)
	{
		if ((*it)->GetDelete())
		{
			m_spComponents.erase(it);
			break;
		}
	}

	for (auto& it = m_spScriptComponents.begin(); it != m_spScriptComponents.end(); it++)
	{
		if ((*it)->GetDelete())
		{
			m_spScriptComponents.erase(it);
			break;
		}
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

void GameObject::AddComponent(const std::string_view& componentName, const std::string_view& objectName, bool bStart)
{
	const auto it = m_wpComponentFactory.lock()->GetConstructs().find(componentName.data());

	if (it == m_wpComponentFactory.lock()->GetConstructs().end())
	{
		return;
	}

	std::shared_ptr<Component> spComponent = static_cast<std::shared_ptr<Component>>(it->second());

	m_spComponents.push_back(spComponent);
	spComponent->SetGameObject(shared_from_this());
	spComponent->SetObjectName(objectName);
	spComponent->Init();
	if (bStart)
	{
		spComponent->Start();
	}
}

void GameObject::AddScriptComponent(const std::string_view& componentName, const std::string_view& objectName, bool bStart)
{
	const auto it = m_wpComponentFactory.lock()->GetScriptConstructs().find(componentName.data());

	if (it == m_wpComponentFactory.lock()->GetScriptConstructs().end())
	{
		return;
	}

	std::shared_ptr<ScriptComponent> spScriptComponent = static_cast<std::shared_ptr<ScriptComponent>>(it->second());

	m_spScriptComponents.push_back(spScriptComponent);
	spScriptComponent->SetGameObject(shared_from_this());
	spScriptComponent->SetObjectName(objectName);
	spScriptComponent->Init();
	if (bStart)
	{
	spScriptComponent->Start();
	}
}

void GameObject::RemoveComponent(const std::string_view& componentName)
{
	for (const auto& spComponent : m_spComponents)
	{
		if (spComponent->GetName() == componentName)
		{
			spComponent->SetDelete(true);
			break;
		}
	}
}

void GameObject::RemoveScriptComponent(const std::string_view& componentName)
{
	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		if (spScriptComponent->GetName() == componentName)
		{
			spScriptComponent->SetDelete(true);
			break;
		}
	}
}

void GameObject::ReferenceJson()
{
	m_name = m_wpJsonManager.lock()->GetJson(m_name)->GetConstJson().at("Name");
	m_tag = m_wpJsonManager.lock()->GetJson(m_name)->GetConstJson().at("Tag");

	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->GetJson(m_name)->WorkConstJson().at("Component").begin(); it != m_wpJsonManager.lock()->GetJson(m_name)->WorkConstJson().at("Component").end(); it++)
	{
		AddComponent(it.key(), m_name,false);
	}

	for (nlohmann::json::iterator it = m_wpJsonManager.lock()->GetJson(m_name)->WorkConstJson().at("ScriptComponent").begin(); it != m_wpJsonManager.lock()->GetJson(m_name)->WorkConstJson().at("ScriptComponent").end(); it++)
	{
		AddScriptComponent(it.key(), m_name,false);
	}
}

void GameObject::BakeJson()
{
	auto json = nlohmann::json::object();

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->PrepareJsonParameter(json);
	}

	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		spScriptComponent->PrepareJsonParameter(json);
	}

	m_wpJsonManager.lock()->GetJson(m_name)->BakeJson(PrepareJsonParameter(json));
	m_wpJsonManager.lock()->Reference();

	for (const auto& spComponent : m_spComponents)
	{
		spComponent->ReferenceJson();
	}

	for (const auto& spScriptComponent : m_spScriptComponents)
	{
		spScriptComponent->ReferenceJson();
	}
}

const nlohmann::json& GameObject::PrepareJsonParameter(nlohmann::json& json, JsonType jsonType, const std::string_view& ignoreObject)
{
	switch (jsonType)
	{
	case GameObject::JsonType::Instance:
	{
		auto array = nlohmann::json::object();
		array.clear();
		for (const auto& spGameObject : Application::Instance().GetSceneManager()->GetCurrentScene()->GetGameObjects())
		{
			if (ignoreObject != spGameObject->GetName())
			{
				array[spGameObject->GetName()] = "Asset/Json/" + spGameObject->GetName() + "/constant.json";
			}
		}

		json["Instance"] = array;
	}
	break;
	case GameObject::JsonType::Constant:
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
		
		array.clear();
		for (const auto& spScriptComponents : m_spScriptComponents)
		{
			array[spScriptComponents->GetName()] = nullptr;
		}

		json["ScriptComponent"] = array;
	}
	break;
	default:
		break;
	}

	return json;
}