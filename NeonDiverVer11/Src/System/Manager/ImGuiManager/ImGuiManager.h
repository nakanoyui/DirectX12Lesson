#pragma once

class ImGuiManager
{
private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	~ImGuiManager();

	void Init();
	void PreDraw();
	void Draw();

	void IncrementOffset() { m_descriptorOffset++; }
	UINT GetNextOffset() { return m_descriptorOffset; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT offset);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT offset);

private:
	ComPtr<ID3D12DescriptorHeap> m_pHeap = nullptr;

	UINT m_incrementSize;
	UINT m_descriptorOffset;
};