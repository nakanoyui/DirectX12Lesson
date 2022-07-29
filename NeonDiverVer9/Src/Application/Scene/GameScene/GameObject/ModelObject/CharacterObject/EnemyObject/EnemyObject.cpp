#include "EnemyObject.h"

#include "Application/Application.h"

void EnemyObject::Init()
{
	m_wpPlayerManager = Application::Instance().GetSceneManager()->GetCurrentScene()->GetPlayerManager();
}