#include "TestScene.h"
#include "PlayerCamera.h"
#include "ClearDrawScreen.h"
#include "Input.h"
#include "InstantDrawer.h"
#include "MathF.h"

void TestScene::LoadResource()
{
	player->LoadResource();
}

void TestScene::Initialize()
{
	tri3d.SetModel(ModelManager::GetModel("mountain2"));
	tri3d.SetTexture(TextureManager::GetTexture("white"));

	tri3d.position = {0,-10,0 };
	tri3d.scale = { 5,5,5 };
	tri3d.rotation = { 0,0,0 };

	PlayerCamera::Get()->Initialize();

	player->Initialize();
	//player->SetModel(ModelManager::GetModel("Sphere"));
	player->position = { 0,2,0 };
	player->scale = { 1.5f,1.5f,1.5f };
	//player->ChangeState(Player::PlayerState::Debug);
	//player->SetNoGravity(true);
}

void TestScene::Update()
{
	debugCubes.clear();
	PlayerCamera::Get()->Update();
	player->Update();

	tri3d.Update(*Camera::sCamera);

	//gui.Begin({100,100},{500,300});
	//ImGui::Text("playerpos x:%fy:%fz:%f", player->position.x, player->position.y, player->position.z);
	//ImGui::Text("tri3d.position x:%fy:%fz:%f", tri3d.position.x, tri3d.position.y, tri3d.position.z);
	bool hit = false;
	Vector3 inter;
	Triangle tri;
	
	Ray ray;
	ray.start = player->position;
	//レイの視点を判定球の上の頂点に
	ray.start.y += player->scale.y / 2;
	ray.direction = { 0,-1,0 };

	if (Collsions::CheckRayToPoligon(ray, tri3d,&player->distance, &inter))
	{
		Cube temp;
		temp.position = inter;
		temp.scale = { 1,1,1 };
		debugCubes.push_back(temp);
		temp.position = (tri.pos0 * tri3d.matScale * tri3d.matRot) + tri3d.position;
		debugCubes.push_back(temp);
		temp.position = (tri.pos1 * tri3d.matScale * tri3d.matRot) + tri3d.position;
		debugCubes.push_back(temp);
		temp.position = (tri.pos2 * tri3d.matScale * tri3d.matRot) + tri3d.position;
		debugCubes.push_back(temp);
		hit = true;
	}


	//レイとポリゴンで判定
	if (Collsions::CheckRayToPoligon(ray, tri3d, &player->distance, &inter)) {
		//足元に判定があるなら球で判定
		if (Collsions::SpherePoligonCollsion(player->sphereCol, tri3d))
		{
			//通るなら当たってるので
			//プレイヤーを設置状態に
			if (player->jumpState == Mob::JumpState::Down)
			{
				player->jumpState = Mob::JumpState::None;
			}
			player->position.y = inter.y + player->sphereCol.radius / 2;
		}
		//判定に通らないなら落下中なので
	}

	//ここの処理はMobのUpdate内でやんないといけない？
	if (inter.y < player->position.y - player->sphereCol.radius / 2)
	{
		if (player->jumpState == Mob::JumpState::None)
		{
			player->jumpState = Mob::JumpState::Down;
		}
	}

	/*if (player->distance) {
		ImGui::Text("distance %f", player->distance);
	}

	ImGui::Text("jumpState %d", player->jumpState);

	ImGui::Text("inter x:%fy:%fz:%f",
		inter.x, inter.y, inter.z);
	Vector3 pFeet = player->position;
	pFeet.y -= player->sphereCol.radius / 2;
	ImGui::Text("pFeet x:%fy:%fz:%f",
		pFeet.x, pFeet.y, pFeet.z);

	gui.End();*/
}

void TestScene::Draw()
{
	player->Draw();
	

	BasicObjectPreDraw("WireFrame");
	tri3d.DrawMaterial();

	InstantDrawer::DrawGraph(200, 200, 1.0f, 1.0f, "fovieta");

	for (auto& cube : debugCubes)
	{
		InstantDrawer::DrawBox3D(cube);
	}
}

void TestScene::End()
{
}
