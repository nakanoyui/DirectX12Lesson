#pragma once

class DebugWireFrame
{
public:
	DebugWireFrame() = default;
	~DebugWireFrame() = default;

	void CreateLineSphere(const DirectX::BoundingSphere& sphere, float vertexCount, const Math::Color& color);
	void CreateLineCube(const DirectX::BoundingBox& bs,const Math::Color&color);
	void CreateLineModel(const std::vector<ModelVertex>* modelVerteies);

	const std::shared_ptr<Mesh>& GetMesh()const { return m_spMesh; }

private:
	std::shared_ptr<Mesh>	m_spMesh = nullptr;
};