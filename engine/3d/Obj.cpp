#include "Obj.h"
#include "Result.h"

void Obj3d::SetModel(const Model* model)
{
	MODEL = model;
}

void Obj3d::SetTexture(const Texture* texture)
{
	this->TEXTURE = texture;
}

void Obj3d::Initialize()
{
	MODEL = ModelManager::GetModel("Cube");
	TEXTURE = TextureManager::GetTexture("default");
}

void Obj3d::Update(const Camera& camera)
{
	Matrix4 matScale;	//スケーリング行列
	Matrix4 matRot;	//回転行列
	Matrix4 matTrans;	//平行移動行列

	//スケールを設定
	matScale = Matrix4::Identity();
	matScale = Matrix4::scale(scale);

	//回転を設定
	matRot = Matrix4::Identity();
	matRot = Matrix4::rotateZ(rotation.z) * Matrix4::rotateX(rotation.x) * Matrix4::rotateY(rotation.y);

	matTrans = Matrix4::Identity();
	matTrans = Matrix4::translate(position);

	matWorld = Matrix4::Identity();
	matWorld = matScale * matRot * matTrans;

	if (parent != nullptr)
	{	
		if (notScaleFlag)
		{
			/*Matrix4 matRotPar;
			Matrix4 matTransPar;

			matRotPar *= XMMatrixRotationZ(parent->rotation.z);
			matRotPar *= XMMatrixRotationX(parent->rotation.x);
			matRotPar *= XMMatrixRotationY(parent->rotation.y);

			matTransPar = XMMatrixTranslation(
				parent->position.x, parent->position.y, parent->position.z);

			matWorld *= matRotPar;
			matWorld *= matTransPar;*/
		}
		else
		{
			matWorld = matWorld * parent->matWorld;
		}

	}

	constBufferT.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferT.mConstBufferData->world = matWorld;
	constBufferT.mConstBufferData->cameraPos = camera.mEye;
	constBufferT.mConstBufferData->cameraDir = camera.mMatView.ExtractAxisZ();

	if (MODEL != nullptr)
	{
		constBufferB1.mConstBufferData->ambient = MODEL->mMaterial.mAmbient;
		constBufferB1.mConstBufferData->diffuse = MODEL->mMaterial.mDiffuse;
		constBufferB1.mConstBufferData->specular = MODEL->mMaterial.mSpecular;
		constBufferB1.mConstBufferData->alpha = MODEL->mMaterial.mAlpha;
		constBufferB.mConstBufferData->brightness = color_;
	}

	constBufferOutLine.mConstBufferData->color = mOutLineColor;
	constBufferOutLine.mConstBufferData->thickness = mOutLineThickness;

	transparentTimer.Update();
}

Vector3 Obj3d::GetWorldTrans()
{
	Vector3 worldTrans={
		matWorld.m[3][0],
		matWorld.m[3][1],
		matWorld.m[3][2]
	};
	return worldTrans;
}

void Obj3d::Draw() {

	//見えないフラグが立ってるなら描画を行わない
	if (mIsVisiable == false)
	{
		return;
	}

	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();
	
	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, TEXTURE->mGpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//頂点バッファの設定
	dx12->mCmdList->IASetVertexBuffers(0, 1, &MODEL->mVbView);

	//インデックスバッファの設定
	dx12->mCmdList->IASetIndexBuffer(&MODEL->mIbView);
	
	//定数バッファビュー(CBV)の設定コマンド
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferB1.mBuffer->GetGPUVirtualAddress());
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferM.buffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(2, constBufferT.mBuffer->GetGPUVirtualAddress());
	
	dx12->mCmdList->SetGraphicsRootConstantBufferView(3, constBufferB.mBuffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->mCmdList->DrawIndexedInstanced((UINT)MODEL->mMesh.indices.size() , 1, 0, 0, 0);
}

void Obj3d::DrawMaterial() {
	//見えないフラグが立ってるなら描画を行わない
	if (mIsVisiable == false)
	{
		return;
	}

	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, MODEL->mMaterial.mTextire->mGpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//頂点バッファの設定
	dx12->mCmdList->IASetVertexBuffers(0, 1, &MODEL->mVbView);

	//インデックスバッファの設定
	dx12->mCmdList->IASetIndexBuffer(&MODEL->mIbView);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferB1.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(2, constBufferT.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(3, constBufferB.mBuffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->mCmdList->DrawIndexedInstanced((UINT)MODEL->mMesh.indices.size(), 1, 0, 0, 0);
}

void Obj3d::DrawOutLine()
{
	//見えないフラグが立ってるなら描画を行わない
	if (mIsVisiable == false)
	{
		return;
	}

	DirectX12* dx12 = DirectX12::Get();
	
	//頂点バッファの設定
	dx12->mCmdList->IASetVertexBuffers(0, 1, &MODEL->mVbView);

	//インデックスバッファの設定
	dx12->mCmdList->IASetIndexBuffer(&MODEL->mIbView);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferOutLine.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->SetGraphicsRootConstantBufferView(1, constBufferT.mBuffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->mCmdList->DrawIndexedInstanced((UINT)MODEL->mMesh.indices.size(), 1, 0, 0, 0);

}

void Obj3d::SetOutLineState(const Float4& color, float thickness)
{
	mOutLineColor = color;
	mOutLineThickness = thickness;
}
