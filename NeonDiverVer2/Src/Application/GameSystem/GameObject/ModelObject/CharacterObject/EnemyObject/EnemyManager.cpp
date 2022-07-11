#include "EnemyManager.h"

void EnemyManager::AddObject(std::shared_ptr<CharacterObject>spEnemy)
{
	m_wpEnemyObjects.push_back(spEnemy);
}