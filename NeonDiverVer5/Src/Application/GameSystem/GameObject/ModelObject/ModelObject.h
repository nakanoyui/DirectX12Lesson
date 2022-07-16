#pragma once

class ModelObject :public GameObject
{
public:
	ModelObject() = default;
	virtual ~ModelObject() = default;

	virtual void LoadAsset() {}

	// アクセサ
	virtual const std::shared_ptr<ModelWork>& GetModelWork() { return m_spModelWork; }

protected:
	std::shared_ptr<ModelData>	m_spModelData = nullptr;
	std::shared_ptr<ModelWork>	m_spModelWork = nullptr;
	std::weak_ptr<ShaderManager>m_wpShaderManager;
	std::weak_ptr<ModelManager> m_wpModelManager;

	std::unique_ptr<DebugWireFrame> m_upDebugWireFrame = nullptr;
};