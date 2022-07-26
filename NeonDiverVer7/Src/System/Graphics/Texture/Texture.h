#pragma once

class Texture
{
private:
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Texture() = default;
	virtual ~Texture();
	bool Load(const std::string& filepath, GraphicsDevice* pGraphicsDevice, bool bCreateMesh = false);
	virtual void Set(int descIndex);
	ComPtr<ID3D12Resource> GetTexBuffer() { return m_pTexBuffer; }
	void SetTexBuffer(ComPtr<ID3D12Resource> resource, GraphicsDevice* pGraphicsDevice) { m_pDevice = pGraphicsDevice; m_pTexBuffer = resource; }
	const std::shared_ptr<Mesh>& GetMesh() { return m_spMesh; }
protected:
	ComPtr<ID3D12Resource> m_pTexBuffer = nullptr;
	int				m_srvNumber = 0;
	std::shared_ptr<Mesh>	m_spMesh = nullptr;
private:
	GraphicsDevice* m_pDevice = nullptr;		// ‘½dŒp³‚Å–¼‘O”í‚è‚ğ–h‚®‚½‚ß‚É–¼‘O‚ğ•Ï‚¦‚Ä‚é
};