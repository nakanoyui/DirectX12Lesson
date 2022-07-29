#pragma once

struct LineObject
{
	Math::Vector3 pos;
	unsigned int color = 0xFFFFFFFF;
};

namespace MeshCreater
{
	void Create2DQuad(MeshData<TextureVertex>& meshData, Math::Vector2 size);
	void CreateLineSphere(float size, float vertexCount, const Math::Color& color, MeshData<LineObject>& meshData);
	void CreateLineCube(Math::Vector3 size, const Math::Color& color, MeshData<LineObject>& meshData);
}