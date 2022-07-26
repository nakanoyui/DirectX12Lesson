#pragma once

// �A�j���[�V�����L�[(�N�H�[�^�j�I��
struct AnimKeyQuaternion
{
	float				m_time = 0;		// ����
	Math::Quaternion	m_quat;			// �N�H�[�^�j�I���f�[�^
};

// �A�j���[�V�����L�[(�x�N�g��
struct AnimKeyVector3
{
	float				m_time = 0;		// ����
	Math::Vector3		m_vec;			// 3D�x�N�g���f�[�^
};

//============================
// �A�j���[�V�����f�[�^
//============================
struct AnimationData
{
	// �A�j���[�V������
	std::string		m_name;
	// �A�j���̒���
	float			m_maxLength = 0;

	// �P�m�[�h�̃A�j���[�V�����f�[�^
	struct Node
	{
		int			m_nodeOffset = -1;	// �Ώۃ��f���m�[�h��Offset�l

		// �e�`�����l��
		std::vector<AnimKeyVector3>		m_translations;	// �ʒu�L�[���X�g
		std::vector<AnimKeyQuaternion>	m_rotations;	// ��]�L�[���X�g
		std::vector<AnimKeyVector3>		m_scales;		// �g�k�L�[���X�g

		void Interpolate(Math::Matrix& rDst, float time);
		bool InterpolateTranslations(Math::Vector3& result, float time);
		bool InterpolateRotations(Math::Quaternion& result, float time);
		bool InterpolateScales(Math::Vector3& result, float time);
	};

	// �S�m�[�h�p�A�j���[�V�����f�[�^
	std::vector<Node>	m_nodes;
};

class Animator
{
public:
	const std::shared_ptr<AnimationData> GetAnimationData()const { return m_spAnimation; }

	inline void SetAnimation(const std::shared_ptr<AnimationData>& rData, const bool bLoop = true)
	{
		m_spAnimation = rData;
		m_bLoop = bLoop;

		m_time = 0.0f;
	}

	// �A�j���[�V�������I�����Ă�H
	bool IsAnimationEnd() const
	{
		if (m_spAnimation == nullptr) { return true; }
		if (m_time >= m_spAnimation->m_maxLength) { return true; }

		return false;
	}

	// �A�j���[�V�����̍X�V
	void AdvanceTime(std::vector<ModelWork::Node>& rNodes, float speed = 1.0f);

	// �A�j���[�V�����̃��Z�b�g
	void ResetAdvanceTime() { m_time = 0; }

	inline float GetNowAnimationTime()const { return m_time; }

	inline float GetNowAnimationMaxTime()const { return m_spAnimation->m_maxLength; }

private:

	std::shared_ptr<AnimationData>	m_spAnimation = nullptr;	// �Đ�����A�j���[�V�����f�[�^

	bool m_bLoop;
	float m_time = 0.0f;
};