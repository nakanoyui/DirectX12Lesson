#pragma once

#include "Component.h"

class ModelComponent :public Component
{
public:
	ModelComponent();
	~ModelComponent() {}

	void Init() override;
	void Draw()override;

	void ImGuiUpdateInspecter()override;
	void PrepareJsonParameter(nlohmann::json& json)override;

private:
	void ReferenceJson()override;

	std::shared_ptr<ModelWork>m_spModelWork = nullptr;
	std::string m_filePath;
	std::string m_shaderName;

	std::weak_ptr<TransformComponent>m_wpTransformComponent;
	std::weak_ptr<AssetManager>m_wpAssetManager;
};