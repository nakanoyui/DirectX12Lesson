#pragma once

class FPSCamera;
class TPSCamera;

struct CameraInfo 
{
	float Fov = 60.0f;
	float MaxRange = 1000.0f;
	float MinRange = 0.01f;
	float AspectRatio = GraphicsDevice::Instance().GetAspectRatio();
	Math::Vector3 LocalPos = Math::Vector3::Zero;
	Math::Vector3 LocalGazePos = Math::Vector3::Zero;
	Math::Vector2 ClampAngleX = Math::Vector2(-45.0f, 45.0f);
	Math::Vector2 ClampAngleY = Math::Vector2(-45.0f, 45.0f);
};

class CameraManager
{
public:
	const std::shared_ptr<TPSCamera>& GetNowCamera() { return m_spNowCamera; }

	void SetCamera(const std::shared_ptr<TPSCamera>& spCamera);
	void SetMatrix(const Math::Matrix& mat);

	void Update();

	void Set2D(int setDescriptorIndex);
	void Set3D(int setDescriptorIndex);
private:
	std::shared_ptr<TPSCamera>	m_spNowCamera;
	Math::Matrix m_mWorld;
};