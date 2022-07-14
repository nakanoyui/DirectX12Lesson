#include "PlayerManager.h"

void PlayerManager::AddObject(std::shared_ptr<Player>spPlayer)
{
	m_wpPlayer = spPlayer;
}