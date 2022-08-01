#pragma once

class CameraComponent :public Component
{
public:
	CameraComponent();
	~CameraComponent() {}

	void Init() override;
	void Start() override;
	void Update()override;

	void ImGuiUpdateInspecter()override;
	void PrepareJsonParameter(nlohmann::json& json)override;

private:
	void ReferenceJson()override;
	void SetCameraInfo();

	CameraInfo m_camInfo;

	std::shared_ptr<TPSCamera>m_spCamera = nullptr;
	std::weak_ptr<CameraManager> m_wpCameraManager;

	std::weak_ptr<TransformComponent>m_wpTransformComponent;
};