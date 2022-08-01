#include "Scene.h"

void Scene::RemoveGameObject(const std::string_view& objectName)
{
	for (auto it = m_spGameObjects.begin(); it != m_spGameObjects.end();)
	{
		if ((*it)->GetName() == objectName)
		{
			(*it)->Remove((*it)->GetName());
			m_spGameObjects.erase(it);
			break;
		}

		it++;
	}
}
