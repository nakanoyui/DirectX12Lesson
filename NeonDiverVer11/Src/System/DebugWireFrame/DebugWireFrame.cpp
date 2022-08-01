#include "DebugWireFrame.h"

void DebugWireFrame::CreateLineSphere(const DirectX::BoundingSphere& sphere, float vertexCount, const Math::Color& color)
{
	if (vertexCount <= 2)assert(0&&"’¸“_“ñ‚Â‚Å‰~‚Íì‚é‚±‚Æ‚ª‚Å‚«‚Ü‚¹‚ñB");
	MeshData<LineObject> vert;
	MeshCreater::CreateLineSphere(sphere, vertexCount,color,vert);
	m_spMesh = std::make_shared<Mesh>();
	m_spMesh->Create(&GraphicsDevice::Instance(), vert);
}

void DebugWireFrame::CreateLineCube(const DirectX::BoundingBox& bs, const Math::Color& color)
{
	MeshData<LineObject> vert;
	MeshCreater::CreateLineCube(bs,color,vert);
	m_spMesh = std::make_shared<Mesh>();
	m_spMesh->Create(&GraphicsDevice::Instance(), vert);
}