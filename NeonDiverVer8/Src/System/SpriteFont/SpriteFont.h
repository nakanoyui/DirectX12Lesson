#pragma once

class SpriteFont
{
public:
	SpriteFont() = default;
	~SpriteFont() = default;

	void Init(GraphicsDevice* pGraphicsDevice);

	void Begin();
	void End();

	const std::unique_ptr<DirectX::SpriteFont>& GetSpriteFont()const { return m_upSpriteFont; }
	const std::unique_ptr<DirectX::SpriteBatch>& GetSpriteBatch()const { return m_upSpriteBatch; }

private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> CreateOneDescriptorHeapSRV();

	ComPtr<ID3D12DescriptorHeap> m_pHeapForSpriteFont = nullptr;
	std::unique_ptr<DirectX::GraphicsMemory> m_upGraphicsMemory = nullptr;
	std::unique_ptr<DirectX::SpriteFont> m_upSpriteFont = nullptr;
	std::unique_ptr<DirectX::SpriteBatch> m_upSpriteBatch = nullptr;

	GraphicsDevice* m_pGraphicsDevice = nullptr;
};