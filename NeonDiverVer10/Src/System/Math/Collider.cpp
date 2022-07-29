//#include "Collider.h"
//
//bool Collider::Intersects(Type checkType, const DirectX::BoundingSphere& target, std::list<Collider::CollisionResult>* pResults)
//{
//	bool isHit = false;
//
//	for (auto& collisionShape : m_collisions)
//	{
//		if (!(checkType & collisionShape->GetType())) { continue; }
//
//		Collider::CollisionResult tmpRes;
//		Collider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;
//
//		if (collisionShape->Intersects(target, m_owner.GetMatrix(), pTmpRes))
//		{
//			isHit = true;
//
//			if (!pResults) { break; }
//
//			pResults->push_back(tmpRes);
//		}
//	}
//
//	return isHit;
//}
//
//bool Collider::Intersects(Type checkType, const DirectX::BoundingBox& target, std::list<Collider::CollisionResult>* pResults)
//{
//	bool isHit = false;
//
//	for (auto& collisionShape : m_collisions)
//	{
//		if (!(checkType & collisionShape->GetType())) { continue; }
//
//		Collider::CollisionResult tmpRes;
//		Collider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;
//
//		if (collisionShape->Intersects(target, m_owner.GetMatrix(), pTmpRes))
//		{
//			isHit = true;
//
//			if (!pResults) { break; }
//
//			pResults->push_back(tmpRes);
//		}
//	}
//
//	return isHit;
//}
//
//bool Collider::Intersects(Type checkType, const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults)
//{
//	if (!target.m_dir.LengthSquared()) { return false; }
//
//	bool isHit = false;
//
//	for (auto& collisionShape : m_collisions)
//	{
//		if (!(checkType & collisionShape->GetType())) { continue; }
//
//		Collider::CollisionResult tmpRes;
//		Collider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;
//
//		if (collisionShape->Intersects(target, m_owner.GetMatrix(), pTmpRes))
//		{
//			isHit = true;
//
//			if (!pResults) { break; }
//
//			pResults->push_back(tmpRes);
//		}
//	}
//
//	return isHit;
//}
//
//bool SphereCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
//{
//	DirectX::BoundingSphere myShape;
//
//	m_shape.Transform(myShape, world);
//
//	// 球同士の当たり判定
//	bool isHit = myShape.Intersects(target);
//
//	// 詳細なしなら結果だけ返す
//	if (!pRes) { return isHit; }
//
//	pRes->m_hit = isHit;
//
//	// 当たった時のみ計算
//	if (isHit)
//	{
//		// お互いに当たらない最小距離
//		float needDistance = target.Radius + myShape.Radius;
//
//		// 自身から相手に向かう方向ベクトル
//		pRes->m_hitDir = (Math::Vector3(target.Center) - Math::Vector3(myShape.Center));
//		float betweenDistance = pRes->m_hitDir.Length();
//
//		// 重なり量 = お互い当たらない最小距離 - お互いの実際距離
//		pRes->m_overlapDistance = needDistance - betweenDistance;
//
//		pRes->m_hitDir.Normalize();
//
//		// 当たった座標はお互いの球の衝突の中心点
//		pRes->m_hitPos = myShape.Center + pRes->m_hitDir * (myShape.Radius + pRes->m_overlapDistance * 0.5f);
//	}
//
//	return isHit;
//}
//
//bool SphereCollision::Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
//{
//	DirectX::BoundingSphere myShere;
//
//	m_shape.Transform(myShere, world);
//
//	bool isHit = myShere.Intersects(target);
//
//	pRes;
//
//	return isHit;
//}
//
//bool SphereCollision::Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
//{
//	DirectX::BoundingSphere myShape;
//
//	m_shape.Transform(myShape, world);
//
//	float hitDistance = 0.0f;
//
//	bool isHit = myShape.Intersects(target.m_pos, target.m_dir, hitDistance);
//
//	// 判定限界距離を加味
//	isHit &= (target.m_range >= hitDistance);
//
//	// 詳細なしなら結果だけ返す
//	if (!pRes) { return isHit; }
//
//	pRes->m_hit = isHit;
//
//	// 当たった時のみ計算
//	if (isHit)
//	{
//		// レイ発射位置 + レイの当たった位置までのベクトル 
//		pRes->m_hitPos = target.m_pos + target.m_dir * hitDistance;
//
//		pRes->m_hitDir = target.m_dir * (-1);
//
//		pRes->m_overlapDistance = target.m_range - hitDistance;
//	}
//
//	return isHit;
//}
//
//bool AABBCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
//{
//	DirectX::BoundingBox myAABB;
//
//	m_aabb.Transform(myAABB, world);
//
//	bool isHit = myAABB.Intersects(target);
//
//	// 詳細なしなら結果だけ返す
//	if (!pRes) { return isHit; }
//
//	pRes->m_hit = isHit;
//
//	return isHit;
//}
//
//bool ModelCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
//{
//	if (!m_shape) { return false; }
//
//	const std::vector<ModelData::Node>& dataNodes = m_shape->GetDataNodes();
//	const std::vector<ModelWork::Node>& workNodes = m_shape->GetNodes();
//
//	// 各メッシュに押される用の球・押される毎に座標を更新する必要がある
//	DirectX::BoundingSphere pushedSphere = target;
//	// 計算用にFloat3 → Vectorへ変換
//	DirectX::XMVECTOR pushedSphereCenter = DirectX::XMLoadFloat3(&pushedSphere.Center);
//
//	bool isHit = false;
//
//	DirectX::XMVECTOR hitPos = {0};
//
//	for (int index : m_shape->GetCollisionMeshNodeIndices())
//	{
//		const ModelData::Node& dataNode = dataNodes[index];
//		const ModelWork::Node& workNode = workNodes[index];
//
//		if (!dataNode.m_spMesh) { continue; }
//
//		CollisionMeshResult tmpResult;
//		CollisionMeshResult* pTmpResult = pRes ? &tmpResult : nullptr;
//
//		if (!MeshIntersect(*dataNode.m_spMesh, pushedSphere, workNode.m_worldTransform * world, pTmpResult))
//		{
//			continue;
//		}
//
//		if (!pRes) { return true; }
//
//		isHit = true;
//
//		// 重なった分押し戻す
//		pushedSphereCenter = DirectX::XMVectorAdd(pushedSphereCenter, DirectX::XMVectorScale(tmpResult.m_hitDir, tmpResult.m_overlapDistance));
//
//		DirectX::XMStoreFloat3(&pushedSphere.Center, pushedSphereCenter);
//
//		// とりあえず当たった座標で更新
//		hitPos = tmpResult.m_hitPos;
//	}
//
//	if (!pRes) { return false; }
//
//	pRes->m_hit = isHit;
//
//	if (isHit)
//	{
//		// 最後に当たった座標が使用される
//		pRes->m_hitPos = hitPos;
//
//		// 複数のメッシュに押された最終的な位置 - 移動前の位置 = 押し出しベクトル
//		pRes->m_hitDir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pushedSphere.Center), DirectX::XMLoadFloat3(&target.Center));
//
//		pRes->m_overlapDistance = DirectX::XMVector3Length(pRes->m_hitDir).m128_f32[0];
//
//		if (pRes->m_overlapDistance > 1.0f)
//		{
//			std::string mes = "何かがおかしい";
//		}
//
//		pRes->m_hitDir = DirectX::XMVector3Normalize(pRes->m_hitDir);
//	}
//
//	return isHit;
//}
//
//bool ModelCollision::Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
//{
//	if (!m_shape) { return false; }
//
//	const std::vector<ModelData::Node>& dataNodes = m_shape->GetDataNodes();
//	const std::vector<ModelWork::Node>& workNodes = m_shape->GetNodes();
//
//	CollisionMeshResult nearestResult;
//
//	bool isHit = false;
//
//	for (int index : m_shape->GetCollisionMeshNodeIndices())
//	{
//		const ModelData::Node& dataNode = dataNodes[index];
//		const ModelWork::Node& workNode = workNodes[index];
//
//		if (!dataNode.m_spMesh) { continue; }
//
//		CollisionMeshResult tmpResult;
//		CollisionMeshResult* pTmpResult = pRes ? &tmpResult : nullptr;
//
//		if (!MeshIntersect(*dataNode.m_spMesh, target.m_pos, target.m_dir, target.m_range,
//			workNode.m_worldTransform * world, pTmpResult))
//		{
//			continue;
//		}
//
//		if (!pRes) { return true; }
//
//		isHit = true;
//
//		if (tmpResult.m_overlapDistance > nearestResult.m_overlapDistance)
//		{
//			nearestResult = tmpResult;
//		}
//	}
//
//	if (!pRes) { return false; }
//
//	pRes->m_hit = isHit;
//
//	if (isHit)
//	{
//		// 最も近くで当たったヒット情報をコピーする
//		pRes->m_hitPos = nearestResult.m_hitPos;
//
//		pRes->m_hitDir = nearestResult.m_hitDir;
//
//		pRes->m_overlapDistance = nearestResult.m_overlapDistance;
//	}
//
//	return isHit;
//}