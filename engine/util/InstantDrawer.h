#pragma once
#include "Sprite.h"
#include "Color.h"
#include <list>
#include <Billboard.h>
#include <Collision.h>

/*! InstantDrawer
	DxLibライクな描画関数を利用できるクラス
*/

class InstantSprite : public Sprite
{
public:
	bool isUse = false;
};

class InstantBillboard : public Billboard
{
public:
	bool isUse = true;
};

class InstantObj : public Obj3d
{
public:
	bool isUse = true;
};

class InstantDrawer
{
public:
	enum class Anchor
	{
		LEFT,
		CENTER,
		RIGHT,
		UP,
		DOWN,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	static void DrawInit();

	/// <summary>
	/// 2Dの箱を描画する機能
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="color"></param>
	/// <param name="anchor"></param>
	static void DrawBox(
		const float& x, const float& y,
		const float& width, const float& height,
		const Color& color, const Anchor& anchor = Anchor::CENTER);

	/// <summary>
	/// 2Dで画像を描画する機能
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="sizerateX"></param>
	/// <param name="sizerateY"></param>
	/// <param name="handle"></param>
	/// <param name="anchor"></param>
	static void DrawGraph(
		const float& x, const float& y,
		float sizerateX,float sizerateY,
		const std::string& handle, const Anchor& anchor = Anchor::CENTER);

	/// <summary>
	/// 2Dで画像を描画する機能
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="sizerateX"></param>
	/// <param name="sizerateY"></param>
	/// <param name="angle">弧度法で角度</param>
	/// <param name="handle"></param>
	/// <param name="anchor"></param>
	static void DrawRotaGraph(
		const float& x, const float& y,
		float sizerateX, float sizerateY,float angle,
		const std::string& handle,Color color = {1,1,1,1}, const Anchor& anchor = Anchor::CENTER);
	
	/// <summary>
	/// 3Dで画像を描画する機能(ビルボード)
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="handle"></param>
	static void DrawGraph3D(const Vector3& pos,float width,float height,const std::string& handle);
	
	/// <summary>
	/// コリジョンのcubeを3Dオブジェクトとして描画する関数
	/// </summary>
	static void DrawBox3D(const Cube& cube);

	/// <summary>
	/// 全更新
	/// </summary>
	static void AllUpdate();

	/// <summary>
	/// 全描画
	/// </summary>
	static void AllDraw2D();
	static void AllDraw3D();

	//事前にメモリを確保
	static void PreCreate();

private:
	static InstantDrawer* Get() {
		static InstantDrawer instance;
		return &instance;
	}

	static const int SP_MAX = 500;
	std::list<InstantSprite> sSprites;

	static const int BILL_MAX = 100;
	std::list<InstantBillboard> sBillboards;

	static const int OBJ_MAX = 100;
	std::list<InstantObj> sObjs;
};