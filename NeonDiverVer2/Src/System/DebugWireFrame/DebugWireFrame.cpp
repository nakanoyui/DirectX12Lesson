#include "DebugWireFrame.h"

void DebugWireFrame::CreateLineSphere(float size, float vertexCount, const Math::Vector4& color)
{
	if (vertexCount <= 2)assert(0&&"���_��ŉ~�͍�邱�Ƃ��ł��܂���B");
	MeshData<LineCircle> vert;
	MeshCreater::CreateLineSphere(size, vertexCount,color,vert);
	m_spMesh = std::make_shared<Mesh>();
	m_spMesh->Create(&GraphicsDevice::Instance(), vert);
}