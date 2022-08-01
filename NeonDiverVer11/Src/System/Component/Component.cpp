#include "Component.h"

#include "Application/Application.h"

Component::Component():
	m_name("none"), m_objectName("GameObject"), m_bEnable(true), m_bDelete(false)
{
}

void Component::SetGameObject(std::shared_ptr<GameObject> wpGameObject)
{
	m_wpGameObject = wpGameObject;
}

const std::shared_ptr<GameObject> Component::GetGameObject()
{
	return m_wpGameObject.lock();
}