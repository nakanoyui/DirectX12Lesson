#pragma once

class RendererComponent:public Component
{
public:
	RendererComponent();
	virtual ~RendererComponent() {}

	virtual void Init()override;
	virtual void Start()override;
	virtual void Update()override;
	virtual void Draw() {}

	void PrepareJsonParameter(nlohmann::json& json)override {}

protected:
	void ReferenceJson()override {}

	std::string m_shaderName;

	std::weak_ptr<TransformComponent>m_wpTransformComponent;
	std::weak_ptr<ModelComponent>m_wpModelComponent;
	std::weak_ptr<AssetManager>m_wpAssetManager;
	std::weak_ptr<DrawManager>m_wpDrawManager;
};