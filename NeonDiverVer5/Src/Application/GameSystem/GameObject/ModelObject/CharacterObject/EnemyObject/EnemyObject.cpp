#include "EnemyObject.h"

void EnemyObject::Init()
{
	m_wpPlayerManager = Application::Instance().GetGameSystem()->GetPlayerManager();
}