#pragma once

#include "../CharacterObject.h"

class PlayerManager;

class EnemyObject :public CharacterObject
{
public:
	EnemyObject() = default;
	virtual ~EnemyObject() = default;

	virtual void Init()override;
	virtual void UpdateSearch() {}

	virtual void Reset() {}
protected:
	std::weak_ptr<PlayerManager> m_wpPlayerManager;

	std::unique_ptr<DebugWireFrame> m_upSearchDebugWireFrame = nullptr;
	std::unique_ptr<DebugWireFrame> m_upAttackDebugWireFrame = nullptr;

	Math::Vector3 m_attackVelocity;

	int m_attackCoolTimeCount;
	bool m_bStartAttack;
};