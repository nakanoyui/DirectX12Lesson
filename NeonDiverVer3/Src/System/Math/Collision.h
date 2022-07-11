#pragma once

//=================================================
// ���b�V���̓����蔻�茋��
//=================================================
struct CollisionMeshResult
{
	bool m_hit = false;				// �����������ǂ���
	DirectX::XMVECTOR m_hitPos;		// �����������W
	DirectX::XMVECTOR m_hitDir;		// �Ώۂւ̕����x�N�g��
	float m_overlapDistance = 0.0f; // �d�Ȃ��Ă��鋗��
};

bool MeshIntersect(const Mesh& mesh, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);
bool MeshIntersect(const Mesh& mesh, const DirectX::BoundingSphere& sphere,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);

// �_ vs �|���S���i�O�p�`�ʁj�Ƃ̍ŋߐړ_�����߂�
void KdPointToPolygon(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& v1,
	const DirectX::XMVECTOR& v2, const DirectX::XMVECTOR& v3, DirectX::XMVECTOR& nearestPoint);