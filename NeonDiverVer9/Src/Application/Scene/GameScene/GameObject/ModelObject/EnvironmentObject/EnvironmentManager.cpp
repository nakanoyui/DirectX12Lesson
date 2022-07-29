#include "EnvironmentManager.h"

void EnvironmentManager::AddObject(const std::shared_ptr<EnvironmentObject>& spEnvironment)
{
	m_wpEnvironmentObjects.push_back(spEnvironment);
}