#include "UI.h"
#include "ImguiManager.h"
#include "Util.h"
#include <sstream>
#include <fstream>

void UI::LoadResource()
{
	TextureManager::Load("Resources\\ui\\number0.png", "0");
	TextureManager::Load("Resources\\ui\\number1.png", "1");
	TextureManager::Load("Resources\\ui\\number2.png", "2");
	TextureManager::Load("Resources\\ui\\number3.png", "3");
	TextureManager::Load("Resources\\ui\\number4.png", "4");
	TextureManager::Load("Resources\\ui\\number5.png", "5");
	TextureManager::Load("Resources\\ui\\number6.png", "6");
	TextureManager::Load("Resources\\ui\\number7.png", "7");
	TextureManager::Load("Resources\\ui\\number8.png", "8");
	TextureManager::Load("Resources\\ui\\number9.png", "9");

	TextureManager::Load("Resources\\ui\\twopoint.png", "twopoint");
}

void UI::Initialize(const std::string& texturehandle)
{
	for (int i = 0; i < 2; i++)
	{
		num[i].SetTexture(*TextureManager::GetTexture("0"));
	}
	point.SetTexture(*TextureManager::GetTexture("twopoint"));

	mainTex.SetTexture(*TextureManager::GetTexture(texturehandle));
}

void UI::Update()
{
	for (int i = 0; i < 2; i++)
	{
		num[i].mPosition = { mPos.x + (150 - i * 150) * mScale.x,mPos.y,0};
	}
	point.mPosition = { mPos.x - 100 * mScale.x,mPos.y,0};
	mainTex.mPosition = { mPos.x - 200 * mScale.x,mPos.y,0};

	for (int i = 0; i < 2; i++)
	{
		num[i].SetSize(num[i].GetInitSize() * mScale);
	}
	point.SetSize(point.GetInitSize() * mScale);
	mainTex.SetSize(mainTex.GetInitSize() * mScale);

	for (int i = 0; i < 2; i++)
	{
		num[i].Update();
	}
	point.Update();
	mainTex.Update();
}

void UI::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		num[i].Draw();
	}
	point.Draw();
	mainTex.Draw();
}

void UI::ValueSliders()
{
	ImGui::SliderFloat("ui posX", &mPos.x, 0, Util::WIN_WIDTH);
	ImGui::SliderFloat("ui posY", &mPos.y, 0, Util::WIN_HEIGHT);
	
	ImGui::SliderFloat("ui scale", &guiScale, 0.1f, 5.f);
	mScale = { guiScale,guiScale };
	
	ImGui::InputText("filename", output, sizeof(output));

	if (ImGui::Button("SaveValue"))
	{
		//表示用の名前を保存
		savename = output;
		savename += ".txt";

		//ファイル出力処理
		std::ofstream writing_file;

		std::string filename = "";
		filename = output;
		filename = "./Data/" + filename + ".txt";

		writing_file.open(filename, std::ios::out);

		writing_file << "mPos.x:" << mPos.x << ",\n";
		writing_file << "mPos.y:" << mPos.y << ",\n";
		writing_file << "mScale.x:" << mScale.x << ",\n";
		writing_file << "mScale.y:" << mScale.y << ",\n";

		writing_file.close();
	}
}

void UI::UpdateNumber(int32_t status)
{
	//整数2桁にしか対応してないので、それ以外は入れない
	if (status > 99 || status < 0)
	{
		return;
	}
	//[0]が1の位,[1]が10の位
	int32_t digit[2];
	digit[0] = status % 10;
	digit[1] = (status - digit[0]) / 10;
	

	for (int i = 0; i < 2; i++)
	{
		std::ostringstream oss; 
		oss << digit[i];
		num[i].SetTexture(*TextureManager::GetTexture(oss.str()));
	}
}