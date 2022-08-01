#pragma once

#include "../Component.h"

class RigidBodyComponent:public Component
{
public:
	RigidBodyComponent();
	~RigidBodyComponent() {}
	
	void Init()override;
	void Start()override;
	void Update()override;

	void ImGuiUpdateInspecter()override;

	void PrepareJsonParameter(nlohmann::json& json)override;
private:
	void ReferenceJson()override;
	int m_debugBump;
	std::weak_ptr<SphereColliderComponent> m_wpMyColliderComponent;
	std::list<std::weak_ptr<ColliderComponent>>m_wpTargetColliderComponent;
	std::unordered_map<std::string, std::list<Collider::CollisionResult>>m_bumpResults;
};