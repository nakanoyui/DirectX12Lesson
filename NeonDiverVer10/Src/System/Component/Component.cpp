#include "Component.h"

#include "Application/Application.h"

Component::Component():
	m_name("none"), m_objectName("GameObject"), m_bEnable(true)
{
}

void Component::SetGameObject(GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
}

const GameObject* Component::GetGameObject()
{
	return m_pGameObject;
}