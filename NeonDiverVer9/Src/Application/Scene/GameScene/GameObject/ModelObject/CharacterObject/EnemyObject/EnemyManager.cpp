#include "EnemyManager.h"

void EnemyManager::AddObject(const std::shared_ptr<EnemyObject>& spEnemy)
{
	m_wpEnemyObjects.push_back(spEnemy);
}