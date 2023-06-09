#include "ImguiManager.h"
#include "WinAPI.h"
#include "DirectXInit.h"
#include "Texture.h"

GUI::GUI(const char* name)
{
	this->name = name;
}

void GUI::Begin(Vector2 pos, Vector2 size)
{
	if (setPosFlag == false)
	{
		setPosFlag = true;
		ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	}
	ImGui::Begin(name);
}

void GUI::End()
{
	ImGui::End();
}

void ImguiManager::Initialize()
{
	WinAPI* winapi = WinAPI::Get();
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* tManager = TextureManager::Get();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.Fonts->AddFontDefault();

	//imgui‚ÌF‚ðÝ’è
	ImGui::StyleColorsDark();

	static int const NUM_FRAMES_IN_FLIGHT = 3;

	ImGui_ImplWin32_Init(winapi->hwnd);
	ImGui_ImplDX12_Init(dx12->device.Get(),
		NUM_FRAMES_IN_FLIGHT,
		//SRGB‚ª‚Â‚¢‚Ä‚È‚©‚Á‚½‚º™
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		tManager->srvHeap.Get(),
		tManager->srvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
		tManager->srvHeap.Get()->GetGPUDescriptorHandleForHeapStart()
	);
	//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}

void ImguiManager::PreUpdate()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::Draw()
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* tManager = TextureManager::Get();

	dx12->commandList->SetDescriptorHeaps(1, tManager->srvHeap.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dx12->commandList.Get());
}

void ImguiManager::PreDraw()
{
	ImGui::Render();
}

void ImguiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ImguiManager* ImguiManager::GetInstance()
{
	static ImguiManager instance;
	return &instance;
}