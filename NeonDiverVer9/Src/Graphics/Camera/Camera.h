#pragma once

class Camera
{
public:
	Camera()=default;
	~Camera()=default;

	void SetProjectionMatrix(float fov, float maxRange = 2000, float minRange = 0.01f, float aspectRatio = GraphicsDevice::Instance().GetAspectRatio());
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj);

	void SetFocus(float focusDist, float focusForeRange, float focusBackRange);

	inline const Math::Matrix& GetCameraMatrix() const { return m_mCam; }

	inline const Math::Matrix& GetViewMatrix() const { return m_mView; }

	inline const Math::Matrix& GetProjMatrix() const { return m_mProj; }

	// �J�����s��̃Z�b�g�E�J�����s�񂩂�r���[�s��𐶐�
	virtual void SetCameraMatrix(const Math::Matrix& mCam);

	// �r���[�s��̃Z�b�g�E�r���[�s�񂩂�J�����s��𐶐�
	virtual void SetViewMatrix(const Math::Matrix& mView);

	const DirectX::SimpleMath::Matrix& GetCamera() { return m_mCam; }
	DirectX::SimpleMath::Matrix& WorkCamera() { return m_mCam; }

	//// �J��������̃��C���𐶐�
	//static void GenerateCameraRayInfoFromClientPos(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange);

	//// �X�N���[�����W��(2D���W)�����[���h���W(3D���W)�ɕϊ�����
	//void ConvertScreenToWorldDetail(const Math::Vector2& pos, Math::Vector3& result);

	//// ���[���h���W(3D���W)���X�N���[�����W(2D���W)�ɕϊ�����
	//void ConvertWorldToScreenDetail(const Math::Vector3& pos, Math::Vector3& result);

protected:

	Math::Matrix	m_mCam;

	Math::Matrix	m_mView;

	Math::Matrix	m_mProj;

	float m_nearClippingDistance = 0.0f;
	float m_farClippingDistance = 2000.0f;

	float m_focusDistance = 0.0f;
	float m_focusForeRange = 0.0f;
	float m_focusBackRange = 2000.0f;
};