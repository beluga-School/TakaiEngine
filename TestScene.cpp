#include "TestScene.h"
#include "PlayerCamera.h"
#include "ClearDrawScreen.h"
#include "Input.h"
#include "InstantDrawer.h"

void TestScene::LoadResource()
{
	player->LoadResource();
}

void TestScene::Initialize()
{
	tri3d.SetModel(ModelManager::GetModel("Cube"));
	tri3d.SetTexture(TextureManager::GetTexture("white"));

	tri3d.position = {0,-10,0 };
	tri3d.scale = { 5,5,5 };
	tri3d.rotation = { 0,0,0 };

	PlayerCamera::Get()->Initialize();

	player->Initialize();
	player->SetModel(ModelManager::GetModel("Sphere"));
	player->position = { 0,2,0 };
	player->ChangeState(Player::PlayerState::Debug);
	player->SetNoGravity(true);
}

void TestScene::Update()
{
	debugCubes.clear();
	PlayerCamera::Get()->Update();
	player->Update();

	static bool trigger = false;

	if (trigger)
	{
		tri3d.position.x += Input::Keyboard::TriggerKey(DIK_RIGHT) - Input::Keyboard::TriggerKey(DIK_LEFT);
		tri3d.position.z += Input::Keyboard::TriggerKey(DIK_UP) - Input::Keyboard::TriggerKey(DIK_DOWN);
	}
	else
	{
		tri3d.position.x += Input::Keyboard::PushKey(DIK_RIGHT) - Input::Keyboard::PushKey(DIK_LEFT);
		tri3d.position.z += Input::Keyboard::PushKey(DIK_UP) - Input::Keyboard::PushKey(DIK_DOWN);
	}
	tri3d.Update(*Camera::sCamera);

	gui.Begin({100,100},{500,300});
	ImGui::Text("trigger %d", trigger);
	if(ImGui::Button("triggerChange"))
	{
		trigger = !trigger;
	}
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

	if (hit)
	{
		//雑押し戻し
		player->position -= player->moveValue;
		ImGui::Text("hit");
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
