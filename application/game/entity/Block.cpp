#include "Block.h"
#include "Player.h"
#include "DirectXInit.h"

void Block::Update()
{
	Obj3d::Update(*Camera::sCamera);
	constBufferPlayerPos.mConstBufferData->value = Player::Get()->position;
}

void Block::Draw()
{
	//b4にプレイヤー位置を送る
	DirectX12::Get()->mCmdList->SetGraphicsRootConstantBufferView(5, constBufferPlayerPos.mBuffer->GetGPUVirtualAddress());
	Obj3d::Draw();
}

void Block::DrawMaterial()
{	
	//b4にプレイヤー位置を送る
	//この書き方だと送れない　なぜ？
	DirectX12::Get()->mCmdList->SetGraphicsRootConstantBufferView(5, constBufferPlayerPos.mBuffer->GetGPUVirtualAddress());
	Obj3d::DrawMaterial();
}
