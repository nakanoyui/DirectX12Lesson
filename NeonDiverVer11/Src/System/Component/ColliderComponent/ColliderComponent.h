#pragma once

class DebugWireFrame;

class ColliderComponent :public Component
{
public:
	ColliderComponent();
	virtual ~ColliderComponent() {}

	virtual void Init()override;
	virtual void Start()override;
	void DrawLate() override;

	const std::unique_ptr<Collider>&GetCollider()const { return m_upCollider; }

protected:
	virtual void CreateCollision() {}

	std::unique_ptr<Collider>m_upCollider = nullptr;
	std::unique_ptr<DebugWireFrame>m_upDebugWireFrame = nullptr;
	std::weak_ptr<TransformComponent>m_wpTransformComponent;
	std::weak_ptr<DrawManager>m_wpDrawManager;
	std::weak_ptr<AssetManager>m_wpAssetManager;
};