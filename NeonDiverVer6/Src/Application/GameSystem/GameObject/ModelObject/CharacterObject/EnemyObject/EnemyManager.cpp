#include "EnemyManager.h"

void EnemyManager::AddObject(std::shared_ptr<EnemyObject>spEnemy)
{
	m_wpEnemyObjects.push_back(spEnemy);
}