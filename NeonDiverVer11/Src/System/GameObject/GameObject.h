#pragma once

class Component;
class ScriptComponent;
class JsonManager;
class ComponentFactory;

class GameObject :public std::enable_shared_from_this<GameObject>
{
public:
	GameObject(const std::string_view& name, const std::string_view& tag);
	~GameObject() = default;

	void Create();
	void Remove(const std::string& objectName);

	void Init();
	void Update();
	void Draw();
	void DrawLate();

	void ImGuiUpdateHierarchy();
	void ImGuiUpdateInspecter();

	void CheckDeleteComponent();

	const std::string& GetName()const { return m_name; }
	const std::string* GetTag() { return &m_tag; }
	bool GetEnable()const { return m_bEnable; }

	void SetName(const std::string_view& name) { m_name = name; }
	void SetTag(const std::string_view& tag) { m_tag = tag; }
	void SetEnable(const bool bEnable) { m_bEnable = bEnable; }

	bool GetShowInspecter() { return m_bShowInspecter; }

	const std::weak_ptr<JsonManager>& GetJsonManager() { return m_wpJsonManager; }

	void AddComponent(const std::string_view& componentName, const std::string_view& objectName,bool bStart = true);
	void AddScriptComponent(const std::string_view& componentName, const std::string_view& objectName, bool bStart = true);

	void RemoveComponent(const std::string_view& componentName);
	void RemoveScriptComponent(const std::string_view& componentName);

	template<typename T>
	std::shared_ptr<T> GetComponent();

	std::vector<std::shared_ptr<Component>>& WorkComponents() { return m_spComponents; }

	void BakeJson();
private:
	enum class JsonType
	{
		Instance,
		Constant,
	};

	void CheckShowInspecter(const std::string_view& name);
	void ReferenceJson();
	const nlohmann::json& PrepareJsonParameter(nlohmann::json& json, JsonType jsonType = JsonType::Constant, const std::string_view& ignoreObject = "");

	std::vector<std::shared_ptr<Component>>m_spComponents;
	std::vector<std::shared_ptr<ScriptComponent>>m_spScriptComponents;
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