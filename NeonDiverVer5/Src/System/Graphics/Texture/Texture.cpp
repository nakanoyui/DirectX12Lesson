#include "Texture.h"

Texture::~Texture()
{
}

bool Texture::Load(const std::string& filepath, GraphicsDevice* pGraphicsDevice)
{
	m_pDevice = pGraphicsDevice;

	std::string filePath = filepath;
	wchar_t wFilePath[128];
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wFilePath, _countof(wFilePath));

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImage;
	const DirectX::Image* pImage;

	auto result = DirectX::LoadFromWICFile(wFilePath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
	if (FAILED(result))assert(0 && "テクスチャのロード作成失敗");
	pImage = scratchImage.GetImage(0, 0, 0);
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Format = metadata.format;
	resDesc.Width = (UINT64)metadata.width;
	resDesc.Height = (UINT)metadata.height;
	resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	resDesc.MipLevels = (UINT16)metadata.mipLevels;
	resDesc.SampleDesc.Count = 1;

	result = m_pDevice->GetDevice()->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pTexBuffer));
	if (FAILED(result))assert(0 && "テクスチャリソース作成失敗");
	result = m_pTexBuffer->WriteToSubresource(0, nullptr, pImage->pixels, (UINT)pImage->rowPitch, (UINT)pImage->slicePitch);
	if (FAILED(result))assert(0 && "テクスチャバッファー作成失敗");
	m_srvNumber = m_pDevice->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	m_pDevice->GetCBVSRVUAVHeap()->CreateSRV(m_pTexBuffer.Get());

	MeshData<TextureVertex>	texVert;
	MeshCreater::Create2DQuad(texVert, { float(metadata.width),float(metadata.height)});
	m_spMesh = std::make_shared<Mesh>();
	m_spMesh->Create(&GraphicsDevice::Instance(), texVert);

	return true;
}

void Texture::Set(int descIndex)
{
	m_pDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, m_pDevice->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(m_srvNumber));
}