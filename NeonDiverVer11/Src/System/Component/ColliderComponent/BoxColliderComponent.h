#pragma once

#include "ColliderComponent.h"

class BoxColliderComponent :public ColliderComponent
{
public:
	BoxColliderComponent();
	~BoxColliderComponent() {}

	void Init()override;
	void Start()override;
	void Update()override;

	void ImGuiUpdateInspecter()override;
	void PrepareJsonParameter(nlohmann::json& json)override;
private:
	void ReferenceJson()override;
	void CreateCollision()override;

	DirectX::BoundingBox m_bb;
	Math::Vector3 m_color;
};