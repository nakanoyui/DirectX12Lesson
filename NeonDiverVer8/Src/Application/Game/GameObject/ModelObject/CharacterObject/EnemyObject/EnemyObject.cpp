#include "EnemyObject.h"

#include "Application/Game/GameManager.h"

void EnemyObject::Init()
{
	m_wpPlayerManager = Application::Instance().GetGameProcess()->GetGameManager()->GetPlayerManager();
}