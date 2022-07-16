#include "WeaponManager.h"

void WeaponManager::AddObject(std::shared_ptr<WeaponObject> spWeapon)
{
	m_wpWeaponObjects.push_back(spWeapon);
}