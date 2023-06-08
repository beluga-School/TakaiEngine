#include "FBXLoadDemoScene.h"
#include <Sprite.h>
#include <ClearDrawScreen.h>
#include <AssimpLoader.h>
#include "DirectXInit.h"

void FBXLoadDemoScene::Initialize()
{
	//fbx�ǂݍ��݂̌`��
	ImportSettings importSetting =
	{
		modelFile,
		meshes,
		false,
		false,
	};
	AssimpLoader loader;

	if (!loader.Load(importSetting))
	{
		assert(0);
	}

	//spherefbx.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		auto vertices = meshes[i].vertices;
		auto indices = meshes[i].indices;

		/*vertexDatas.emplace_back();
		vertexDatas.back().CreateVertex(vertices, indices);*/

		spherefbx.emplace_back();
		spherefbx.back().Initialize();

		spherefbx.back().model->CreateVertex(vertices, indices);
		spherefbx.back().model->mesh.vertices = vertices;
		spherefbx.back().model->mesh.indices = indices;
	}

	//3d�I�u�W�F�N�g�p�̃p�C�v���C������
	object3dPipelineSet = CreateObject3DPipeline();

	camera->Initialize();

	skydome.Initialize();
	skydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	skydome.SetTexture(&TextureManager::Get()->white);
	skydome.scale = { 10,10,10 };
	skydome.color = { 1.f,1.f,1.f,1.0f };

}

void FBXLoadDemoScene::Update()
{
	camera->DebugMove();
	camera->UpdatematView();

	for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Update(*camera);
	}

	skydome.Update(*camera);
}

void FBXLoadDemoScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	skydome.DrawMaterial();

	for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Draw();
		//auto vbView = vertexDatas[i].vbView; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@
		//auto ibView = vertexDatas[i].ibView; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@

		//DirectX12::Get()->commandList->SetGraphicsRootConstantBufferView(0, constantBuffer[currentIndex]->GetAddress());

		//DirectX12::Get()->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//DirectX12::Get()->commandList->IASetVertexBuffers(0, 1, &vbView);
		//DirectX12::Get()->commandList->IASetIndexBuffer(&ibView);

		//DirectX12::Get()->commandList->DrawIndexedInstanced(meshes[i].indices.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��

	}


	//�X�v���C�g�̑O�`��(���ʃR�}���h)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

}

void FBXLoadDemoScene::End()
{
}
