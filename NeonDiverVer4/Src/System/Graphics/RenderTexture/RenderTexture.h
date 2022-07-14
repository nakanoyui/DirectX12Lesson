#pragma once

#include "../Texture/Texture.h"

class RenderTexture :public Texture, public RenderTarget
{
public:
	RenderTexture() = default;
	~RenderTexture();
	void Prepare()override;
	void Create(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size, const Math::Vector4& color = { 0,0,0,1 });
	void Set(int descIndex) override;
	void EndDraw();
};