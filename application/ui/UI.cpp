#include "UI.h"
#include "ImguiManager.h"
#include "Util.h"
#include <sstream>
#include <fstream>

void UI::StaticLoadResource()
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
		numBack[i].SetTexture(*TextureManager::GetTexture("0"));
		numBack[i].SetColor({0.0f,0.0f,0.0f,1});
	}
	point.SetTexture(*TextureManager::GetTexture("twopoint"));

	mainTex.SetTexture(*TextureManager::GetTexture(texturehandle));
	mainTexBack.SetTexture(*TextureManager::GetTexture(texturehandle));
	mainTexBack.SetColor({ 0.5f,0.5f,0.5f,1 });

	alphaTimer.mElapsedTime = alphaTimer.mMaxTime;
	mAlpha = alphaTimer.GetTimeRate();

	initpos = mPos;
}

void UI::Update()
{
	alphaTimer.Update();
	sideMoveTimer.Update();
	if (mAppMode)
	{
		mAlpha = alphaTimer.GetTimeRate();
	}
	else
	{
		mAlpha = 1 - alphaTimer.GetTimeRate();
	}

	
	//諸々を共通変数からそれぞれへ移す
	for (int i = 0; i < 2; i++)
	{
		num[i].mPosition = { mPos.x + (150 - i * 150) * mScale.x,mPos.y,0};
		num[i].SetSize(num[i].GetInitSize() * mScale);
		num[i].mColor.f4.w = mAlpha;
		
		numBack[i].mPosition = { mPos.x + (150 - i * 150) * mScale.x + 3.f,mPos.y + 3.f,0};
		numBack[i].SetSize(numBack[i].GetInitSize() * mScale);
		numBack[i].mColor.f4.w = mAlpha;
	}
	point.mPosition = { mPos.x - 100 * mScale.x,mPos.y,0};
	mainTex.mPosition = { mPos.x - 200 * mScale.x,mPos.y,0};
	mainTexBack.mPosition = { mPos.x - 200 * mScale.x,mPos.y,0};

	point.SetSize(point.GetInitSize() * mScale);
	mainTex.SetSize(mainTex.GetInitSize() * mScale);
	mainTexBack.SetSize(mainTexBack.GetInitSize() * mScale * 1.3f);
	
	point.mColor.f4.w = mAlpha;
	mainTex.mColor.f4.w = mAlpha;
	mainTexBack.mColor.f4.w = mAlpha;

	for (int i = 0; i < 2; i++)
	{
		numBack[i].Update();
		num[i].Update();
	}
	point.Update();
	mainTex.Update();
	mainTexBack.Update();
}

void UI::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		numBack[i].Draw();
		num[i].Draw();
	}
	//point.Draw();

	mainTexBack.Draw();
	mainTex.Draw();
}

void UI::ValueSliders()
{
	ImGui::SliderFloat("ui posX", &mPos.x, 0, Util::WIN_WIDTH);
	ImGui::SliderFloat("ui posY", &mPos.y, 0, Util::WIN_HEIGHT);
	
	ImGui::SliderFloat("ui scale", &guiScale, 0.1f, 5.f);
	if (ImGui::Button("App"))
	{
		Appearance(0.5f);
	}
	if (ImGui::Button("DisApp"))
	{
		DisAppearance(0.2f);
	}
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
		numBack[i].SetTexture(*TextureManager::GetTexture(oss.str()));
	}
}

void UI::AppLock()
{
	mAppMode = true;
	alphaTimer.mElapsedTime = alphaTimer.mMaxTime;
}

void UI::Appearance(float time)
{
	if (!mAppMode)
	{
		alphaTimer.Reset();
	}
	mAppMode = true;
	alphaTimer.mMaxTime = time;
	alphaTimer.NoInitStart();
}

void UI::DisAppearance(float time)
{
	if (mAppMode)
	{
		alphaTimer.Reset();
	}
	mAppMode = false;
	alphaTimer.mMaxTime = time;
	alphaTimer.NoInitStart();
}