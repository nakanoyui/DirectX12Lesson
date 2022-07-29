#pragma once

class FPSCamera;
class TPSCamera;

struct CameraInfo {
	float fov = 60.0f;
	float maxRange = 2000.0f;
	float minRange = 0.01f;
	float aspectRatio = GraphicsDevice::Instance().GetAspectRatio();
	Math::Vector3 localPos = Math::Vector3::Zero;
	Math::Vector3 localGazePos = Math::Vector3::Zero;
	Math::Vector2 clampAngleX = Math::Vector2(-45.0f, 45.0f);
	Math::Vector2 clampAngleY = Math::Vector2(-45.0f, 45.0f);
};

class CameraManager
{
public:
	void CreateCamera(const CameraInfo& cameraInfo, const std::string_view cameraName);

	const std::shared_ptr<TPSCamera> GetCamera(const std::string_view& cameraName);
	const std::shared_ptr<TPSCamera>& GetNowCamera() { return m_spNowCamera; }

	void SetCamera(const std::shared_ptr<TPSCamera>& camera);
	void SetMatrix(const Math::Matrix& mat);

	void Update();

	void Set2D(int setDescriptorIndex);
	void Set3D(int setDescriptorIndex);
private:
	std::map<std::string_view, std::shared_ptr<TPSCamera>> m_spCameras;

	std::shared_ptr<TPSCamera>	m_spNowCamera;
	Math::Matrix m_mWorld;
};