#include "CameraManager.h"

void CameraManager::CreateCamera(const CameraInfo& cameraInfo, const std::string_view cameraName)
{
	auto it = m_spCameras.find(cameraName);

	// 無い場合
	if (it == m_spCameras.end())
	{
		std::shared_ptr<TPSCamera>spTPSCamera = std::make_shared<TPSCamera>();
		spTPSCamera->SetProjectionMatrix(cameraInfo.fov, cameraInfo.maxRange, cameraInfo.minRange, cameraInfo.aspectRatio);
		spTPSCamera->SetLocalPos(Math::Vector3(cameraInfo.localPos));
		spTPSCamera->SetLocalGazePos(Math::Vector3(cameraInfo.localGazePos));
		spTPSCamera->SetClampAngleX(cameraInfo.clampAngleX.x, cameraInfo.clampAngleX.y);
		spTPSCamera->SetClampAngleY(cameraInfo.clampAngleY.x, cameraInfo.clampAngleY.y);
		m_spCameras[cameraName] = spTPSCamera;
	}
}

const std::shared_ptr<TPSCamera>CameraManager::GetCamera(const std::string_view& cameraName)
{
	auto it = m_spCameras.find(cameraName.data());

	if (it != m_spCameras.end())
	{
		return it->second;
	}

	assert(0 && "Cameraが存在しません");
	return nullptr;
}

void CameraManager::SetCamera(std::shared_ptr<TPSCamera>& camera)
{
	m_spNowCamera = camera;
}

void CameraManager::SetMatrix(Math::Matrix& mat)
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
	Math::Matrix mProj2D = DirectX::XMMatrixOrthographicLH(Constant::ScreenSize.x, Constant::ScreenSize.y, 0, 1);

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
	(setDescriptorIndex, ConstantBuffer::cbCamera{ Math::Matrix::Identity,mProj2D,Math::Matrix::Identity,Math::Matrix::Identity,m_spNowCamera->GetCameraMatrix().Translation() });
}

void CameraManager::Set3D(int setDescriptorIndex)
{
	auto mBillBoard = m_spNowCamera->GetViewMatrix().Invert();
	mBillBoard._41 = 0;
	mBillBoard._42 = 0;
	mBillBoard._43 = 0;
	mBillBoard._44 = 1;

	GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttach
	(setDescriptorIndex, ConstantBuffer::cbCamera{ m_spNowCamera->GetViewMatrix(),m_spNowCamera->GetProjMatrix(),m_spNowCamera->GetViewMatrix()* m_spNowCamera->GetProjMatrix(),
		mBillBoard,m_spNowCamera->GetCameraMatrix().Translation() });
}