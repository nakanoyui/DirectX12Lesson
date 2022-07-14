#pragma once

class DebugWireFrame
{
public:
	DebugWireFrame() = default;
	~DebugWireFrame() = default;

	void CreateLineSphere(float size, float vertexCount, const Math::Color& color);
	void CreateLineCube(const Math::Vector3 size,const Math::Color&color);

	const std::shared_ptr<Mesh>& GetMesh()const { return m_spMesh; }
private:
	std::shared_ptr<Mesh>	m_spMesh = nullptr;
};