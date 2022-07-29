#pragma once

#include "Application/Application.h"

// ObjectManager
#include "Application/Scene/GameScene/GameObject/ModelObject/CharacterObject/PlayerObject/PlayerManager.h"
#include "Application/Scene/GameScene/GameObject/ModelObject/CharacterObject/EnemyObject/EnemyManager.h"
#include "Application/Scene/GameScene/GameObject/ModelObject/EnvironmentObject/EnvironmentManager.h"
#include "Application/Scene/GameScene/GameObject/ModelObject/WeaponObject/WeaponManager.h"

#include "System/Manager/CSVManager/CSVManager.h"

class GameObjectFactory
{
public:
	GameObjectFactory() {}
	~GameObjectFactory() = default;

	template<class Class,class Draw>
	void CreateGameObjectParticle(const std::shared_ptr<Draw>& spDraw)
	{
		std::shared_ptr<Class>spObject= std::make_shared<Class>();

		spObject->LoadAsset();
		spObject->Init();

		Application::Instance().GetSceneManager()->GetCurrentScene()->AddGameObject(spObject);
		spDraw->AddDrawList(spObject);
	}

	template<class Class,class ObjectManager,class Draw>
	void CreateGameObjectModel(const std::shared_ptr<ObjectManager>& spManager, const std::shared_ptr<Draw>& spDraw)
	{
		std::shared_ptr<Class>spObject = std::make_shared<Class>();

		spObject->LoadAsset();
		spObject->Init();

		Application::Instance().GetSceneManager()->GetCurrentScene()->AddGameObject(spObject);
		spManager->AddObject(spObject);
		spDraw->AddDrawList(spObject);
	}

	template<class Class,class ObjectManager,class Draw>
	void CreateGameObjectModelFromCSV(const std::shared_ptr<ObjectManager>& spManager, const std::shared_ptr<Draw>& spDraw, const StageData& stageData)
	{
		std::shared_ptr<Class>spObject = std::make_shared<Class>();

		spObject->LoadAsset();
		spObject->SetStageData(stageData);
		spObject->Init();

		Application::Instance().GetSceneManager()->GetCurrentScene()->AddGameObject(spObject);
		spManager->AddObject(spObject);
		spDraw->AddDrawList(spObject);
	}
};