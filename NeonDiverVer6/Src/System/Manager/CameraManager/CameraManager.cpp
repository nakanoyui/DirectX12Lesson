#include "CameraManager.h"

void CameraManager::CreateCamera(const CameraInfo& cameraInfo, const std::string_view cameraName)
{
	auto it = m_spCameras.find(cameraName);

	// �����ꍇ
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

	assert(0 && "Camera�����݂��܂���");
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

void CameraManager::UpdateMatrix()
{
	if (m_spNowCamera)
	{
		// �J�������̍X�V
		m_spNowCamera->Update();

		// ���݂̍��W�����ƂɈړ��s��̍쐬
		Math::Matrix transMat;

		transMat = Math::Matrix::CreateTranslation(m_mWorld.Translation());

		// �J�����ɂ̓J�����̉�]������̂ŁA�����ł̓v���C���[�̈ʒu�����n���΂���
		m_spNowCamera->SetCameraMatrix(transMat);
	}
}