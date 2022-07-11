#pragma once

#include "../CharacterObject.h"

class EnemyManager
{
public:
	EnemyManager() = default;
	~EnemyManager() = default;

	void AddObject(std::shared_ptr<CharacterObject>spEnemy);

	const std::list<std::weak_ptr<CharacterObject>>GetEnemyObjects()const { return m_wpEnemyObjects; }
private:
	std::list<std::weak_ptr<CharacterObject>> m_wpEnemyObjects;
};