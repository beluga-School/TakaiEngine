#include "GameScene.h"
#include "TimeManager.h"
#include "Collision.h"

void GameScene::Initialize()
{
	//3d�I�u�W�F�N�g�p�̃p�C�v���C������
	object3dPipelineSet = CreateObject3DPipeline();

	//�W�I���g���I�u�W�F�N�g�p�p�C�v���C������
	geometryObjectPipelineSet = CreateGeometryPipeline();

	//�X�v���C�g
	SpriteCreate(&pizzaSprite, &TextureManager::GetInstance()->pizza, { 0.5f,0.5f });
	SpriteInit(pizzaSprite, SpriteCommon::spriteCommon, { 100,100 }, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });
	
	SpriteCreate(&slimeSprite, &TextureManager::GetInstance()->slime, { 0.5f,0.5f });
	SpriteInit(slimeSprite, SpriteCommon::spriteCommon, { 100,100 }, 0, { 1,1,1,1 });

	//�W�I���g���I�u�W�F�N�g����
	//particleManager->Initialize();

	skydome.Initialize();

	//�ǂ̃��f���̌`���g������ݒ�
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);

	skydome.SetTexture(&TextureManager::GetInstance()->slime);

	//�r���[�ϊ��s��(�������e)���v�Z
	camera->Initialize();

	daruma.Initialize();
	daruma.SetModel(&ModelManager::GetInstance()->darumaM);
	daruma.SetTexture(&TextureManager::GetInstance()->white);
	daruma.scale = { 5,5,5 };

	/*billboard.Initialize();
	billboard.SetModel(&ModelManager::GetInstance()->boardM);
	billboard.SetTexture(&TextureManager::GetInstance()->pizza);

	billboard.position = { 10,10,10 };
	billboard.scale = { 3,3,3 };*/

	camera->target = {
		0,0,0
	};

	camera->SetEye({ 0,10,-30 });

	stage.Initialize();
	stage.position = { 0,-10,0 };
	stage.scale = { 10,10,10 };
}

void GameScene::Update()
{
	//�J�������W�𓮂���
	if (input->PushKey(DIK_RIGHT)) {
		camera->eye.x += 20 * TimeManager::deltaTime;
		camera->target.x += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_LEFT)) {
		camera->eye.x -= 20 * TimeManager::deltaTime;
		camera->target.x -= 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_UP)) {
		camera->eye.z += 20 * TimeManager::deltaTime;
		camera->target.z += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_DOWN)) {
		camera->eye.z -= 20 * TimeManager::deltaTime;
		camera->target.z -= 20 * TimeManager::deltaTime;
	}

	if (input->PushKey(DIK_D)) {
		daruma.position.x += 10.0f * TimeManager::deltaTime;
		//camera->eye.x += 20 * TimeManager::deltaTime;
		//camera->target.x += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_A)) {
		daruma.position.x -= 10.0f * TimeManager::deltaTime;
		//camera->eye.x -= 20 * TimeManager::deltaTime;
		//camera->target.x -= 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_W)) {
		daruma.position.z += 10.0f * TimeManager::deltaTime;
		//camera->eye.z += 20 * TimeManager::deltaTime;
		//camera->target.z += 20 * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_S)) {
		daruma.position.z -= 10.0f * TimeManager::deltaTime;
		//camera->eye.z -= 20 * TimeManager::deltaTime;
		//camera->target.z -= 20 * TimeManager::deltaTime;
	}

	camera->UpdatematView();

	if (input->TriggerKey(DIK_R))
	{
		daruma.position = { 0,10,0 };
		daruma.jumpPower = 0;
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		daruma.jumpPower = 1;
	}

	camera->UpdatematView();

	//�I�u�W�F�N�g�̍X�V
	//particleManager->Update(camera->matView, camera->matProjection);

	skydome.Update(camera->matView, camera->matProjection);

	stage.Update(camera->matView, camera->matProjection);
	stage.cubeCol.position = stage.position;
	stage.cubeCol.scale = stage.scale;


	if (!CubeCollision(stage.cubeCol, daruma.cubeCol))
	{
		//�d�͂�����
		daruma.jumpPower += daruma.gravity;
		//Y�������ւ̏d�͂�t�^
		daruma.position.y += daruma.jumpPower;
	}

	//�G�����߂��A���ꂾ�Ɣ��ɂ������Ă��܂��̂ł悭�Ȃ�
	if (CubeCollision(stage.cubeCol, daruma.cubeCol))
	{
		daruma.position.y = stage.position.y + stage.scale.y;
		daruma.jumpPower = 0;
	}

	Plane plane;
	plane.normal = { 1,0,0 };

	colflag = false;
	if (RayPlaneCollision(daruma.rayCol, plane))
	{
		colflag = true;
	}

	daruma.Update(camera->matView, camera->matProjection);
	daruma.cubeCol.position = daruma.position;
	daruma.cubeCol.scale = daruma.scale;
	daruma.rayCol.start = daruma.position;
	daruma.rayCol.direction = { -1,0,0 };
}

void GameScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	//�`�揈��

	stage.Draw();

	//billboard.Draw();
	daruma.DrawMaterial();

	skydome.DrawMaterial();

	GeometryObjectPreDraw(geometryObjectPipelineSet);
	//particleManager->Draw(&TextureManager::GetInstance()->particle);


	//�X�v���C�g�̑O�`��(���ʃR�}���h)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

	//�X�v���C�g�P�̕`��
	//SpriteDraw(pizzaSprite);
	//SpriteDraw(slimeSprite);
}

void GameScene::End()
{
}
