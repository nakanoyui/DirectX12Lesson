#pragma once

#include "Application/main.h"
#include "Application/GameSystem/GameSystem.h"

#include "System/Manager/CSVManager/CSVManager.h"

class GameObjectFactory
{
public:
	GameObjectFactory() = default;
	~GameObjectFactory() = default;

	template<class Class,class Draw>
	void CreateGameObjectParticle(std::shared_ptr<Draw>& spDraw)
	{
		std::shared_ptr<Class>spObject;
		spObject = std::make_shared<Class>();

		spObject->LoadAsset();
		spObject->Init();

		Application::Instance().GetGameSystem()->AddGameObject(spObject);
		spDraw->AddDrawList(spObject);
	}

	template<class Class,class ObjectManager,class Draw>
	void CreateGameObjectModel(std::shared_ptr<ObjectManager>& spManager,std::shared_ptr<Draw>& spDraw)
	{
		std::shared_ptr<Class>spObject;
		spObject = std::make_shared<Class>();

		spObject->LoadAsset();
		spObject->Init();

		Application::Instance().GetGameSystem()->AddGameObject(spObject);
		spManager->AddObject(spObject);
		spDraw->AddDrawList(spObject);
	}

	template<class Class,class ObjectManager,class Draw>
	void CreateGameObjectModelFromCSV(std::shared_ptr<ObjectManager>& spManager,std::shared_ptr<Draw>& spDraw,StageData& stageData)
	{
		std::shared_ptr<Class>spObject;
		spObject = std::make_shared<Class>();

		spObject->LoadAsset();
		spObject->SetStageData(stageData);
		spObject->Init();

		Application::Instance().GetGameSystem()->AddGameObject(spObject);
		spManager->AddObject(spObject);
		spDraw->AddDrawList(spObject);
	}
};