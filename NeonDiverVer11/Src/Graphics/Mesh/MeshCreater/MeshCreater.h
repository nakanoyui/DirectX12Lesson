#pragma once

struct LineObject
{
	Math::Vector3 pos;
	unsigned int color = 0xFFFFFFFF;
};

namespace MeshCreater
{
	void Create2DQuad(MeshData<TextureVertex>& meshData, Math::Vector2 size);
	void CreateLineSphere(const DirectX::BoundingSphere& sphere, float vertexCount, const Math::Color& color, MeshData<LineObject>& meshData);
	void CreateLineCube(const DirectX::BoundingBox& bs, const Math::Color& color, MeshData<LineObject>& meshData);
}