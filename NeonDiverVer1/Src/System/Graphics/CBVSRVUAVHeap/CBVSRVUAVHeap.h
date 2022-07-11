#pragma once

class CBVSRVUAVHeap
{
public:
	CBVSRVUAVHeap() = default;
	~CBVSRVUAVHeap();

	/// <summary>
	/// �f�o�C�X�̃|�C���^���Z�b�g����֐�
	/// </summary>
	/// <param name="pDevice">�f�o�C�X�̃|�C���^</param>
	/// <param name="pCmdList">�R�}���h���X�g�̃|�C���^</param>
	void SetGraphicsDevice(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCmdList);

	/// <summary>
	/// �q�[�v�̐����֐�
	/// </summary>
	/// <param name="useCount">CBV�ASRV�AUAV�̏��Ԃł����̃f�[�^���i�[�ł���悤�ɂ��邩�ݒ肷��l</param>
	void Create(const Math::Vector3& useCount);

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�f�B�X�N���v�^�q�[�v�̃|�C���^</returns>
	ID3D12DescriptorHeap* GetHeap() { return m_pHeap; }

	/// <summary>
	/// CBV�ASRV�AUAV�̏��Ԃł����̃f�[�^���i�[�ł��邩�Ԃ��֐�
	/// </summary>
	/// <returns>�e�f�[�^�̋��e�l</returns>
	Math::Vector3 GetUseCount() { return m_useCount; }

	/// <summary>
	/// �����œn���ꂽGPU�o�b�t�@����ShaderResourceView�𐶐�����֐�
	/// </summary>
	/// <param name="pBuffer">GPUBuffer</param>
	void CreateSRV(ID3D12Resource* pBuffer);

	/// <summary>
	/// �����œn���ꂽGPU�o�b�t�@�Ǝw�肳�ꂽView�̐����f�[�^����ShaderResourceView�𐶐�����֐�
	/// </summary>
	/// <param name="pBuffer">GPUBuffer</param>
	/// <param name="viewDesc">�V�F�[�_�[���\�[�X�r���[���</param>
	void CreateSRV(ID3D12Resource* pBuffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

	/// <summary>
	/// SRV��GPU���A�h���X��Ԃ��֐�
	/// </summary>
	/// <param name="number">�e�N�X�`���ԍ�</param>
	/// <returns>SRV��GPU���A�h���X</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForSRV(int number);

	/// <summary>
	/// ���ɐ��������ׂ�SRV�p�̃f�B�X�N���v�^�q�[�v�̔Ԓn��Ԃ��֐�
	/// </summary>
	/// <returns>���ɐ��������ׂ�SRV�p�̃f�B�X�N���v�^�q�[�v�̔Ԓn</returns>
	int GetNextSrvNumber() { return m_nextSrvDescriptorNum; }

	/// <summary>
	/// �q�[�v���Z�b�g����֐�
	/// </summary>
	void Set();
private:
	ID3D12Device* m_pDevice = nullptr;
	ID3D12GraphicsCommandList* m_pCmdList = nullptr;
	ID3D12DescriptorHeap* m_pHeap = nullptr;
	Math::Vector3 m_useCount;
	int m_incrementSize = 0;
	int m_nextSrvDescriptorNum = 0;
};