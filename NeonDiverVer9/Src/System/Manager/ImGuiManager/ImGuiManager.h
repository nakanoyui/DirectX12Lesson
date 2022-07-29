#pragma once

class ImGuiManager
{
public:
	~ImGuiManager();

	void Init();
	void PreDraw();
	void Draw();

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeap = nullptr;
};