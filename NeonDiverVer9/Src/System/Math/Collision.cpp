using namespace DirectX;

bool MeshIntersect(const Mesh& mesh, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir,
	float rayRange, const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult)
{
	//--------------------------------------------------------
	// レイの逆行列化
	//--------------------------------------------------------

	// モデルの逆行列でレイを変換
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, matrix);

	// レイの判定開始位置を逆変換
	DirectX::XMVECTOR rayPosInv = XMVector3TransformCoord(rayPos, invMat);

	// レイの方向を逆変換
	DirectX::XMVECTOR rayDirInv = XMVector3TransformNormal(rayDir, invMat);

	float dirLength = DirectX::XMVector3Length(rayDirInv).m128_f32[0];

	// レイの方向ベクトルの長さ=拡大率で判定限界距離を補正
	// ※逆行列に拡縮が入っていると、レイの長さが変わるため
	// レイが当たった座標からの距離に拡縮が反映されてしまうので
	// 判定用の最大距離にも拡縮を反映させておく
	float limitRange = rayRange * dirLength;

	// 方角レイとして正しく扱うためには長さが１でなければならないので正規化
	rayDirInv = DirectX::XMVector3Normalize(rayDirInv);

	//--------------------------------------------------------
	// ブロードフェイズ
	// 　比較的軽量なAABB vs レイな判定で、
	// 　あきらかにヒットしない場合は、これ以降の判定を中止
	//--------------------------------------------------------
	{
		// AABB vs レイ
		float AABBdist = FLT_MAX;
		DirectX::BoundingBox aabb;
		mesh.GetBoundingBox().Transform(aabb, matrix);

		if (aabb.Intersects(rayPos, rayDir, AABBdist) == false) { return false; }

		// 最大距離外なら範囲外なので中止
		if (AABBdist > limitRange) { return false; }
	}

	//--------------------------------------------------------
	// ナローフェイズ
	// 　レイ vs 全ての面
	//--------------------------------------------------------

	// ヒット判定
	bool isHit = false;
	float closestDist = FLT_MAX;

	// DEBUGビルドでも速度を維持するため、別変数に拾っておく
	const MeshFace* pFaces = &mesh.GetFaces()[0];
	UINT faceNum = static_cast<UINT>(mesh.GetFaces().size());

	// 全ての面(三角形)
	for (UINT faceIdx = 0; faceIdx < faceNum; ++faceIdx)
	{
		// 三角形を構成する３つの頂点のIndex
		const UINT* idx = pFaces[faceIdx].Idx;

		// レイと三角形の判定
		float triDist = FLT_MAX;
		bool bHit = DirectX::TriangleTests::Intersects(rayPosInv, rayDirInv,
			mesh.GetVertexPositions()[idx[0]],
			mesh.GetVertexPositions()[idx[1]],
			mesh.GetVertexPositions()[idx[2]],
			triDist
		);

		// ヒットした
		if (bHit == false) { continue; }

		// 最大距離以内の場合
		if (triDist <= limitRange)
		{
			isHit = true;

			// 当たり判定を行うモデルの行列の拡大率が影響するので、その影響を打ち消す
			triDist /= dirLength;

			// 近いほうを残す
			if ((triDist) < closestDist)
			{
				closestDist = triDist;		// 距離を更新
			}
		}
	}

	if (!pResult) { return isHit; }

	pResult->m_hit = isHit;

	// リザルトに結果を格納
	if (isHit)
	{
		pResult->m_hitPos = DirectX::XMVectorAdd(rayPos, DirectX::XMVectorScale(rayDir, closestDist));

		pResult->m_hitDir = DirectX::XMVectorScale(rayDir, -1);

		pResult->m_overlapDistance = rayRange - closestDist;
	}

	return isHit;
}

bool MeshIntersect(const Mesh& mesh, const DirectX::BoundingSphere& sphere,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult)
{
	//------------------------------------------
	// ブロードフェイズ
	// 　高速化のため、まずは境界ボックス(AABB)で判定
	// 　この段階でヒットしていないなら、詳細な判定をする必要なし
	//------------------------------------------
	{
		// メッシュのAABBを元に、行列で変換したAABBを作成
		DirectX::BoundingBox aabb;
		mesh.GetBoundingBox().Transform(aabb, matrix);

		if (aabb.Intersects(sphere) == false) { return false; }
	}

	//------------------------------------------
	// ナローフェイズ
	// 　球とメッシュとの詳細判定
	//------------------------------------------

	// １つでもヒットしたらtrue
	bool isHit = false;

	// DEBUGビルドでも速度を維持するため、別変数に拾っておく
	const auto* pFaces = &mesh.GetFaces()[0];
	UINT faceNum = static_cast<UINT>(mesh.GetFaces().size());
	const Math::Vector3* vertices = &mesh.GetVertexPositions()[0];

	// メッシュの逆行列で、球の中心座標を変換(メッシュの座標系へ変換される)
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, matrix);
	DirectX::XMVECTOR spherePos = XMLoadFloat3(&sphere.Center);
	spherePos = XMVector3TransformCoord(spherePos, invMat);

	// 半径の二乗(判定の高速化用)
	float radiusSq = sphere.Radius * sphere.Radius;	// 半径の２乗

	// 行列の各軸の拡大率を計算しておく
	DirectX::XMVECTOR scale;
	scale.m128_f32[0] = DirectX::XMVector3Length(matrix.r[0]).m128_f32[0];
	scale.m128_f32[1] = DirectX::XMVector3Length(matrix.r[1]).m128_f32[0];
	scale.m128_f32[2] = DirectX::XMVector3Length(matrix.r[2]).m128_f32[0];
	scale.m128_f32[3] = 0;

	DirectX::XMVECTOR finalPos = spherePos;
	DirectX::XMVECTOR nearestPoint;

	// 全ての面と判定
	// ※判定はメッシュのローカル空間で行われる
	for (UINT fi = 0; fi < faceNum; fi++)
	{
		DirectX::XMVECTOR nearPoint;

		// 点 と 三角形 の最近接点を求める
		KdPointToPolygon(finalPos,
			vertices[pFaces[fi].Idx[0]],
			vertices[pFaces[fi].Idx[1]],
			vertices[pFaces[fi].Idx[2]],
			nearPoint);

		// 最近接点→球　ベクトルを求める
		DirectX::XMVECTOR vToCenter = finalPos - nearPoint;

		// XYZ軸が別々の大きさで拡縮されてる状態の場合、球が変形してる状態なため正確な半径がわからない。
		// そこでscaleをかけてXYZ軸のスケールが均等な座標系へ変換する
		vToCenter *= scale;

		// 最近接点が半径以内の場合は、衝突している
		if (DirectX::XMVector3LengthSq(vToCenter).m128_f32[0] <= radiusSq)
		{
			nearPoint = XMVector3TransformCoord(nearPoint, matrix);

			// 押し戻し計算
			// めり込んでいるぶんのベクトルを計算
			DirectX::XMVECTOR vPush = DirectX::XMVector3Normalize(vToCenter);

			vPush *= sphere.Radius - DirectX::XMVector3Length(vToCenter).m128_f32[0];

			// 拡縮を考慮した座標系へ戻す
			vPush /= scale;

			// 球の座標を移動させる
			finalPos += vPush;

			nearestPoint = nearPoint;

			isHit = true;
		}
	}

	if (!pResult) { return isHit; }

	pResult->m_hit = isHit;

	// リザルトに結果を格納
	if (isHit)
	{
		pResult->m_hitPos = XMVector3TransformCoord(nearestPoint, matrix);

		finalPos = XMVector3TransformCoord(finalPos, matrix);

		pResult->m_hitDir = DirectX::XMVectorSubtract(finalPos, XMLoadFloat3(&sphere.Center));

		pResult->m_overlapDistance = DirectX::XMVector3Length(pResult->m_hitDir).m128_f32[0];

		if (pResult->m_overlapDistance)
		{
			pResult->m_hitDir = DirectX::XMVector3Normalize(pResult->m_hitDir);
		}
	}

	return isHit;
}

// 点 vs 三角形との最近接点を求める
void KdPointToPolygon(const XMVECTOR& p, const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c, DirectX::XMVECTOR& outPt)
{
	// ※参考:[書籍]「ゲームプログラミングのためのリアルタイム衝突判定」

	// pがaの外側の頂点領域の中にあるかどうかチェック
	XMVECTOR ab = b - a;
	XMVECTOR ac = c - a;
	XMVECTOR ap = p - a;

	float d1 = XMVector3Dot(ab, ap).m128_f32[0];//ab.Dot(ap);
	float d2 = XMVector3Dot(ac, ap).m128_f32[0];//ac.Dot(ap);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		outPt = a;	// 重心座標(1,0,0)
		return;
	}

	// pがbの外側の頂点領域の中にあるかどうかチェック
	XMVECTOR bp = p - b;
	float d3 = XMVector3Dot(ab, bp).m128_f32[0];//ab.Dot(bp);
	float d4 = XMVector3Dot(ac, bp).m128_f32[0];//ac.Dot(bp);

	if (d3 >= 0.0f && d4 <= d3)
	{
		outPt = b;	// 重心座標(0,1,0)
		return;
	}

	// pがabの辺領域の中にあるかどうかチェックし、あればpのab上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;

	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		outPt = a + ab * v;	// 重心座標(1-v,v,0)
		return;
	}

	// pがcの外側の頂点領域の中にあるかどうかチェック
	XMVECTOR cp = p - c;
	float d5 = XMVector3Dot(ab, cp).m128_f32[0];//ab.Dot(cp);
	float d6 = XMVector3Dot(ac, cp).m128_f32[0];;//ac.Dot(cp);

	if (d6 >= 0.0f && d5 <= d6)
	{
		outPt = c;	// 重心座標(0,0,1)
		return;
	}

	// pがacの辺領域の中にあるかどうかチェックし、あればpのac上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		outPt = a + ac * w;	// 重心座標(1-w,0,w)
		return;
	}

	// pがbcの辺領域の中にあるかどうかチェックし、あればpのbc上に対する射影を返す
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		outPt = b + (c - b) * w;	// 重心座標(0,1-w,w)
		return;
	}

	// pは面領域の中にある。Qをその重心座標(u,v,w)を用いて計算
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	outPt = a + ab * v + ac * w;	// = u*a + v*b + w*c, u = va*demon = 1.0f - v - w
}