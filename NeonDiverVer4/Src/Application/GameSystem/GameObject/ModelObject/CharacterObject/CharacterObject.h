#pragma once

#include "../ModelObject.h"

class CharacterObject :public ModelObject
{
public:
	CharacterObject() = default;
	virtual ~CharacterObject() = default;

	virtual void UpdateAnimation(){}
	virtual void UpdateCollision(){}
	virtual void UpdateRotate(){}
	virtual void UpdateMatrix() {}

	// アクセサ
	virtual AnimationBlend& GetAnimationBlend() { return m_animationBlend; }
	virtual void SetAnimeSpeed(const float& animeSpeed) { m_animeSpeed = animeSpeed; }

protected:
	std::weak_ptr<KeyManager>m_wpKeyManager;
	Math::Vector3 m_velocity;

	float m_yaw;

	AnimationBlend m_animationBlend;
	float m_animeSpeed;
};