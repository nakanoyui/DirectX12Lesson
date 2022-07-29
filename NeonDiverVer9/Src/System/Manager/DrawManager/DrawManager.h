#pragma once

class DrawList
{
public:
	DrawList() = default;
	~DrawList() = default;

	void AddDrawList(const std::shared_ptr<GameObject>& spGameObject) { m_wpGameObjects.push_back(spGameObject); }

	const std::list<std::weak_ptr<GameObject>>&GetGameObjects()const { return m_wpGameObjects; }

private:
	std::list<std::weak_ptr<GameObject>> m_wpGameObjects;
};

class DrawManager
{
public:
	DrawManager() = default;
	~DrawManager() = default;

	void Create(const std::string_view &drawListName);

	const std::shared_ptr<DrawList>& GetDrawList(const std::string_view& drawListName);

private:
	std::map<std::string, std::shared_ptr<DrawList>> m_spDrawLists;
};