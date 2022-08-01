#pragma once

class PlayerControllerComponent :public ScriptComponent
{
public:
	PlayerControllerComponent();
	~PlayerControllerComponent() {}

	void Init()override;
	void Start()override;
	void Update()override;

	void ImGuiUpdateInspecter()override;

	void PrepareJsonParameter(nlohmann::json& json) override;
private:
	void ReferenceJson()override;

	Math::Vector3 m_velocity;
	float m_amountOfMovement;
	std::weak_ptr<TransformComponent>m_wpTransformComponent;
	std::weak_ptr<KeyManager>m_wpKeyManager;
	std::weak_ptr<CameraManager>m_wpCameraManager;
};