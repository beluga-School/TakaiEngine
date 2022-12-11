#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Vector2.h"

class GUI
{
public:
	GUI(const char* name);
	
	void Begin(Vector2 pos, Vector2 size);
	void End();
private:
	const char* name;
	bool setPosFlag = false;
};

class ImguiManager
{
public:

	/// <summary>
	/// imguiの初期化
	/// </summary>
	void Initialize();

	void PreUpdate();

	/// <summary>
	/// imguiの描画
	/// </summary>
	void Draw();

	void PreDraw();

	/// <summary>
	/// imguiの終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns></returns>
	static ImguiManager* GetInstance();

private:
	ImguiManager(){};
	~ImguiManager() {};
	ImguiManager(const ImguiManager& a) = delete;
	ImguiManager& operator=(const ImguiManager&) = delete;
};