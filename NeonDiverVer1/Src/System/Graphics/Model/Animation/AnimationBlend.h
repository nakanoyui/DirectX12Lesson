#pragma once

class AnimationBlend
{
public:
	void AdvanceTime(std::vector<ModelWork::Node>& rNodes, float speed = 1.0f);

	// アクセサ
	void SetAnimation(const std::shared_ptr<AnimationData>& nextData, bool bLoop = true);
	void SetSpeed(const float speed) { m_speed = speed; }

	const std::shared_ptr<Animator>GetAnimator()const;
private:
	void BlendAnimation(std::vector<ModelWork::Node>& nowNodes, std::vector<ModelWork::Node>& nextNodes);

	std::shared_ptr<Animator> m_spNowAnimator = nullptr;	// 現在のアニメーション
	std::shared_ptr<Animator> m_spNextAnimator = nullptr;	// 次のアニメーション

	float m_nowAnimeSpeed;
	float m_nextAnimeSpeed;

	float m_progress = 0.0f;	// ブレンド進捗度
	float m_speed = 0.02f;		// ブレンドの移行進度	
};