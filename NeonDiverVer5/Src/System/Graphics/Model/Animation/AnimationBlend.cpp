#include "AnimationBlend.h"

void AnimationBlend::AdvanceTime(std::vector<ModelWork::Node>& rNodes, float speed)
{
	if (m_spNowAnimator)
	{
		m_spNowAnimator->AdvanceTime(rNodes, m_nowAnimeSpeed);
	}

	if (m_spNextAnimator)
	{
		std::vector<ModelWork::Node> nodes;
		nodes.resize((rNodes.size()));

		m_spNextAnimator->AdvanceTime(nodes, speed);

		m_bBlend = true;

		BlendAnimation(rNodes, nodes);

		if (m_progress > 1.0f)
		{
			m_spNowAnimator.reset();
			m_spNowAnimator = nullptr;
			m_spNowAnimator.swap(m_spNextAnimator);

			m_spNextAnimator.reset();
			m_spNextAnimator = nullptr;

			m_nowAnimeSpeed = speed;

			m_bBlend = false;
		}

		m_progress += m_speed;
	}
}

void AnimationBlend::SetAnimation(const std::shared_ptr<AnimationData>& nextData, bool bLoop)
{
	if (!nextData)return;

	m_spNextAnimator = std::make_shared<Animator>();
	m_spNextAnimator->SetAnimation(nextData,bLoop);

	m_progress = 0.0f;
}

const std::shared_ptr<Animator> AnimationBlend::GetAnimator() const
{
	if (m_spNextAnimator)
	{
		return m_spNextAnimator;
	}

	if (m_spNowAnimator)
	{
		return m_spNowAnimator;
	}

	return nullptr;
}

void AnimationBlend::BlendAnimation(std::vector<ModelWork::Node>& nowNodes, std::vector<ModelWork::Node>& nextNodes)
{
	for (auto animeNode : m_spNextAnimator->GetAnimationData()->m_nodes)
	{
		UINT idx = animeNode.m_nodeOffset;

		// 移動、回転、拡大をそれぞればらばらにする
		// 現在
		Math::Vector3 nowTrans, nowScale;
		Math::Quaternion nowQuaternion;
		// 次
		Math::Vector3 nextTrans, nextScale;
		Math::Quaternion nextQuaternion;

		// Decompose・・・行列の各要素を抽出
		nowNodes[idx].m_localTransform.Decompose(nowScale, nowQuaternion, nowTrans);
		nextNodes[idx].m_localTransform.Decompose(nextScale, nextQuaternion, nextTrans);

		// 移動・回転・拡大行列をそれぞれ求める
		Math::Vector3 combineScale = DirectX::XMVectorLerp(nowScale, nextScale, m_progress);
		Math::Vector3 combineTrans = DirectX::XMVectorLerp(nowTrans, nextTrans, m_progress);
		Math::Quaternion quaternion = DirectX::XMQuaternionSlerp(nowQuaternion, nextQuaternion, m_progress);

		Math::Matrix mScale = Math::Matrix::CreateScale(combineScale);
		Math::Matrix mTrans = Math::Matrix::CreateTranslation(combineTrans);
		Math::Matrix mRotate = Math::Matrix::CreateFromQuaternion(quaternion);

		nowNodes[idx].m_localTransform = mScale * mRotate * mTrans;
	}
}