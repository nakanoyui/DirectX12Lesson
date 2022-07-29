#include "PlayerManager.h"

void PlayerManager::AddObject(const std::shared_ptr<Player>& spPlayer)
{
	m_wpPlayer = spPlayer;
}