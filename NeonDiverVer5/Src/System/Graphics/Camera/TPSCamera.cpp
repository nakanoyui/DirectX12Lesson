#include "TPSCamera.h"

void TPSCamera::SetCameraMatrix(const Math::Matrix& mWorld)
{
	// キャラクターから注視点までの高さをもとに行列を作成
	Math::Matrix localGazeTrans;
	localGazeTrans = Math::Matrix::CreateTranslation(m_localGazePos);

	// 注視点から実際のカメラまでの奥行を使用して行列作成
	Math::Matrix localTrans;		//移動行列
	localTrans = Math::Matrix::CreateTranslation(m_localPos);

	// カメラの回転行列を作成
	Math::Matrix rotation;	//回転行列

	// 継承によりFPSCameraのpublicが使える
	rotation = GetRotationMatrix();

	// カメラ行列にセット
	Camera::SetCameraMatrix(localTrans * rotation * localGazeTrans * mWorld);
}