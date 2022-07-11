#include "DepthStencil.h"

DepthStencil::~DepthStencil()
{
	SafeRelease(m_pDepthBuffer);
	SafeRelease(m_pHeap);
}

void DepthStencil::CreateBuffer(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size)
{
	m_pGraphicsDevice = pGraphicsDevice;
	// �f�v�X�o�b�t�@�[�ݒ�
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = (UINT64)size.x;
	resDesc.Height = (UINT64)size.y;
	resDesc.DepthOrArraySize = 1;
	resDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//�f�v�X�o�b�t�@�̃t�H�[�}�b�g�ƍő�[�x�l��ݒ�
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	// �ݒ�����ƂɃf�v�X�o�b�t�@�𐶐�
	result = m_pGraphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&m_pDepthBuffer));
	if (FAILED(result)) assert(0&&"�f�v�X�X�e���V���o�b�t�@�[�̍쐬�Ɏ��s");
	// DSV���i�[���邽�߂̃q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	// DSV�q�[�v�̐���
	result = m_pGraphicsDevice->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_pHeap));
	if (FAILED(result)) assert(0 && "�f�v�X�X�e���V���o�b�t�@�[�̍쐬�Ɏ��s");
	// DSV�̐���
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_pGraphicsDevice->GetDevice()->CreateDepthStencilView(m_pDepthBuffer, &dsvDesc, m_pHeap->GetCPUDescriptorHandleForHeapStart());
}

void DepthStencil::Clear()
{
	m_pGraphicsDevice->GetCmdList()->ClearDepthStencilView(m_pHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}