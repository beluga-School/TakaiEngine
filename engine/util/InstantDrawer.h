#pragma once
#include "Sprite.h"
#include "Color.h"
#include <list>
#include <Billboard.h>

/*! InstantDrawer
	DxLibライクな描画関数を利用できるクラス
*/
class InstantDrawer
{
public:
	enum class Anchor
	{
		LEFT,
		CENTER,
		RIGHT,
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
	/// 3Dで画像を描画する機能(ビルボード)
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="handle"></param>
	static void DrawGraph3D(const Vector3& pos,float width,float height,const std::string& handle);
	
	/// <summary>
	/// 全更新
	/// </summary>
	static void AllUpdate();

	/// <summary>
	/// 全描画
	/// </summary>
	static void AllDraw2D();
	static void AllDraw3D();

private:
	static std::list<Sprite> sSprites;

	static std::list<Billboard> sBillboards;
};