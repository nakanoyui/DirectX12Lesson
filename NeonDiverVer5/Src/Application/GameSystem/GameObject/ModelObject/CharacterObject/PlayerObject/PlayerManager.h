#pragma once

#include "Player.h"

class PlayerManager
{
public:
	PlayerManager() = default;
	~PlayerManager() = default;

	void AddObject(std::shared_ptr<Player>spPlayer);

	const std::weak_ptr<Player>GetPlayer()const { return m_wpPlayer; }
private:
	std::weak_ptr<Player> m_wpPlayer;
};