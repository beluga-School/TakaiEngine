#include "ClearDrawScreen.h"
#include "DirectXInit.h"
#include "Result.h"
#include "Util.h"
#include "Texture.h"

ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

ComPtr<ID3D12Resource> depthBuff;

D3D12_RESOURCE_BARRIER barrierDesc{};

void CreateDepthView()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Util::window_width;	//レンダーターゲットに合わせる
	depthResourceDesc.Height = Util::window_height;	//レンダーターゲットに合わせる
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
	
	result = dx12->device->CreateCommittedResource(
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
	
	result = dx12->device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	assert(SUCCEEDED(result));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx12->device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void ClearDrawScreen()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };

	UINT bbIndex = dx12->swapChain->GetCurrentBackBufferIndex();

	//1.リソースバリアで書き込み化に変更
	//D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = dx12->backBuffers[bbIndex].Get();	//バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	//表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		//描画状態へ
	dx12->commandList->ResourceBarrier(1, &barrierDesc);
	
	//2.描画先の変更
	//レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx12->rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dx12->device->GetDescriptorHandleIncrementSize(dx12->rtvHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dx12->commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//3.画面クリア
	dx12->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	dx12->commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void BasicObjectPreDraw(PipelineSet objectPipelineSet)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::window_width;
	viewport.Height = Util::window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//ビューポート設定コマンドを、コマンドリストに積む
	dx12->commandList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//切り抜き座標左
	scissorRect.right = scissorRect.left + Util::window_width;	//切り抜き座標右
	scissorRect.top = 0;									//切り抜き座標上
	scissorRect.bottom = scissorRect.top + Util::window_height;	//切り抜き座標下
	//シザー矩形設定コマンドを、コマンドリストに積む
	dx12->commandList->RSSetScissorRects(1, &scissorRect);

	//パイプラインステートとルートシグネチャの設定コマンド
	//スプライトじゃない方
	dx12->commandList->SetPipelineState(objectPipelineSet.pipelinestate.Get());
	dx12->commandList->SetGraphicsRootSignature(objectPipelineSet.rootsignature.Get());

	//プリミティブ形状の設定コマンド
	dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SRVヒープの設定コマンド
	dx12->commandList->SetDescriptorHeaps(1, texM->srvHeap.GetAddressOf());
}

void GeometryObjectPreDraw(PipelineSet geometryPipelineSet)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::window_width;
	viewport.Height = Util::window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//ビューポート設定コマンドを、コマンドリストに積む
	dx12->commandList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//切り抜き座標左
	scissorRect.right = scissorRect.left + Util::window_width;	//切り抜き座標右
	scissorRect.top = 0;									//切り抜き座標上
	scissorRect.bottom = scissorRect.top + Util::window_height;	//切り抜き座標下
	//シザー矩形設定コマンドを、コマンドリストに積む
	dx12->commandList->RSSetScissorRects(1, &scissorRect);

	//パイプラインステートとルートシグネチャの設定コマンド
	//スプライトじゃない方
	dx12->commandList->SetPipelineState(geometryPipelineSet.pipelinestate.Get());
	dx12->commandList->SetGraphicsRootSignature(geometryPipelineSet.rootsignature.Get());

	dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//SRVヒープの設定コマンド
	dx12->commandList->SetDescriptorHeaps(1, texM->srvHeap.GetAddressOf());
}

void PostDraw()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;	//表示状態へ
	dx12->commandList->ResourceBarrier(1, &barrierDesc);

	//命令のクローズ
	result = dx12->commandList->Close();
	assert(SUCCEEDED(result));

	//コマンドリストの実行
	ID3D12CommandList* commandLists[] = { dx12->commandList.Get() };
	dx12->commandQueue->ExecuteCommandLists(1, commandLists);

	//画面に表示するバッファをフリップ(裏表の入れ替え)
	result = dx12->swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	//コマンドの実行完了を待つ
	dx12->commandQueue->Signal(dx12->fence.Get(), ++dx12->fenceVal);
	if (dx12->fence->GetCompletedValue() != dx12->fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		dx12->fence->SetEventOnCompletion(dx12->fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//キューをクリア
	result = dx12->commandAllocator->Reset();
	assert(SUCCEEDED(result));

	//再びコマンドリストを貯める準備
	result = dx12->commandList->Reset(dx12->commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}
