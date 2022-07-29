#include "GraphicsDevice.h"

#include "RenderTexture/RenderTexture.h"
#include "DepthStencil/DepthStencil.h"
#include "System/GPUParticle/GPUParticleManager.h"

bool GraphicsDevice::Init(HWND hWnd, int w, int h)
{
	SetViewport(Math::Vector2((float)w, (float)h));

	if (!CreateFactory())
	{
		assert(0 && "�t�@�N�g���[�쐬���s");
		Release();
		return false;
	}

#ifdef _DEBUG
	EnableDebugLayer();
#endif

	if (!CreateDevice())
	{
		assert(0 && "Direct3D12�f�o�C�X�쐬���s");
		Release();
		return false;
	}

	if (!CreateCommandList())
	{
		assert(0 && "�R�}���h���X�g�̍쐬���s");
		Release();
		return false;
	}

	if (!CreateSwapChain(hWnd, w, h))
	{
		assert(0 && "�X���b�v�`�F�C���쐬���s");
		Release();
		return false;
	}

	if (!CreateSwapChainRTV())
	{
		assert(0 && "�X���b�v�`�F�C����RTV�쐬���s");
		Release();
		return false;
	}

	if (!CreateDsv())
	{
		assert(0 && "�f�v�X�X�e���V���쐬���s");
		Release();
		return false;
	}

	if (!CreateFence())
	{
		assert(0 && "�t�F���X�̍쐬���s");
		Release();
		return false;
	}

	m_spCBVSRVUAVHeap = std::make_shared<CBVSRVUAVHeap>();
	m_spCBufferAllocater = std::make_shared<CBufferAllocater>();

	m_spCBVSRVUAVHeap->SetGraphicsDevice(m_pDevice.Get(), m_pCmdList.Get());
	m_spCBVSRVUAVHeap->Create({ 10000,256,0 });

	m_spCBufferAllocater->SetGraphicsDevice(this);
	m_spCBufferAllocater->SetHeap(m_spCBVSRVUAVHeap.get());
	m_spCBufferAllocater->Create();

	m_pWhiteTex = CreateWhiteTexture();

	m_spGPUParticleManager = std::make_shared<GPUParticleManager>(this);

	return true;
}

void GraphicsDevice::ScreenFlip()
{
	if (m_upSwapChainRT->GetCurrentResourceState() != D3D12_RESOURCE_STATE_PRESENT)
	{
		m_upSwapChainRT->SetCurrentResourceState(D3D12_RESOURCE_STATE_PRESENT);
		SetResourceBarrier(m_pFrameBuffers[m_pSwapChain->GetCurrentBackBufferIndex()], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	}

	m_pCmdList->Close();

	ID3D12CommandList* cmdlists[] = { m_pCmdList.Get() };
	m_pCmdQueue->ExecuteCommandLists(1, cmdlists);

	WaitForCommandQueue();

	m_pCmdAllocator->Reset();						// �R�}���h�A���P�[�^�[�̏�����
	m_pCmdList->Reset(m_pCmdAllocator.Get(), nullptr);		// �R�}���h���X�g�̏�����

	m_pSwapChain->Present(1, 0);
}

void GraphicsDevice::WaitForCommandQueue()
{
	m_pCmdQueue->Signal(m_pFence.Get(), ++m_fenceVal);
	if (m_pFence->GetCompletedValue() != m_fenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);		// �C�x���g�n���h���̎擾
		if (!event)
		{
			assert(0 && "�C�x���g�G���[�A�A�v���P�[�V�������I�����܂�");
		}
		m_pFence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);		// �C�x���g����������܂ő҂�������
		CloseHandle(event);							// �C�x���g�n���h�������
	}
}

void GraphicsDevice::Release()
{
	SafeRelease(m_pFrameBuffers[0]);
	SafeRelease(m_pFrameBuffers[1]);
}

void GraphicsDevice::EnableDebugLayer()
{
	ID3D12Debug* debugLayer = nullptr;

	D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	debugLayer->EnableDebugLayer();		// �f�o�b�O���C���[��L���ɂ���
	debugLayer->Release();
}

bool GraphicsDevice::CreateFactory()
{
	UINT flagsDXGI = 0;
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
	auto result = CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_pDxgiFactory.ReleaseAndGetAddressOf()));

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateDevice()
{
	// �g�pGPU�I���i�O���t�B�b�N��p�������̗e�ʂ̑�������D��
	ComPtr<IDXGIAdapter> pSelectAdapter = nullptr;
	SIZE_T maxVideoMemory = 0;
	for (UINT index = 0; 1; ++index)
	{
		ComPtr<IDXGIAdapter> pAdapter = nullptr;
		HRESULT ret = m_pDxgiFactory->EnumAdapters(index, &pAdapter);

		if (ret == DXGI_ERROR_NOT_FOUND) { break; }
		DXGI_ADAPTER_DESC desc = {};
		pAdapter->GetDesc(&desc);

		if (maxVideoMemory < desc.DedicatedVideoMemory)
		{
			maxVideoMemory = desc.DedicatedVideoMemory;
			pSelectAdapter = pAdapter;
		}
	}

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// Direct3D�f�o�C�X�̏�����
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : levels)
	{
		if (D3D12CreateDevice(pSelectAdapter.Get(), lv, IID_PPV_ARGS(&m_pDevice)) == S_OK)
		{
			featureLevel = lv;
			break;		// �����\�ȃo�[�W���������������烋�[�v�ł��؂�
		}
	}

	return true;
}

bool GraphicsDevice::CreateCommandList()
{
	auto hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAllocator));

	if (FAILED(hr))
	{
		return false;
	}

	hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&m_pCmdList));

	if (FAILED(hr))
	{
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;		// �^�C���A�E�g�Ȃ�
	cmdQueueDesc.NodeMask = 0;								// �A�_�v�^�[��1�����g��Ȃ��Ƃ���0�ł���
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// �v���C�I���e�B�͓��Ɏw��Ȃ�
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;		// �R�}���h���X�g�ƍ��킹��

	// �L���[����
	hr = m_pDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_pCmdQueue));

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateSwapChain(HWND hwnd, const int& width, const int& height)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = width;
	swapchainDesc.Height = height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// �t���b�v��͑��₩�ɔj��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// �E�B���h�E�ƃt���X�N���[���؂�ւ��\

	auto result = m_pDxgiFactory->CreateSwapChainForHwnd(m_pCmdQueue.Get(), hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)m_pSwapChain.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateSwapChainRTV()
{
	m_upSwapChainRT = std::make_unique<RenderTarget>();

	m_pFrameBuffers.resize(2);

	HRESULT result;
	for (int i = 0; i < (int)m_pFrameBuffers.size(); ++i)
	{
		result = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pFrameBuffers[i]));
	}

	if (!m_upSwapChainRT->Create(this, &m_pFrameBuffers))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateDsv()
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = (UINT64)m_viewport.Width;
	resDesc.Height = (UINT64)m_viewport.Height;
	resDesc.DepthOrArraySize = 1;
	resDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	// �f�v�X�o�b�t�@�̃t�H�[�}�b�g�ƍő�l��ݒ�
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	// �ݒ�����ƂɃf�v�X�o�b�t�@�𐶐�
	result = m_pDevice->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&m_pDepthBuffer));
	if (FAILED(result))
	{
		return false;
	}
	// DSV���i�[���邽�߂̃q�[�v��ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	// DSV�q�[�v�̐���
	result = m_pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_pDsvHeap));
	if (FAILED(result))
	{
		return false;
	}
	// DSV�̐���
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get(), &dsvDesc, m_pDsvHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool GraphicsDevice::CreateFence()
{
	auto result = m_pDevice->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void GraphicsDevice::ClearRenderTarget(const Math::Vector4& color, bool clearFlag, RenderTarget* pRenderTarget, DepthStencil* pDepthStencil)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	// pRenderTarget��nullptr�̏ꍇ�̓o�b�N�o�b�t�@�������_�[�^�[�Q�b�g�Ƃ��邩�[�x�l��������
	if (pRenderTarget == nullptr)
	{
		pRenderTarget = m_upSwapChainRT.get();
		if (pRenderTarget->GetCurrentResourceState() != D3D12_RESOURCE_STATE_RENDER_TARGET)
		{
			pRenderTarget->SetCurrentResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET);
			SetResourceBarrier(m_pFrameBuffers[m_pSwapChain->GetCurrentBackBufferIndex()], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

		rtvHandle = pRenderTarget->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += (UINT64)m_pSwapChain->GetCurrentBackBufferIndex() * m_pDevice->GetDescriptorHandleIncrementSize(pRenderTarget->GetHeap()->GetDesc().Type);
		if (pDepthStencil)
		{
			dsvHandle = pDepthStencil->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			dsvHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
		}
		m_pCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	}
	else
	{
		pRenderTarget->Prepare();
		rtvHandle = pRenderTarget->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		if (pDepthStencil)
		{
			dsvHandle = pDepthStencil->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			dsvHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
		}
		m_pCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	}

	if (clearFlag)
	{
		// pRenderTarget��color�̐F�œh��Ԃ�
		Math::Vector4 setColor = color;
		if (setColor.x > 1)setColor.x /= 255.0f;
		if (setColor.y > 1)setColor.y /= 255.0f;
		if (setColor.z > 1)setColor.z /= 255.0f;
		if (setColor.w > 1)setColor.w /= 255.0f;

		float rgba[] = { setColor.x,setColor.y,setColor.z,setColor.w };
		m_pCmdList->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);

		if (pDepthStencil)
		{
			pDepthStencil->Clear();
		}
		else
		{
			ClearDepthStencil();
		}
	}

	D3D12_VIEWPORT viewport = m_viewport;
	D3D12_RECT rect = m_rect;
	D3D12_RESOURCE_DESC resDesc = pRenderTarget->GetResource()->GetDesc();

	viewport.Height = (float)resDesc.Height;
	viewport.Width = (float)resDesc.Width;

	rect.bottom = (int)resDesc.Height;
	rect.right = (int)resDesc.Width;

	m_pCmdList->RSSetViewports(1, &viewport);
	m_pCmdList->RSSetScissorRects(1, &rect);
	m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GraphicsDevice::ClearDepthStencil()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_pCmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void GraphicsDevice::SetResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = resource;
	barrier.Transition.StateAfter = after;
	barrier.Transition.StateBefore = before;
	m_pCmdList->ResourceBarrier(1, &barrier);
}

void GraphicsDevice::SetViewport(const Math::Vector2& size)
{
	m_viewport = { 0,0,(float)size.x,(float)size.y,0,1.0f };
	m_rect = { 0,0,(int)size.x,(int)size.y };
	m_aspectRatio = size.x / size.y;
}

Microsoft::WRL::ComPtr<ID3D12Resource> GraphicsDevice::CreateWhiteTexture()
{
	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDesc.Width = 4;
	resDesc.Height = 4;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;

	ComPtr<ID3D12Resource> pWhiteBuff = nullptr;
	auto result = m_pDevice->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&pWhiteBuff));

	if (FAILED(result))
	{
		return nullptr;
	}

	std::vector<unsigned char>data(4 * 4 * 4);
	std::fill(data.begin(), data.end(), 0xff);

	result = pWhiteBuff->WriteToSubresource(0, nullptr, data.data(), 4 * 4, static_cast<UINT>(data.size()));

	return pWhiteBuff;
}
