#pragma once

#include "../ModelObject.h"

class PlayerManager;

class WeaponObject :public ModelObject
{
public:
	WeaponObject() = default;
	virtual ~WeaponObject() = default;

protected:
	std::weak_ptr<PlayerManager> m_wpPlayerManager;
	Math::Vector3 m_offeset;
};