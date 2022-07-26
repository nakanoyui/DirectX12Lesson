#pragma once

#include "EnemyObject.h"

class EnemyManager
{
public:
	EnemyManager() = default;
	~EnemyManager() = default;

	void AddObject(std::shared_ptr<EnemyObject>spEnemy);

	const std::list<std::weak_ptr<EnemyObject>>&GetEnemyObjects()const { return m_wpEnemyObjects; }

private:
	std::list<std::weak_ptr<EnemyObject>> m_wpEnemyObjects;
};