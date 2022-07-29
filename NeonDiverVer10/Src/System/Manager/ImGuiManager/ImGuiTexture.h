#pragma once

class ImGuiManager;

class ImGuiTexture
{
private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ImGuiTexture(ImGuiManager* pManager);

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	UINT GetSRVNumber() { return m_srvNumber; }

	void LoadTexture(std::string_view textureName);
private:
	ComPtr<ID3D12Resource> m_pTexBuffer = nullptr;

	UINT m_srvNumber;
	int m_width;
	int m_height;

	ImGuiManager* m_pManager = nullptr;
};