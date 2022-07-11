#pragma once

#include "FPSCamera.h"

class TPSCamera : public FPSCamera
{
public:
	TPSCamera() {}
	~TPSCamera() {}

	// 渡されたキャラのワールド行列をもとに行列を作成する関数
	void SetCameraMatrix(const Math::Matrix& mWorld)override;

	// カメラ位置の座標設定
	void SetLocalGazePos(const Math::Vector3& localPos)
	{
		m_localGazePos = localPos;
	}

private:
	// 注視点へのローカル座標(キャラクターの原点からの注視点までの相対座標)
	Math::Vector3 m_localGazePos;
};