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
	player->SetModel(ModelManager::GetModel("Sphere"));
	player->position = { 0,2,0 };
	player->ChangeState(Player::PlayerState::Debug);
	//player->SetNoGravity(true);
}

void TestScene::Update()
{
	debugCubes.clear();
	PlayerCamera::Get()->Update();
	player->Update();

	tri3d.Update(*Camera::sCamera);

	gui.Begin({100,100},{500,300});
	ImGui::Text("playerpos x:%fy:%fz:%f", player->position.x, player->position.y, player->position.z);
	ImGui::Text("tri3d.position x:%fy:%fz:%f", tri3d.position.x, tri3d.position.y, tri3d.position.z);
	bool hit = false;
	Vector3 inter;
	Triangle tri;
	if (Collsions::SpherePoligonCollsion(player->mEncountCol, tri3d, &inter, &tri))
	{
		Cube temp;
		temp.position = inter;
		temp.scale = {1,1,1};
		debugCubes.push_back(temp);
		temp.position = (tri.pos0 * tri3d.matScale * tri3d.matRot) + tri3d.position;
		debugCubes.push_back(temp);
		temp.position = (tri.pos1 * tri3d.matScale * tri3d.matRot) + tri3d.position;
		debugCubes.push_back(temp);
		temp.position = (tri.pos2 * tri3d.matScale * tri3d.matRot) + tri3d.position;
		debugCubes.push_back(temp);
		hit = true;
	}

	float distance = 0;
	Ray ray;
	ray.start = player->position;

	ray.direction = {0,-1,0};
	inter = { 0,0,0 };

	if (distance) {
		ImGui::Text("distance %f",distance);
	}

	player->SetNoGravity(false);

	if (hit)
	{
		if (Collsions::CheckRayToPoligon(ray, tri3d, &distance, &inter)) {
			ImGui::Text("rayhit");
		}

		if (MathF::Avarage(inter)) {
			ImGui::Text("inter x:%f y:%f z:%f", inter.x, inter.y, inter.z);
			Cube temp;
			temp.position = inter;
			temp.scale = { 1,1,1 };
			debugCubes.push_back(temp);

			//雑押し戻し
			player->position.y = inter.y;
			player->SetNoGravity(true);
		}
	}
	gui.End();
}

void TestScene::Draw()
{
	player->Draw();
	
	BasicObjectPreDraw("WireFrame");

	tri3d.DrawMaterial();

	for (auto& cube : debugCubes)
	{
		InstantDrawer::DrawBox3D(cube);
	}
}

void TestScene::End()
{
}
