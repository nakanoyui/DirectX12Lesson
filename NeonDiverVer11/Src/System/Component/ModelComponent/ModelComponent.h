#pragma once

class ModelComponent :public Component
{
public:
	ModelComponent();
	~ModelComponent() {}

	void Init() override;
	
	void ImGuiUpdateInspecter()override;
	void PrepareJsonParameter(nlohmann::json& json)override;

	const std::shared_ptr<ModelWork>& GetModelWork() { return m_spModelWork; }

private:
	void ReferenceJson()override;

	std::shared_ptr<ModelWork>m_spModelWork = nullptr;
	char m_filePath[ImGuiHelper::InputTextSize] = "";

	std::weak_ptr<AssetManager>m_wpAssetManager;
};