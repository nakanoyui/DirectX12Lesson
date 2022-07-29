#pragma once

#include "../CharacterObject.h"

class Player :public CharacterObject
{
public:
	enum class AnimeType
	{
		Combo01,
		Combo02,
		Combo03,
		AirCombo01,
		AirCombo02,
		AirCombo03,
		Idle,
		KnockBack01,
		Rise01,
		RunFast,
		StrafeRunFront,
		StrafeRunBack,
		StrafeRunLeft,
		StrafeRunLeft45,
		StrafeRunLeft135,
		StrafeRunRight,
		StrafeRunRight45,
		StrafeRunRight135,
		JumpStart,
		JumpLanding,
		DodgeBack,
	};

	Player() = default;
	~Player() = default;

	void LoadAsset()override;

	void Init()override;
	void Update()override;
	void DrawPlayer()override;
	void DrawEffect()override;

	void ImGuiUpdate()override;

	// �A�N�Z�T
	AnimationBlend& GetAnimationBlend() override { return m_animationBlend; }
	const AnimeType& GetAnimeType() const{ return m_animeType; }

	void SetAnimeSpeed(float animeSpeed)override { m_animeSpeed = animeSpeed; }
	void SetHitStop(bool bHitStop) { m_bHitStop = bHitStop; }

	float GetLimPower() { return m_limPower; }
	void SetJustDodge(bool bJustDodge) { m_bJustDodge = bJustDodge; }

private:
	void ReferenceJson()override;

	void UpdateAnimation()override;
	bool UpdateAnimationAttack();
	void UpdateAnimationDodge();
	void UpdateCollision()override;
	void UpdateRotate()override;
	void UpdateMatrix()override;
	void UpdateCamera();

	float m_speed;
	float m_jumpPower;
	bool m_bJump;
	bool m_bJumpLanded;
	bool m_bJumpAttack;

	bool m_bJustDodge;

	float m_limPower;
	float m_dissolveRate;
	float m_justDodgeSpeedCount;

	int m_hitStopCount;
	bool m_bHitStop;
	AnimeType m_saveAnimeType;

	std::vector<Math::Matrix> m_mCamWork;

	std::weak_ptr<TPSCamera>			m_wpCamera;
	std::weak_ptr<CameraManager>		m_wpCameraManager;

	class AnimationState
	{
	public:
		AnimationState() = default;
		virtual ~AnimationState() = default;
	};

	class Combo01 :public AnimationState
	{
	public:
		Combo01(Player* pPlayer);
	};

	class Combo02 :public AnimationState
	{
	public:
		Combo02(Player* pPlayer);
	};

	class Combo03 :public AnimationState
	{
	public:
		Combo03(Player* pPlayer);
	};

	class AirCombo01 :public AnimationState
	{
	public:
		AirCombo01(Player* pPlayer);
	};

	class AirCombo02 :public AnimationState
	{
	public:
		AirCombo02(Player* pPlayer);
	};

	class AirCombo03 :public AnimationState
	{
	public:
		AirCombo03(Player* pPlayer);
	};

	class Idle :public AnimationState
	{
	public:
		Idle(Player* pPlayer);
	};

	class KnockBack01 :public AnimationState
	{
	public:
		KnockBack01(Player* pPlayer);
	};

	class Rise01 :public AnimationState
	{
	public:
		Rise01(Player* pPlayer);
	};

	class RunFast :public AnimationState
	{
	public:
		RunFast(Player* pPlayer);
	};

	class StrafeRunFront :public AnimationState
	{
	public:
		StrafeRunFront(Player* pPlayer);
	};

	class StrafeRunBack :public AnimationState
	{
	public:
		StrafeRunBack(Player* pPlayer);
	};

	class StrafeRunRight :public AnimationState
	{
	public:
		StrafeRunRight(Player* pPlayer);
	};

	class StrafeRunLeft :public AnimationState
	{
	public:
		StrafeRunLeft(Player* pPlayer);
	};

	class StrafeRunRight45 :public AnimationState
	{
	public:
		StrafeRunRight45(Player* pPlayer);
	};

	class StrafeRunRight135 :public AnimationState
	{
	public:
		StrafeRunRight135(Player* pPlayer);
	};

	class StrafeRunLeft45 :public AnimationState
	{
	public:
		StrafeRunLeft45(Player* pPlayer);
	};

	class StrafeRunLeft135 :public AnimationState
	{
	public:
		StrafeRunLeft135(Player* pPlayer);
	};

	class JumpStart :public AnimationState
	{
	public:
		JumpStart(Player* pPlayer);
	};

	class JumpLanding :public AnimationState
	{
	public:
		JumpLanding(Player* pPlayer);
	};

	class DodgeBack :public AnimationState
	{
	public:
		DodgeBack(Player* pPlayer);
	};

	class AnimationStateContext
	{
	public:
		AnimationStateContext(Player* pPlayer) :m_upState(std::make_unique<StrafeRunFront>(pPlayer)), m_upNext() {}

		// ���̏�ԂɑJ�ڂ�����
		void SetNextState(std::unique_ptr<AnimationState> upState)
		{
			if (m_upNext) m_upNext = nullptr;
			m_upNext.swap(upState);
		}
	private:
		std::unique_ptr<AnimationState> m_upState = nullptr;		// ���݂̃X�e�[�g
		std::unique_ptr<AnimationState> m_upNext = nullptr;			// ���̃X�e�[�g
	};

	AnimeType m_animeType;
	std::unique_ptr<Player::AnimationStateContext> m_upStateContext;
};