#pragma once

#include "FPSCamera.h"

class TPSCamera : public FPSCamera
{
public:
	TPSCamera() {}
	~TPSCamera() {}

	// �n���ꂽ�L�����̃��[���h�s������Ƃɍs����쐬����֐�
	void SetCameraMatrix(const Math::Matrix& mWorld)override;

	// �J�����ʒu�̍��W�ݒ�
	void SetLocalGazePos(const Math::Vector3& localPos)
	{
		m_localGazePos = localPos;
	}

private:
	// �����_�ւ̃��[�J�����W(�L�����N�^�[�̌��_����̒����_�܂ł̑��΍��W)
	Math::Vector3 m_localGazePos;
};