#include "EnvironmentManager.h"

void EnvironmentManager::AddObject(std::shared_ptr<EnvironmentObject>spEnvironment)
{
	m_wpEnvironmentObjects.push_back(spEnvironment);
}