#include "CharacterObject.h"

void CharacterObject::Update()
{
	m_verticalVector -= Application::Instance().GetGameSystem()->GetWorldGravity();
}