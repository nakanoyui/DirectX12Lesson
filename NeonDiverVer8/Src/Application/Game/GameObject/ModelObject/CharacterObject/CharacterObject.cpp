#include "CharacterObject.h"

#include "Application/Game/GameManager.h"

void CharacterObject::Update()
{
	m_verticalVector -= Application::Instance().GetGameProcess()->GetGameManager()->GetWorldGravity();
}