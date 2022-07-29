#pragma once

//=================================================
// メッシュの当たり判定結果
//=================================================
struct CollisionMeshResult
{
	bool m_hit = false;				// 当たったかどうか
	DirectX::XMVECTOR m_hitPos;		// 当たった座標
	DirectX::XMVECTOR m_hitDir;		// 対象への方向ベクトル
	float m_overlapDistance = 0.0f; // 重なっている距離
};

bool MeshIntersect(const Mesh& mesh, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);
bool MeshIntersect(const Mesh& mesh, const DirectX::BoundingSphere& sphere,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);

// 点 vs ポリゴン（三角形面）との最近接点を求める
void KdPointToPolygon(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& v1,
	const DirectX::XMVECTOR& v2, const DirectX::XMVECTOR& v3, DirectX::XMVECTOR& nearestPoint);