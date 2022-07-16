using namespace DirectX;

bool MeshIntersect(const Mesh& mesh, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir,
	float rayRange, const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult)
{
	//--------------------------------------------------------
	// ���C�̋t�s��
	//--------------------------------------------------------

	// ���f���̋t�s��Ń��C��ϊ�
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, matrix);

	// ���C�̔���J�n�ʒu���t�ϊ�
	DirectX::XMVECTOR rayPosInv = XMVector3TransformCoord(rayPos, invMat);

	// ���C�̕������t�ϊ�
	DirectX::XMVECTOR rayDirInv = XMVector3TransformNormal(rayDir, invMat);

	float dirLength = DirectX::XMVector3Length(rayDirInv).m128_f32[0];

	// ���C�̕����x�N�g���̒���=�g�嗦�Ŕ�����E������␳
	// ���t�s��Ɋg�k�������Ă���ƁA���C�̒������ς�邽��
	// ���C�������������W����̋����Ɋg�k�����f����Ă��܂��̂�
	// ����p�̍ő勗���ɂ��g�k�𔽉f�����Ă���
	float limitRange = rayRange * dirLength;

	// ���p���C�Ƃ��Đ������������߂ɂ͒������P�łȂ���΂Ȃ�Ȃ��̂Ő��K��
	rayDirInv = DirectX::XMVector3Normalize(rayDirInv);

	//--------------------------------------------------------
	// �u���[�h�t�F�C�Y
	// �@��r�I�y�ʂ�AABB vs ���C�Ȕ���ŁA
	// �@�����炩�Ƀq�b�g���Ȃ��ꍇ�́A����ȍ~�̔���𒆎~
	//--------------------------------------------------------
	{
		// AABB vs ���C
		float AABBdist = FLT_MAX;
		DirectX::BoundingBox aabb;
		mesh.GetBoundingBox().Transform(aabb, matrix);

		if (aabb.Intersects(rayPos, rayDir, AABBdist) == false) { return false; }

		// �ő勗���O�Ȃ�͈͊O�Ȃ̂Œ��~
		if (AABBdist > limitRange) { return false; }
	}

	//--------------------------------------------------------
	// �i���[�t�F�C�Y
	// �@���C vs �S�Ă̖�
	//--------------------------------------------------------

	// �q�b�g����
	bool isHit = false;
	float closestDist = FLT_MAX;

	// DEBUG�r���h�ł����x���ێ����邽�߁A�ʕϐ��ɏE���Ă���
	const MeshFace* pFaces = &mesh.GetFaces()[0];
	UINT faceNum = static_cast<UINT>(mesh.GetFaces().size());

	// �S�Ă̖�(�O�p�`)
	for (UINT faceIdx = 0; faceIdx < faceNum; ++faceIdx)
	{
		// �O�p�`���\������R�̒��_��Index
		const UINT* idx = pFaces[faceIdx].Idx;

		// ���C�ƎO�p�`�̔���
		float triDist = FLT_MAX;
		bool bHit = DirectX::TriangleTests::Intersects(rayPosInv, rayDirInv,
			mesh.GetVertexPositions()[idx[0]],
			mesh.GetVertexPositions()[idx[1]],
			mesh.GetVertexPositions()[idx[2]],
			triDist
		);

		// �q�b�g����
		if (bHit == false) { continue; }

		// �ő勗���ȓ��̏ꍇ
		if (triDist <= limitRange)
		{
			isHit = true;

			// �����蔻����s�����f���̍s��̊g�嗦���e������̂ŁA���̉e����ł�����
			triDist /= dirLength;

			// �߂��ق����c��
			if ((triDist) < closestDist)
			{
				closestDist = triDist;		// �������X�V
			}
		}
	}

	if (!pResult) { return isHit; }

	pResult->m_hit = isHit;

	// ���U���g�Ɍ��ʂ��i�[
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
	// �u���[�h�t�F�C�Y
	// �@�������̂��߁A�܂��͋��E�{�b�N�X(AABB)�Ŕ���
	// �@���̒i�K�Ńq�b�g���Ă��Ȃ��Ȃ�A�ڍׂȔ��������K�v�Ȃ�
	//------------------------------------------
	{
		// ���b�V����AABB�����ɁA�s��ŕϊ�����AABB���쐬
		DirectX::BoundingBox aabb;
		mesh.GetBoundingBox().Transform(aabb, matrix);

		if (aabb.Intersects(sphere) == false) { return false; }
	}

	//------------------------------------------
	// �i���[�t�F�C�Y
	// �@���ƃ��b�V���Ƃ̏ڍה���
	//------------------------------------------

	// �P�ł��q�b�g������true
	bool isHit = false;

	// DEBUG�r���h�ł����x���ێ����邽�߁A�ʕϐ��ɏE���Ă���
	const auto* pFaces = &mesh.GetFaces()[0];
	UINT faceNum = static_cast<UINT>(mesh.GetFaces().size());
	const Math::Vector3* vertices = &mesh.GetVertexPositions()[0];

	// ���b�V���̋t�s��ŁA���̒��S���W��ϊ�(���b�V���̍��W�n�֕ϊ������)
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, matrix);
	DirectX::XMVECTOR spherePos = XMLoadFloat3(&sphere.Center);
	spherePos = XMVector3TransformCoord(spherePos, invMat);

	// ���a�̓��(����̍������p)
	float radiusSq = sphere.Radius * sphere.Radius;	// ���a�̂Q��

	// �s��̊e���̊g�嗦���v�Z���Ă���
	DirectX::XMVECTOR scale;
	scale.m128_f32[0] = DirectX::XMVector3Length(matrix.r[0]).m128_f32[0];
	scale.m128_f32[1] = DirectX::XMVector3Length(matrix.r[1]).m128_f32[0];
	scale.m128_f32[2] = DirectX::XMVector3Length(matrix.r[2]).m128_f32[0];
	scale.m128_f32[3] = 0;

	DirectX::XMVECTOR finalPos = spherePos;
	DirectX::XMVECTOR nearestPoint;

	// �S�Ă̖ʂƔ���
	// ������̓��b�V���̃��[�J����Ԃōs����
	for (UINT fi = 0; fi < faceNum; fi++)
	{
		DirectX::XMVECTOR nearPoint;

		// �_ �� �O�p�` �̍ŋߐړ_�����߂�
		KdPointToPolygon(finalPos,
			vertices[pFaces[fi].Idx[0]],
			vertices[pFaces[fi].Idx[1]],
			vertices[pFaces[fi].Idx[2]],
			nearPoint);

		// �ŋߐړ_�����@�x�N�g�������߂�
		DirectX::XMVECTOR vToCenter = finalPos - nearPoint;

		// XYZ�����ʁX�̑傫���Ŋg�k����Ă��Ԃ̏ꍇ�A�����ό`���Ă��ԂȂ��ߐ��m�Ȕ��a���킩��Ȃ��B
		// ������scale��������XYZ���̃X�P�[�����ϓ��ȍ��W�n�֕ϊ�����
		vToCenter *= scale;

		// �ŋߐړ_�����a�ȓ��̏ꍇ�́A�Փ˂��Ă���
		if (DirectX::XMVector3LengthSq(vToCenter).m128_f32[0] <= radiusSq)
		{
			nearPoint = XMVector3TransformCoord(nearPoint, matrix);

			// �����߂��v�Z
			// �߂荞��ł���Ԃ�̃x�N�g�����v�Z
			DirectX::XMVECTOR vPush = DirectX::XMVector3Normalize(vToCenter);

			vPush *= sphere.Radius - DirectX::XMVector3Length(vToCenter).m128_f32[0];

			// �g�k���l���������W�n�֖߂�
			vPush /= scale;

			// ���̍��W���ړ�������
			finalPos += vPush;

			nearestPoint = nearPoint;

			isHit = true;
		}
	}

	if (!pResult) { return isHit; }

	pResult->m_hit = isHit;

	// ���U���g�Ɍ��ʂ��i�[
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

// �_ vs �O�p�`�Ƃ̍ŋߐړ_�����߂�
void KdPointToPolygon(const XMVECTOR& p, const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c, DirectX::XMVECTOR& outPt)
{
	// ���Q�l:[����]�u�Q�[���v���O���~���O�̂��߂̃��A���^�C���Փ˔���v

	// p��a�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR ab = b - a;
	XMVECTOR ac = c - a;
	XMVECTOR ap = p - a;

	float d1 = XMVector3Dot(ab, ap).m128_f32[0];//ab.Dot(ap);
	float d2 = XMVector3Dot(ac, ap).m128_f32[0];//ac.Dot(ap);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		outPt = a;	// �d�S���W(1,0,0)
		return;
	}

	// p��b�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR bp = p - b;
	float d3 = XMVector3Dot(ab, bp).m128_f32[0];//ab.Dot(bp);
	float d4 = XMVector3Dot(ac, bp).m128_f32[0];//ac.Dot(bp);

	if (d3 >= 0.0f && d4 <= d3)
	{
		outPt = b;	// �d�S���W(0,1,0)
		return;
	}

	// p��ab�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����p��ab��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;

	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		outPt = a + ab * v;	// �d�S���W(1-v,v,0)
		return;
	}

	// p��c�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR cp = p - c;
	float d5 = XMVector3Dot(ab, cp).m128_f32[0];//ab.Dot(cp);
	float d6 = XMVector3Dot(ac, cp).m128_f32[0];;//ac.Dot(cp);

	if (d6 >= 0.0f && d5 <= d6)
	{
		outPt = c;	// �d�S���W(0,0,1)
		return;
	}

	// p��ac�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����p��ac��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		outPt = a + ac * w;	// �d�S���W(1-w,0,w)
		return;
	}

	// p��bc�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����p��bc��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		outPt = b + (c - b) * w;	// �d�S���W(0,1-w,w)
		return;
	}

	// p�͖ʗ̈�̒��ɂ���BQ�����̏d�S���W(u,v,w)��p���Čv�Z
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	outPt = a + ab * v + ac * w;	// = u*a + v*b + w*c, u = va*demon = 1.0f - v - w
}