#include "TPSCamera.h"

void TPSCamera::SetCameraMatrix(const Math::Matrix& mWorld)
{
	// �L�����N�^�[���璍���_�܂ł̍��������Ƃɍs����쐬
	Math::Matrix localGazeTrans;
	localGazeTrans = Math::Matrix::CreateTranslation(m_localGazePos);

	// �����_������ۂ̃J�����܂ł̉��s���g�p���čs��쐬
	Math::Matrix localTrans;		//�ړ��s��
	localTrans = Math::Matrix::CreateTranslation(m_localPos);

	// �J�����̉�]�s����쐬
	Math::Matrix rotation;	//��]�s��

	// �p���ɂ��FPSCamera��public���g����
	rotation = GetRotationMatrix();

	// �J�����s��ɃZ�b�g
	Camera::SetCameraMatrix(localTrans * rotation * localGazeTrans * mWorld);
}