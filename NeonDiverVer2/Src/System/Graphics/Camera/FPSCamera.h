#pragma once

class FPSCamera : public Camera
{
public:

	FPSCamera() {}
	~FPSCamera() {}

	void Init();
	void Update();

	//回転行列を作成し、返す関数
	const Math::Matrix GetRotationMatrix();

	//Y軸回転(横方向回転)行列を返す関数
	const Math::Matrix GetRotationYMatrix() { return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_cameraAngle.y)); }

	//座標を設定する関数
	void SetLocalPos(Math::Vector3& pos) { m_localPos = pos; }

	//座標を受け取る関数
	Math::Vector3 GetLocalPos() { return m_localPos; }

	//角度制御を設定する関数
	void SetClampAngleX(float minAngleX, float maxAngleX)
	{
		m_minAngleX = minAngleX;
		m_maxAngleX = maxAngleX;
	}

	//角度制御を設定する関数
	void SetClampAngleY(float minAngleY, float maxAngleY)
	{
		m_minAngleY = minAngleY;
		m_maxAngleY = maxAngleY;
	}

	//継承先でオーバーライド(上書き)したいならvirtual型にする
	virtual void  SetCameraMatrix(const Math::Matrix& mWorld);//カメラの行列を作成

	//現在の回転角度を返す関数(プレイヤーの回転などに必要)
	const Math::Vector3& GetCameraAngle() { return m_cameraAngle; }

	// マウスの移動量を返す関数
	const POINT& GetMouseMove() { return m_mouseMove; }

	//継承先しか使えない
protected:
	Math::Vector3 m_localPos;//ローカル座標

private:
	Math::Vector3 m_cameraAngle;  //カメラ回転角度格納行列(オイラー角,デグリー角)

	POINT m_postPos;

	POINT m_mouseMove;

	//角度制限の値格納用変数
	//初期値は制限無し(FLT_MAXがfloatのmaxの値)
	float m_minAngleX = -FLT_MAX;
	float m_maxAngleX = FLT_MAX;

	float m_minAngleY = -FLT_MAX;
	float m_maxAngleY = FLT_MAX;
};