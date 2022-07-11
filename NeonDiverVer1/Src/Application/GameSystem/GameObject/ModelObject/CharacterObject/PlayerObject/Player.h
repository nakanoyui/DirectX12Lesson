#pragma once

#include "../CharacterObject.h"

class AnimationStateContext;

class Player :public CharacterObject
{
public:

	Player() = default;
	~Player() = default;

	void LoadAsset()override;

	void Init()override;
	void Update()override;
	void DrawSkinMesh()override;

	void UpdateAnimation();
	void UpdateRotate();
	void UpdateMatrix();
	void UpdateCamera();

	void ImGuiUpdate()override;

	// アクセサ
	AnimationBlend& GetAnimationBlend() { return m_animationBlend; }

private:

	enum class AnimeType
	{
		Combo01,
		Combo02,
		Combo03,
		Idle,
		StrafeRunFront,
		StrafeRunBack,
		StrafeRunLeft,
		StrafeRunLeft45,
		StrafeRunLeft135,
		StrafeRunRight,
		StrafeRunRight45,
		StrafeRunRight135,
	};

	float m_speed;
	float m_jumpPower;
	bool m_bJump;
	float m_yaw;

	std::vector<Math::Matrix> m_mCamWork;

	std::weak_ptr<TPSCamera>			m_wpCamera;
	std::weak_ptr<CameraManager>		m_wpCameraManager;

	AnimationBlend m_animationBlend;

	std::unique_ptr<AnimationStateContext> m_upStateContext;
	AnimeType m_animeType;
};

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

class Idle :public AnimationState
{
public:
	Idle(Player* pPlayer);
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

class AnimationStateContext
{
public:
	AnimationStateContext(Player* pPlayer) :m_upState(std::make_unique<StrafeRunFront>(pPlayer)), m_upNext() {}

	// 次の状態に遷移させる
	void SetNextState(std::unique_ptr<AnimationState> upState)
	{
		if (m_upNext) m_upNext = nullptr;
		m_upNext.swap(upState);
	}
private:
	std::unique_ptr<AnimationState> m_upState = nullptr;		// 現在のステート
	std::unique_ptr<AnimationState> m_upNext = nullptr;			// 次のステート
};