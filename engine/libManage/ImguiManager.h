#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Vector2.h"
#include <string>

/*! GUI
	ImGuiのGUIを生成するクラス
*/
class GUI
{
public:
	GUI(const std::string& name);
	
	void Begin(const Vector2& pos,const Vector2 &size);
	void End();
private:
	std::string mName;
	bool mSetPosFlag = false;
};

/*! ImguiManager
	ImGuiの初期化などをまとめたクラス
*/
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
	static ImguiManager* Get();

private:
	ImguiManager(){};
	~ImguiManager() {};
	ImguiManager(const ImguiManager& a) = delete;
	ImguiManager& operator=(const ImguiManager&) = delete;
};