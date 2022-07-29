#pragma once

class DepthStencil
{
public:
	DepthStencil() = default;
	virtual ~DepthStencil();

	/// <summary>
	/// DSV�Ƃ��Ĉ���GPU�o�b�t�@�̐����֐�
	/// </summary>
	/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
	/// <param name="size">�𑜓x</param>
	void CreateBuffer(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size);

	/// <summary>
	/// �Z�b�g����Ă���[�x�o�b�t�@�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�Z�b�g����Ă���[�x�o�b�t�@�̃|�C���^</returns>
	ID3D12Resource* GetResource() { return m_pDepthBuffer; }

	/// <summary>
	/// �Z�b�g����Ă���q�[�v�̃|�C���^��Ԃ��֐�
	/// </summary>
	/// <returns>�Z�b�g����Ă���q�[�v�̃|�C���^</returns>
	ID3D12DescriptorHeap* GetHeap() { return m_pHeap; }

	/// <summary>
	/// �[�x�o�b�t�@�ɏ������܂�Ă���f�[�^������������֐�
	/// </summary>
	void Clear();

protected:
	GraphicsDevice* m_pGraphicsDevice = nullptr;
	ID3D12Resource* m_pDepthBuffer = nullptr;
	ID3D12DescriptorHeap* m_pHeap = nullptr;
};