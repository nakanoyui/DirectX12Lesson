#pragma once

#include "WeaponObject.h"

class WeaponManager
{
public:
	WeaponManager() = default;
	~WeaponManager() = default;

	void AddObject(const std::shared_ptr<WeaponObject>&spWeapon);

	const std::list<std::weak_ptr<WeaponObject>>&GetWeaponObjects()const { return m_wpWeaponObjects; }
private:
	std::list<std::weak_ptr<WeaponObject>> m_wpWeaponObjects;
};