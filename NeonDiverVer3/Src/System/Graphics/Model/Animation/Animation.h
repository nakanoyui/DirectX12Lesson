#pragma once

// アニメーションキー(クォータニオン
struct AnimKeyQuaternion
{
	float				m_time = 0;		// 時間
	Math::Quaternion	m_quat;			// クォータニオンデータ
};

// アニメーションキー(ベクトル
struct AnimKeyVector3
{
	float				m_time = 0;		// 時間
	Math::Vector3		m_vec;			// 3Dベクトルデータ
};

//============================
// アニメーションデータ
//============================
struct AnimationData
{
	// アニメーション名
	std::string		m_name;
	// アニメの長さ
	float			m_maxLength = 0;

	// １ノードのアニメーションデータ
	struct Node
	{
		int			m_nodeOffset = -1;	// 対象モデルノードのOffset値

		// 各チャンネル
		std::vector<AnimKeyVector3>		m_translations;	// 位置キーリスト
		std::vector<AnimKeyQuaternion>	m_rotations;	// 回転キーリスト
		std::vector<AnimKeyVector3>		m_scales;		// 拡縮キーリスト

		void Interpolate(Math::Matrix& rDst, float time);
		bool InterpolateTranslations(Math::Vector3& result, float time);
		bool InterpolateRotations(Math::Quaternion& result, float time);
		bool InterpolateScales(Math::Vector3& result, float time);
	};

	// 全ノード用アニメーションデータ
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

	// アニメーションが終了してる？
	bool IsAnimationEnd() const
	{
		if (m_spAnimation == nullptr) { return true; }
		if (m_time >= m_spAnimation->m_maxLength) { return true; }

		return false;
	}

	// アニメーションの更新
	void AdvanceTime(std::vector<ModelWork::Node>& rNodes, float speed = 1.0f);

	// アニメーションのリセット
	void ResetAdvanceTime() { m_time = 0; }

	inline float GetNowAnimationTime()const { return m_time; }

	inline float GetNowAnimationMaxTime()const { return m_spAnimation->m_maxLength; }

private:

	std::shared_ptr<AnimationData>	m_spAnimation = nullptr;	// 再生するアニメーションデータ

	bool m_bLoop;
	float m_time = 0.0f;
};