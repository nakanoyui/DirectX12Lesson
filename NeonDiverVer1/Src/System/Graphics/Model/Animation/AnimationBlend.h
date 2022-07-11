#pragma once

class AnimationBlend
{
public:
	void AdvanceTime(std::vector<ModelWork::Node>& rNodes, float speed = 1.0f);

	// �A�N�Z�T
	void SetAnimation(const std::shared_ptr<AnimationData>& nextData, bool bLoop = true);
	void SetSpeed(const float speed) { m_speed = speed; }

	const std::shared_ptr<Animator>GetAnimator()const;
private:
	void BlendAnimation(std::vector<ModelWork::Node>& nowNodes, std::vector<ModelWork::Node>& nextNodes);

	std::shared_ptr<Animator> m_spNowAnimator = nullptr;	// ���݂̃A�j���[�V����
	std::shared_ptr<Animator> m_spNextAnimator = nullptr;	// ���̃A�j���[�V����

	float m_nowAnimeSpeed;
	float m_nextAnimeSpeed;

	float m_progress = 0.0f;	// �u�����h�i���x
	float m_speed = 0.02f;		// �u�����h�̈ڍs�i�x	
};