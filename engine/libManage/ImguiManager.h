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
	/// <summary>
	/// 初期設定
	/// </summary>
	/// <param name="name">GUIの名前</param>
	GUI(const std::string& name);
	
	/// <summary>
	/// namespace::ImGuiが適用される位置の始めの位置
	/// </summary>
	/// <param name="pos">画面上に配置される初期位置</param>
	/// <param name="size">画面上に配置される時の初期サイズ</param>
	void Begin(const Vector2& pos,const Vector2 &size);
	
	/// <summary>
	/// namespace::ImGuiが適用される位置の終わりの位置
	/// </summary>
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