#pragma once

#include "ColliderComponent.h"

class SphereColliderComponent :public ColliderComponent
{
public:
	SphereColliderComponent();
	~SphereColliderComponent() {}

	void Init()override;
	void Start()override;
	void Update()override;

	const DirectX::BoundingSphere& GetBoundingSphere() const{ return m_bs; }

	void ImGuiUpdateInspecter()override;
	void PrepareJsonParameter(nlohmann::json& json)override;
private:
	void ReferenceJson()override;
	void CreateCollision()override;

	DirectX::BoundingSphere m_bs;
	Math::Vector3 m_color;
};