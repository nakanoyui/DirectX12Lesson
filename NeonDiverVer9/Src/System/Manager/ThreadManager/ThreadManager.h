#pragma once

#include "../CSVManager/CSVManager.h"

class ThreadManager
{
public:
	//template<class Class, class ObjectManager, class DrawManager>
	//void RunThread(std::unique_ptr<GameObjectFactory>& upGameObjectFactory, std::shared_ptr<ObjectManager>& spObjectManager, std::shared_ptr<DrawManager>& spDrawManager);
	//
	//template<class Class, class ObjectManager, class DrawManager>
	//void RunThread(std::unique_ptr<GameObjectFactory>& upGameObjectFactory, std::shared_ptr<ObjectManager>& spObjectManager, std::shared_ptr<DrawManager>& spDrawManager, StageData& stageData);

	/// <summary>
	/// ìØä˙Çë“Ç¬ä÷êî
	/// </summary>
	void WaitForSynchronization();

private:
	std::list<std::shared_ptr<std::thread>> m_spTreads;
};

//template<class Class, class ObjectManager, class DrawManager>
//inline void ThreadManager::RunThread(std::unique_ptr<GameObjectFactory>& upGameObjectFactory, std::shared_ptr<ObjectManager>& spObjectManager, std::shared_ptr<DrawManager>& spDrawManager)
//{
//	std::function<void(GameObjectFactory&, std::shared_ptr<Class>, std::shared_ptr<ObjectManager>, std::shared_ptr<DrawManager>)>spCreateFunction = &GameObjectFactory::CreateGameObjectModel<Class, ObjectManager, DrawManager>;
//	std::shared_ptr<Class>spObject;
//	std::shared_ptr<std::thread> spThread = std::make_shared<std::thread>(spCreateFunction, *upGameObjectFactory, spObject, spObjectManager, spDrawManager);
//	m_spTreads.push_back(spThread);
//}
//
//template<class Class, class ObjectManager, class DrawManager>
//inline void ThreadManager::RunThread(std::unique_ptr<GameObjectFactory>& upGameObjectFactory, std::shared_ptr<ObjectManager>& spObjectManager, std::shared_ptr<DrawManager>& spDrawManager, StageData& stageData)
//{
//	std::function<void(GameObjectFactory&, std::shared_ptr<Class>, std::shared_ptr<ObjectManager>, std::shared_ptr<DrawManager>, StageData)>spCreateFunction = &GameObjectFactory::CreateGameObjectModelFromCSV<Class, ObjectManager, DrawManager>;
//	std::shared_ptr<Class>spObject;
//	std::shared_ptr<std::thread> spThread = std::make_shared<std::thread>(spCreateFunction, *upGameObjectFactory, spObject, spObjectManager, spDrawManager, stageData);
//	m_spTreads.push_back(spThread);
//}