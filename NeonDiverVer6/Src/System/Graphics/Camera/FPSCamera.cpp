#include "FPSCamera.h"

#include "Application/main.h"

void FPSCamera::Init()
{

}

void FPSCamera::Update()
{
	POINT nowCursorPos;
	GetCursorPos(&nowCursorPos);

	m_mouseMove.x = nowCursorPos.x - m_postPos.x;
	m_mouseMove.y = nowCursorPos.y - m_postPos.y;

	m_postPos.x = static_cast<float>(nowCursorPos.x);
	m_postPos.y = static_cast<float>(nowCursorPos.y);

	if (!Application::Instance().GetKeyManager()->GetKeyState(VK_RBUTTON,true))return;

	//‚»‚Ì‚Ü‚Ü‚ÌˆÚ“®—Ê‚¾‚Æ‰ñ“]—Ê‚ª‘å‚«‚·‚¬‚é‚½‚ß’²ß‚µ‚½‚à‚Ì‚ğ•Ê‚Ì•Ï”‚ÉŠi”[
	POINT correctMouseMove;
	correctMouseMove.x = static_cast<LONG>(m_mouseMove.x);
	correctMouseMove.y = static_cast<LONG>(m_mouseMove.y);

	// ’²®‚µ‚½‰ñ“]—Ê‚ğs—ñ‚ÌŠe—v‘f‚É‘«‚µ‚±‚Ş
	// ‚±‚±‚ÌX‚ÆY‚Í‹t‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢
	m_cameraAngle.x += correctMouseMove.y * 0.25f;
	m_cameraAngle.y += correctMouseMove.x * 0.25f;

	m_cameraAngle.x = std::clamp(m_cameraAngle.x, m_minAngleX, m_maxAngleX);

	if (m_minAngleY + m_maxAngleY)
	{
		m_cameraAngle.y = std::clamp(m_cameraAngle.y, m_minAngleY, m_maxAngleY);
	}
}

const Math::Matrix FPSCamera::GetRotationMatrix()
{
	return Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_cameraAngle.y),
		DirectX::XMConvertToRadians(m_cameraAngle.x),
		DirectX::XMConvertToRadians(m_cameraAngle.z)
	);
}

void FPSCamera::SetCameraMatrix(const Math::Matrix& mWorld)
{
	Math::Matrix localPos;

	localPos = Math::Matrix::CreateTranslation(m_localPos);

	Math::Matrix rotation;

	rotation = GetRotationMatrix();

	Camera::SetCameraMatrix(rotation * localPos * mWorld);
}