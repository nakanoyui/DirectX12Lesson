#pragma once

#include "WeaponObject.h"

class WeaponManager
{
public:
	WeaponManager() = default;
	~WeaponManager() = default;

	void AddObject(std::shared_ptr<WeaponObject>spWeapon);

	const std::list<std::weak_ptr<WeaponObject>>GetEnemyObjects()const { return m_wpWeaponObjects; }
private:
	std::list<std::weak_ptr<WeaponObject>> m_wpWeaponObjects;
};