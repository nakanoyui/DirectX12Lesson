#pragma once

class ModelObject :public GameObject
{
public:
	ModelObject() = default;
	virtual ~ModelObject() = default;

	virtual void LoadAsset() {}

	// アクセサ
	virtual const std::shared_ptr<ModelWork>& GetModelWork() { return m_spModelWork; }
	virtual std::shared_ptr<ModelWork>& WorkModelWork() { return m_spModelWork; }

	virtual const std::weak_ptr<Json>& GetJson() { return m_wpJson; }

protected:
	std::shared_ptr<ModelData>	m_spModelData = nullptr;
	std::shared_ptr<ModelWork>	m_spModelWork = nullptr;
	std::weak_ptr<ShaderManager>m_wpShaderManager;
	std::weak_ptr<JsonManager> m_wpJsonManager;
	std::weak_ptr<Json>m_wpJson;

	std::unique_ptr<DebugWireFrame> m_upDebugWireFrame = nullptr;
};