#pragma once

#include "../ModelObject.h"

class CharacterObject :public ModelObject
{
public:
	CharacterObject() = default;
	virtual ~CharacterObject() = default;

protected:
	std::weak_ptr<KeyManager>m_wpKeyManager;
	Math::Vector3 m_velocity;
};