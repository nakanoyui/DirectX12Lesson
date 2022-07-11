#pragma once

class DebugWireFrame
{
public:
	DebugWireFrame() = default;
	~DebugWireFrame() = default;

	void CreateLineSphere(float size, float vertexCount, const Math::Vector4& color);

	const std::shared_ptr<Mesh>& GetMesh()const { return m_spMesh; }
private:
	std::shared_ptr<Mesh>	m_spMesh = nullptr;
};