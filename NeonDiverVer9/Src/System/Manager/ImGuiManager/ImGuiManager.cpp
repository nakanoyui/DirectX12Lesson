#include "ImGuiManager.h"

#include "Application/Application.h"

ImGuiManager::~ImGuiManager()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Init()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	GraphicsDevice::Instance().GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pHeap));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(Application::Instance().m_window.GetWndHandle());
	ImGui_ImplDX12_Init(GraphicsDevice::Instance().GetDevice().Get(), 2
		, GraphicsDevice::Instance().GetCurrentFrameBuffer()->GetDesc().Format, m_pHeap.Get()
		, m_pHeap->GetCPUDescriptorHandleForHeapStart(), m_pHeap->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::PreDraw()
{
	GraphicsDevice::Instance().GetCBufferAllocater()->ResetCurrentUseNumber();
	GraphicsDevice::Instance().GetCBVSRVUAVHeap()->Set();

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::Draw()
{
	ImGui::Render();
	ID3D12DescriptorHeap* ppHeaps[] = { m_pHeap.Get() };
	GraphicsDevice::Instance().GetCmdList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GraphicsDevice::Instance().GetCmdList().Get());
}