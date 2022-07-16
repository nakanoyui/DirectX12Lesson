#pragma once

#include "../ModelObject.h"

class CharacterObject :public ModelObject
{
public:
	enum class DamageType
	{
		None,
		KnockBack01,
		KnockBack02,
		BlowAway,
	};

	CharacterObject() = default;
	virtual ~CharacterObject() = default;

	virtual void Update()override;
	virtual void UpdateAnimation(){}
	virtual void UpdateCollision(){}
	virtual void UpdateRotate(){}
	virtual void UpdateMatrix() {}

	// アクセサ
	virtual const AnimationBlend& GetAnimationBlend() { return m_animationBlend; }
	virtual void SetAnimeSpeed(const float& animeSpeed) { m_animeSpeed = animeSpeed; }

	const DamageType& GetDamageType()const { return m_damageType; }
	void SetDamageType(const DamageType& damageType) { m_damageType = damageType; }

protected:
	std::weak_ptr<KeyManager>m_wpKeyManager;

	DamageType m_damageType = DamageType::None;
	Math::Vector3 m_velocity;

	float m_verticalVector;

	float m_yaw;

	AnimationBlend m_animationBlend;
	float m_animeSpeed;
};