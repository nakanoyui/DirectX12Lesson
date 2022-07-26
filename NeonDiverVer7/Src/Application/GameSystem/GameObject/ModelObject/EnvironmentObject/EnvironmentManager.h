#pragma once

#include "EnvironmentObject.h"

class EnvironmentManager
{
public:
	EnvironmentManager() = default;
	~EnvironmentManager() = default;

	void AddObject(std::shared_ptr<EnvironmentObject>spEnvironment);

	const std::list<std::weak_ptr<EnvironmentObject>>&GetEnvironmentObjects()const { return m_wpEnvironmentObjects; }
private:
	std::list<std::weak_ptr<EnvironmentObject>> m_wpEnvironmentObjects;
};