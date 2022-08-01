#pragma once

#include "ColliderComponent.h"

class ModelColliderComponent :public ColliderComponent
{
public:
	ModelColliderComponent();
	~ModelColliderComponent() {}

	void Init()override;
	void Start()override;

	void ImGuiUpdateInspecter()override;
	void PrepareJsonParameter(nlohmann::json& json)override;
private:
	void ReferenceJson()override;
	void CreateCollision()override;

	std::weak_ptr<ModelComponent>m_wpModelComponent;
};