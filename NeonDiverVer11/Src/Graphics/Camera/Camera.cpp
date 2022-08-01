#include "Camera.h"

void Camera::SetProjectionMatrix(float fov, float maxRange, float minRange, float aspectRatio)
{
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(fov*ToRadians, aspectRatio, minRange, maxRange);


	SetProjectionMatrix(m_mProj);

	m_nearClippingDistance = minRange;
	m_farClippingDistance = maxRange;
}

void Camera::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj)
{
	m_mProj = rProj;

	m_nearClippingDistance = -(m_mProj._43 / m_mProj._33);
	m_farClippingDistance = -(m_mProj._43 / (m_mProj._33 - 1));
}

void Camera::SetFocus(float focusDist, float focusForeRange, float focusBackRange)
{
	m_focusDistance = focusDist;
	m_focusForeRange = focusForeRange;
	m_focusBackRange = focusBackRange;
}

void Camera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam)
{
	// カメラ行列セット
	m_mCam = mCam;

	// カメラ行列からビュー行列を算出
	m_mView = mCam.Invert();
}

void Camera::SetViewMatrix(const DirectX::SimpleMath::Matrix& mView)
{
	// ビュー行列セット
	m_mView = mView;

	// ビュー行列からカメラ行列を算出
	m_mCam = mView.Invert();
}