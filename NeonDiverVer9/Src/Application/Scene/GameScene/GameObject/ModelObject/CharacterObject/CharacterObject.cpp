#include "CharacterObject.h"

#include "Application/Application.h"

void CharacterObject::Update()
{
	m_verticalVector -= Application::Instance().GetSceneManager()->GetCurrentScene()->GetWorldGravity();
}