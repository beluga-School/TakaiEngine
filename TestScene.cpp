#include "TestScene.h"
#include "PlayerCamera.h"
#include "ClearDrawScreen.h"
#include "Input.h"

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
	tri3d.rotation = { 1,0,1 };

	PlayerCamera::Get()->Initialize();

	player->Initialize();
	player->SetModel(ModelManager::GetModel("Sphere"));
	player->position = { 0,2,0 };
	player->ChangeState(Player::PlayerState::Debug);
	player->SetNoGravity(true);
}

void TestScene::Update()
{
	for (int32_t i = 0; i < NUM; i++)
	{
		isUse[i] = false;
	}
	PlayerCamera::Get()->Update();
	player->Update();

	tri3d.position.x += Input::Keyboard::PushKey(DIK_RIGHT) - Input::Keyboard::PushKey(DIK_LEFT);
	tri3d.position.z += Input::Keyboard::PushKey(DIK_UP) - Input::Keyboard::PushKey(DIK_DOWN);
	tri3d.Update(*Camera::sCamera);

	gui.Begin({100,100},{500,300});
	ImGui::Text("playerpos x:%fy:%fz:%f", player->position.x, player->position.y, player->position.z);
	ImGui::Text("tri3d.position x:%fy:%fz:%f", tri3d.position.x, tri3d.position.y, tri3d.position.z);
	bool hit = false;
	std::vector<Vector3> hitPoses;
	std::vector<Triangle> triangles;
	//こっちはローカルの状態の三角形(こっちを使うなら、ほかのすべてのやつをローカルに合わせる)
	//for (auto& mesh : tri3d.MODEL->mTriangles)
	//こっちはワールドの状態の三角形
	for (auto& mesh : tri3d.mWorldTriangle)
	{
		if (Collsions::CheckSphere2Triangle(player->mEncountCol, mesh))
		//if (Collsions::SpherePoligonCollsion(player->mEncountCol, tri3d.position, mesh))
		{
			hit = true;
			Collsions::SpherePoligonCollsion(player->mEncountCol, tri3d.position, mesh);
			Vector3 cal = Collsions::ClosestPtPoint2Triangle(player->position, mesh);
			hitPoses.push_back(cal);
			triangles.push_back(mesh);
		}
		//先に四角で囲った範囲だけ判定するように変えたかったが、のちのち困りそうなので撤退
		/*Cube preCheck;
		preCheck.position = tri3d.position;
		Vector3 size = tri3d.scale * 1.05f;
		preCheck.scale = size;
	
		for (int32_t j = 0; j < NUM; j++)
		{
			if (isUse[j] == false)
			{
				debugObjects[j].SetModel(ModelManager::GetModel("Cube"));
				debugObjects[j].position = preCheck.position;
				debugObjects[j].scale = preCheck.scale;
				isUse[j] = true;
				break;
			}
		}
		if (Collsions::CubeCollision(player->box.cubecol, preCheck))
		{
			
		}*/
	
	}
	if (hit)
	{
		player->position -= player->moveValue;
		ImGui::Text("hit");
		for (int32_t i = 0; i < hitPoses.size();i++)
		{
			ImGui::Text("hitpos x:%fy:%fz:%f", hitPoses[i].x, hitPoses[i].y, hitPoses[i].z);
			ImGui::Text("trianglesPos0 x:%fy:%fz:%f", triangles[i].pos0.x, triangles[i].pos0.y, triangles[i].pos0.z);
			ImGui::Text("trianglesPos1 x:%fy:%fz:%f", triangles[i].pos1.x, triangles[i].pos1.y, triangles[i].pos1.z);
			ImGui::Text("trianglesPos2 x:%fy:%fz:%f", triangles[i].pos2.x, triangles[i].pos2.y, triangles[i].pos2.z);
		}
	}
	gui.End();
}

void TestScene::Draw()
{
	player->Draw();
	
	BasicObjectPreDraw("WireFrame");

	tri3d.DrawMaterial();
}

void TestScene::End()
{
}
