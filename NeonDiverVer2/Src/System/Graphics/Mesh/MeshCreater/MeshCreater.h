#pragma once

struct LineCircle
{
	Math::Vector3 pos;
	unsigned int color = 0xFFFFFFFF;
};

namespace MeshCreater
{
	void Create2DQuad(MeshData<TextureVertex>& meshData, Math::Vector2 size);
	void CreateLineSphere(float size, float vertexCount, const Math::Color& color, MeshData<LineCircle>& meshData);
}