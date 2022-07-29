#pragma once

#include "../EnemyObject.h"

class Robot :public EnemyObject
{
public:
	Robot() = default;
	~Robot() = default;

	void LoadAsset()override;

	void Init()override;
	void Update()override;
	void DrawSkinMesh()override;
	void DrawEffect()override;

	void UpdateAnimation()override;
	void UpdateSearch()override;
	void UpdateCollision()override;
	void UpdateMatrix()override;

	void ImGuiUpdateGameObject()override;

	// �A�N�Z�T
	AnimationBlend& GetAnimationBlend()override { return m_animationBlend; }
	void SetAnimeSpeed(float animeSpeed)override { m_animeSpeed = animeSpeed; }

	void Reset()override;
private:
	void ReferenceJson()override;

	bool m_bBump = false;
	bool m_bSearchRange;

	std::unique_ptr<DebugWireFrame> m_upLHandDebugWireFrame = nullptr;
	std::unique_ptr<DebugWireFrame> m_upRHandDebugWireFrame = nullptr;

	Math::Matrix m_mLHand;
	Math::Matrix m_mRHand;

	enum class AnimeType
	{
		Idle,
		Attack,
		Damage1,
		Damage2,
		Dead,
	};

	class AnimationState
	{
	public:
		AnimationState() = default;
		virtual ~AnimationState() = default;
	};

	class Idle :public AnimationState
	{
	public:
		Idle(Robot* pRobot);
	};

	class Attack :public AnimationState
	{
	public:
		Attack(Robot* pRobot);
	};

	class Damage1 :public AnimationState
	{
	public:
		Damage1(Robot* pRobot);
	};

	class Damage2 :public AnimationState
	{
	public:
		Damage2(Robot* pRobot);
	};

	class Dead :public AnimationState
	{
	public:
		Dead(Robot* pRobot);
	};

	class AnimationStateContext
	{
	public:
		AnimationStateContext(Robot* pPlayer) :m_upState(std::make_unique<Idle>(pPlayer)), m_upNext() {}

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

	std::unique_ptr<AnimationStateContext> m_upStateContext;
	AnimeType m_animeType;
};