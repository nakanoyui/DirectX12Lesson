#include "WeaponManager.h"

void WeaponManager::AddObject(const std::shared_ptr<WeaponObject>& spWeapon)
{
	m_wpWeaponObjects.push_back(spWeapon);
}