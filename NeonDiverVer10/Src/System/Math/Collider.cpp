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
//	// �����m�̓����蔻��
//	bool isHit = myShape.Intersects(target);
//
//	// �ڍׂȂ��Ȃ猋�ʂ����Ԃ�
//	if (!pRes) { return isHit; }
//
//	pRes->m_hit = isHit;
//
//	// �����������̂݌v�Z
//	if (isHit)
//	{
//		// ���݂��ɓ�����Ȃ��ŏ�����
//		float needDistance = target.Radius + myShape.Radius;
//
//		// ���g���瑊��Ɍ����������x�N�g��
//		pRes->m_hitDir = (Math::Vector3(target.Center) - Math::Vector3(myShape.Center));
//		float betweenDistance = pRes->m_hitDir.Length();
//
//		// �d�Ȃ�� = ���݂�������Ȃ��ŏ����� - ���݂��̎��ۋ���
//		pRes->m_overlapDistance = needDistance - betweenDistance;
//
//		pRes->m_hitDir.Normalize();
//
//		// �����������W�͂��݂��̋��̏Փ˂̒��S�_
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
//	// ������E����������
//	isHit &= (target.m_range >= hitDistance);
//
//	// �ڍׂȂ��Ȃ猋�ʂ����Ԃ�
//	if (!pRes) { return isHit; }
//
//	pRes->m_hit = isHit;
//
//	// �����������̂݌v�Z
//	if (isHit)
//	{
//		// ���C���ˈʒu + ���C�̓��������ʒu�܂ł̃x�N�g�� 
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
//	// �ڍׂȂ��Ȃ猋�ʂ����Ԃ�
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
//	// �e���b�V���ɉ������p�̋��E������閈�ɍ��W���X�V����K�v������
//	DirectX::BoundingSphere pushedSphere = target;
//	// �v�Z�p��Float3 �� Vector�֕ϊ�
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
//		// �d�Ȃ����������߂�
//		pushedSphereCenter = DirectX::XMVectorAdd(pushedSphereCenter, DirectX::XMVectorScale(tmpResult.m_hitDir, tmpResult.m_overlapDistance));
//
//		DirectX::XMStoreFloat3(&pushedSphere.Center, pushedSphereCenter);
//
//		// �Ƃ肠���������������W�ōX�V
//		hitPos = tmpResult.m_hitPos;
//	}
//
//	if (!pRes) { return false; }
//
//	pRes->m_hit = isHit;
//
//	if (isHit)
//	{
//		// �Ō�ɓ����������W���g�p�����
//		pRes->m_hitPos = hitPos;
//
//		// �����̃��b�V���ɉ����ꂽ�ŏI�I�Ȉʒu - �ړ��O�̈ʒu = �����o���x�N�g��
//		pRes->m_hitDir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pushedSphere.Center), DirectX::XMLoadFloat3(&target.Center));
//
//		pRes->m_overlapDistance = DirectX::XMVector3Length(pRes->m_hitDir).m128_f32[0];
//
//		if (pRes->m_overlapDistance > 1.0f)
//		{
//			std::string mes = "��������������";
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
//		// �ł��߂��œ��������q�b�g�����R�s�[����
//		pRes->m_hitPos = nearestResult.m_hitPos;
//
//		pRes->m_hitDir = nearestResult.m_hitDir;
//
//		pRes->m_overlapDistance = nearestResult.m_overlapDistance;
//	}
//
//	return isHit;
//}