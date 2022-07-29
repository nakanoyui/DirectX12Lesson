#pragma once

class ComponentFactory
{
public:
	void Init();

	const std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>>& GetConstructs() { return m_spConstructs; };
private:
	std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>> m_spConstructs;

	std::weak_ptr<JsonManager> m_wpJsonManager;
};