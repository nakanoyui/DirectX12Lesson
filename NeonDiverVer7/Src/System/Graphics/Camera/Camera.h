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

	// カメラ行列のセット・カメラ行列からビュー行列を生成
	virtual void SetCameraMatrix(const Math::Matrix& mCam);

	// ビュー行列のセット・ビュー行列からカメラ行列を生成
	virtual void SetViewMatrix(const Math::Matrix& mView);

	const DirectX::SimpleMath::Matrix& GetCamera() { return m_mCam; }
	DirectX::SimpleMath::Matrix& WorkCamera() { return m_mCam; }

	//// カメラからのレイ情報を生成
	//static void GenerateCameraRayInfoFromClientPos(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange);

	//// スクリーン座標を(2D座標)をワールド座標(3D座標)に変換する
	//void ConvertScreenToWorldDetail(const Math::Vector2& pos, Math::Vector3& result);

	//// ワールド座標(3D座標)をスクリーン座標(2D座標)に変換する
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