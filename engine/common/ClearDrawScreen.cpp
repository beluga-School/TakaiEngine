#include "ClearDrawScreen.h"
#include "DirectXInit.h"
#include "Result.h"
#include "Util.h"
#include "Texture.h"
#include "LightGroup.h"
#include "Obj.h"

ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

ComPtr<ID3D12Resource> depthBuff;

D3D12_RESOURCE_BARRIER barrierDesc{};

void CreateDepthView()
{
	DirectX12* dx12 = DirectX12::Get();

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Util::WIN_WIDTH;	//レンダーターゲットに合わせる
	depthResourceDesc.Height = Util::WIN_HEIGHT;	//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	//リソース生成
	
	result = dx12->mDevice->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	
	result = dx12->mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	assert(SUCCEEDED(result));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx12->mDevice->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void ClearDrawScreen()
{
	DirectX12* dx12 = DirectX12::Get();

	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };

	UINT bbIndex = dx12->mSwapChain->GetCurrentBackBufferIndex();

	//1.リソースバリアで書き込み化に変更
	//D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = dx12->mBackBuffers[bbIndex].Get();	//バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	//表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		//描画状態へ
	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);
	
	//2.描画先の変更
	//レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx12->mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dx12->mDevice->GetDescriptorHandleIncrementSize(dx12->mRtvHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dx12->mCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//3.画面クリア
	dx12->mCmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	dx12->mCmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void BasicObjectPreDraw(const PipelineSet& objectPipelineSet)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::WIN_WIDTH;
	viewport.Height = Util::WIN_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//ビューポート設定コマンドを、コマンドリストに積む
	dx12->mCmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//切り抜き座標左
	scissorRect.right = scissorRect.left + Util::WIN_WIDTH;	//切り抜き座標右
	scissorRect.top = 0;									//切り抜き座標上
	scissorRect.bottom = scissorRect.top + Util::WIN_HEIGHT;	//切り抜き座標下
	//シザー矩形設定コマンドを、コマンドリストに積む
	dx12->mCmdList->RSSetScissorRects(1, &scissorRect);

	//パイプラインステートとルートシグネチャの設定コマンド
	//スプライトじゃない方
	dx12->mCmdList->SetPipelineState(objectPipelineSet.mPipelinestate.Get());
	dx12->mCmdList->SetGraphicsRootSignature(objectPipelineSet.mRootsignature.Get());

	//プリミティブ形状の設定コマンド
	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SRVヒープの設定コマンド
	dx12->mCmdList->SetDescriptorHeaps(1, texM->mSrvHeap.GetAddressOf());

	LightGroup::mLightGroup->Draw(4);
}

void GeometryObjectPreDraw(const PipelineSet& geometryPipelineSet)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::WIN_WIDTH;
	viewport.Height = Util::WIN_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//ビューポート設定コマンドを、コマンドリストに積む
	dx12->mCmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//切り抜き座標左
	scissorRect.right = scissorRect.left + Util::WIN_WIDTH;	//切り抜き座標右
	scissorRect.top = 0;									//切り抜き座標上
	scissorRect.bottom = scissorRect.top + Util::WIN_HEIGHT;	//切り抜き座標下
	//シザー矩形設定コマンドを、コマンドリストに積む
	dx12->mCmdList->RSSetScissorRects(1, &scissorRect);

	//パイプラインステートとルートシグネチャの設定コマンド
	//スプライトじゃない方
	dx12->mCmdList->SetPipelineState(geometryPipelineSet.mPipelinestate.Get());
	dx12->mCmdList->SetGraphicsRootSignature(geometryPipelineSet.mRootsignature.Get());

	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//SRVヒープの設定コマンド
	dx12->mCmdList->SetDescriptorHeaps(1, texM->mSrvHeap.GetAddressOf());
}

void PostDraw()
{
	DirectX12* dx12 = DirectX12::Get();

	//5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;	//表示状態へ
	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);

	//命令のクローズ
	result = dx12->mCmdList->Close();
	assert(SUCCEEDED(result));

	//コマンドリストの実行
	ID3D12CommandList* commandLists[] = { dx12->mCmdList.Get() };
	dx12->mCmdQueue->ExecuteCommandLists(1, commandLists);

	//画面に表示するバッファをフリップ(裏表の入れ替え)
	result = dx12->mSwapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	//コマンドの実行完了を待つ
	dx12->mCmdQueue->Signal(dx12->mFence.Get(), ++dx12->mFenceVal);
	if (dx12->mFence->GetCompletedValue() != dx12->mFenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		dx12->mFence->SetEventOnCompletion(dx12->mFenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//キューをクリア
	result = dx12->mCommandAllocator->Reset();
	assert(SUCCEEDED(result));

	//再びコマンドリストを貯める準備
	result = dx12->mCmdList->Reset(dx12->mCommandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}
