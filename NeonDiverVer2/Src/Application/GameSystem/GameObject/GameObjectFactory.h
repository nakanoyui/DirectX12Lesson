#pragma once

class GameObjectFactory
{
public:
	GameObjectFactory() = default;
	~GameObjectFactory() = default;

	template<class Class,class ObjectManager,class Draw>
	void CreateGameObjectModel(std::shared_ptr<Class>&spObject,std::shared_ptr<ObjectManager>& spManager,std::shared_ptr<Draw>& spDraw)
	{
		spObject = std::make_shared<Class>();

		spObject->LoadAsset();
		spObject->Init();

		Application::Instance().GetGameSystem()->AddGameObject(spObject);
		spManager->AddObject(spObject);
		spDraw->AddDrawList(spObject);
	}
};