#pragma once

#include "../CharacterObject.h"

class PlayerManager;

class EnemyObject :public CharacterObject
{
public:
	enum class DamageType
	{
		None,
		KnockBack1,
		KnockBack2,
		BlowAway,
	};

	EnemyObject() = default;
	virtual ~EnemyObject() = default;

	virtual void Init();

	void SetDatameType(const DamageType& damageType) { m_damageType = damageType; }
	virtual void Reset() {}
protected:
	DamageType m_damageType = DamageType::None;

	std::weak_ptr<PlayerManager> m_wpPlayerManager;
};