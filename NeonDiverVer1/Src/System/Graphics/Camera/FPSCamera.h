#pragma once

class FPSCamera : public Camera
{
public:

	FPSCamera() {}
	~FPSCamera() {}

	void Init();
	void Update();

	//��]�s����쐬���A�Ԃ��֐�
	const Math::Matrix GetRotationMatrix();

	//Y����](��������])�s���Ԃ��֐�
	const Math::Matrix GetRotationYMatrix() { return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cameraAngle.y)); }

	//���W��ݒ肷��֐�
	void SetLocalPos(Math::Vector3& pos) { m_localPos = pos; }

	//���W���󂯎��֐�
	Math::Vector3 GetLocalPos() { return m_localPos; }

	//�p�x�����ݒ肷��֐�
	void SetClampAngleX(float minAngleX, float maxAngleX)
	{
		m_minAngleX = minAngleX;
		m_maxAngleX = maxAngleX;
	}

	//�p�x�����ݒ肷��֐�
	void SetClampAngleY(float minAngleY, float maxAngleY)
	{
		m_minAngleY = minAngleY;
		m_maxAngleY = maxAngleY;
	}

	//�p����ŃI�[�o�[���C�h(�㏑��)�������Ȃ�virtual�^�ɂ���
	virtual void  SetCameraMatrix(const Math::Matrix& mWorld);//�J�����̍s����쐬

	//���݂̉�]�p�x��Ԃ��֐�(�v���C���[�̉�]�ȂǂɕK�v)
	const Math::Vector3& GetCameraAngle() { return m_cameraAngle; }

	// �}�E�X�̈ړ��ʂ�Ԃ��֐�
	const POINT& GetMouseMove() { return m_mouseMove; }

	//�p���悵���g���Ȃ�
protected:
	Math::Vector3 m_localPos;//���[�J�����W

private:
	Math::Vector3 m_cameraAngle;  //�J������]�p�x�i�[�s��(�I�C���[�p,�f�O���[�p)

	POINT m_postPos;

	POINT m_mouseMove;

	//�p�x�����̒l�i�[�p�ϐ�
	//�����l�͐�������(FLT_MAX��float��max�̒l)
	float m_minAngleX = -FLT_MAX;
	float m_maxAngleX = FLT_MAX;

	float m_minAngleY = -FLT_MAX;
	float m_maxAngleY = FLT_MAX;
};