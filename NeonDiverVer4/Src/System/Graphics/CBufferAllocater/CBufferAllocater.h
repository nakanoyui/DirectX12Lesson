#pragma once

class CBufferAllocater
{
public:
	~CBufferAllocater();

	/// <summary>
	/// �O���t�B�b�N�X�f�o�C�X�̃|�C���^���Z�b�g����֐�
	/// </summary>
	/// <param name="pGraphicsDevice">�O���t�B�b�N�X�f�o�C�X�̃|�C���^</param>
	void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̃|�C���^���Z�b�g����֐�
	/// </summary>
	/// <param name="pHeap">�q�[�v�̃|�C���^</param>
	void SetHeap(CBVSRVUAVHeap* pHeap);

	/// <summary>
	/// �A���P�[�^�̐����֐�
	/// </summary>
	void Create();

	/// <summary>
	/// �g�p���Ă���o�b�t�@�̔ԍ���������������֐�
	/// </summary>
	void ResetCurrentUseNumber();

	/// <summary>
	/// �Z�b�g���郌�W�X�^�ԍ��A�Z�b�g����f�[�^�̃|�C���^�A�f�[�^�T�C�Y���w�肵��
	/// ���ꂩ��g���萔�o�b�t�@�̔Ԓn�̌v�Z���s�����̂��f�[�^��
	/// �V�F�[�_�Ƀf�[�^���Z�b�g����֐��A��ɔz��f�[�^���Z�b�g�������Ƃ��Ɏg�p
	/// </summary>
	/// <param name="descIndex">���W�X�^�ԍ�</param>
	/// <param name="pData">�f�[�^�|�C���^</param>
	/// <param name="size">�f�[�^�T�C�Y</param>
	void BindAndAttachData(int descIndex, const void* pData, int size);

	/// <summary>
	/// �Z�b�g���郌�W�X�^�ԍ��A�f�[�^���w�肵�Ă��ꂩ��g���萔�o�b�t�@�̔Ԓn�̌v�Z���s�����̂��f�[�^���V�F�[�_�ɃZ�b�g����֐�
	/// </summary>
	/// <typeparam name="T">�Z�b�g����f�[�^�̌^</typeparam>
	/// <param name="descIndex">���W�X�^�ԍ�</param>
	/// <param name="data">�f�[�^</param>
	template<typename T>
	void BindAndAttach(int descIndex, const T& data);

private:
	GraphicsDevice* m_pGraphicsDevice = nullptr;
	CBVSRVUAVHeap* m_pHeap = nullptr;
	ID3D12Resource* m_pBuffer = nullptr;
	struct { char buf[256]; }*m_pMappedBuffer = nullptr;
	int m_currentUseNumber = 0;
};

template<typename T>
inline void CBufferAllocater::BindAndAttach(int descIndex, const T& data)
{
	if (!m_pHeap)return;

	// data�T�C�Y��256�A���C�����g���Čv�Z
	int sizeAligned = (sizeof(T) + 0xff) & ~0xff;
	// 256byte�������g�p���邩�A���C�����g�������ʂ�256�Ŋ����Čv�Z
	int useValue = sizeAligned / 0x100;

	// ���ݎg���I������Ă���ԍ��ƍ�����g���e�ʂ��q�[�v�̗e�ʂ𒴂��Ă���ꍇ�̓��^�[��
	if (m_currentUseNumber + useValue > (int)m_pHeap->GetUseCount().x)
	{
		assert(0 && "�g�p�ł���q�[�v�e�ʂ𒴂��܂���");
		return;
	}

	int top = m_currentUseNumber;

	// �擪�A�h���X�Ɏg�����̃|�C���^�𑫂���memcpy
	memcpy(m_pMappedBuffer + top, &data, sizeof(T));

	// �r���[������Ēl���V�F�[�_�[�ɃA�^�b�`
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc = {};
	cbDesc.BufferLocation = m_pBuffer->GetGPUVirtualAddress() + (UINT64)top * 0x100;
	cbDesc.SizeInBytes = sizeAligned;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_pHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += (UINT64)m_pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_currentUseNumber;
	m_pGraphicsDevice->GetDevice()->CreateConstantBufferView(&cbDesc, cpuHandle);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = m_pHeap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += (UINT64)m_pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_currentUseNumber;

	m_pGraphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, gpuHandle);

	m_currentUseNumber += useValue;
}