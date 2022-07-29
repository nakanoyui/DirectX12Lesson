#pragma once

class Component;
class JsonManager;
class ComponentFactory;

class GameObject
{
public:
	GameObject(const std::string_view& name, const std::string_view& tag);
	~GameObject() = default;

	void Init();
	void Update();
	void Draw();
	void DrawLate();

	void ImGuiUpdateHierarchy();
	void ImGuiUpdateInspecter();

	const std::string& GetName() { return m_name; }
	const std::string* GetTag() { return &m_tag; }
	bool GetEnable() { return m_bEnable; }

	void SetName(const std::string_view& name) { m_name = name; }
	void SetTag(const std::string_view& tag) { m_tag = tag; }
	void SetEnable(const bool bEnable) { m_bEnable = bEnable; }

	void CheckShowInspecter(const std::string_view& name);
	bool GetShowInspecter() { return m_bShowInspecter; }

	const std::weak_ptr<JsonManager>& GetJsonManager() { return m_wpJsonManager; }

	void AddComponent(const std::string_view& componentName, const std::string_view& objectName, bool bInit = true);

	void RemoveComponent(const std::string_view& componentName);

	template<typename T>
	std::shared_ptr<T> GetComponent();

private:
	void ReferenceJson(bool bInit = true);
	void BakeJson();
	const nlohmann::json& PrepareJsonParameter(nlohmann::json& json);

	std::vector<std::shared_ptr<Component>>m_spComponents;
	std::string m_name;
	std::string m_tag;
	bool m_bEnable;
	bool m_bShowInspecter;
	std::vector<char*>m_pExistsComponents;
	std::vector<char*>m_pNotExistsComponents;
	std::weak_ptr<JsonManager> m_wpJsonManager;
	std::weak_ptr<ComponentFactory>m_wpComponentFactory;
};

template<typename T>
std::shared_ptr<T> GameObject::GetComponent()
{
	for (int i = 0; i < (int)m_spComponents.size(); ++i)
	{
		std::shared_ptr<T>spComponent = std::dynamic_pointer_cast<T>(m_spComponents[i]);

		if (spComponent != nullptr)
		{
			return spComponent;
		}
	}

	return nullptr;
}