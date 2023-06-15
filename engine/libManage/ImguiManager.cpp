#include "ImguiManager.h"
#include "WinAPI.h"
#include "DirectXInit.h"
#include "Texture.h"

GUI::GUI(const std::string& name_)
{
	mName = name_;
}

void GUI::Begin(const Vector2& pos,const Vector2 &size)
{
	if (mSetPosFlag == false)
	{
		mSetPosFlag = true;
		ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
		ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	}
	ImGui::Begin(mName.c_str());
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

	static int32_t const sNUM_FRAMES_IN_FLIGHT = 3;

	ImGui_ImplWin32_Init(winapi->mHwnd);
	ImGui_ImplDX12_Init(dx12->mDevice.Get(),
		sNUM_FRAMES_IN_FLIGHT,
		//SRGB‚ª‚Â‚¢‚Ä‚È‚©‚Á‚½‚º™
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		tManager->mSrvHeap.Get(),
		tManager->mSrvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
		tManager->mSrvHeap.Get()->GetGPUDescriptorHandleForHeapStart()
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

	dx12->mCmdList->SetDescriptorHeaps(1, tManager->mSrvHeap.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dx12->mCmdList.Get());
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