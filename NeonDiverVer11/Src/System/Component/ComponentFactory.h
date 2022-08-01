#pragma once

class ComponentFactory
{
public:
	void Init();

	const std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>>& GetConstructs() { return m_spConstructs; }

	const std::unordered_map<std::string, std::function<std::shared_ptr<ScriptComponent>()>>& GetScriptConstructs() { return m_spScriptConstructs; }
private:
	std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>> m_spConstructs;

	std::unordered_map<std::string, std::function<std::shared_ptr<ScriptComponent>()>> m_spScriptConstructs;

	std::weak_ptr<JsonManager> m_wpJsonManager;
};