#include "CameraManager.h"

void CameraManager::SetCamera(const std::shared_ptr<TPSCamera>& spCamera)
{
	m_spNowCamera = spCamera;
}

void CameraManager::SetMatrix(const Math::Matrix& mat)
{
	m_mWorld = mat;
}

void CameraManager::Update()
{
	if (m_spNowCamera)
	{
		// カメラ情報の更新
		m_spNowCamera->Update();

		// 現在の座標をもとに移動行列の作成
		Math::Matrix transMat;

		transMat = Math::Matrix::CreateTranslation(m_mWorld.Translation());

		// カメラにはカメラの回転があるので、ここではプレイヤーの位置だけ渡せばいい
		m_spNowCamera->SetCameraMatrix(transMat);
	}
}

void CameraManager::Set2D(int setDescriptorIndex)
{
	if (m_spNowCamera)
	{
		Math::Matrix mProj2D = DirectX::XMMatrixOrthographicLH(Constant::ScreenSize.x, Constant::ScreenSize.y, 0, 1);

		GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
		(setDescriptorIndex, ConstantBuffer::cbCamera{ Math::Matrix::Identity,mProj2D,Math::Matrix::Identity,Math::Matrix::Identity,m_spNowCamera->GetCameraMatrix().Translation() });
	}
}

void CameraManager::Set3D(int setDescriptorIndex)
{
	if (m_spNowCamera)
	{
		auto mBillBoard = m_spNowCamera->GetViewMatrix().Invert();
		mBillBoard._41 = 0;
		mBillBoard._42 = 0;
		mBillBoard._43 = 0;
		mBillBoard._44 = 1;

		GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
		(setDescriptorIndex, ConstantBuffer::cbCamera{ m_spNowCamera->GetViewMatrix(),m_spNowCamera->GetProjMatrix(),m_spNowCamera->GetViewMatrix() * m_spNowCamera->GetProjMatrix(),
			mBillBoard,m_spNowCamera->GetCameraMatrix().Translation() });
	}
}