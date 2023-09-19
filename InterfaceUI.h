#pragma once
#include <TEasing.h>
#include <Vector2.h>

enum class UIMove
{
	START,
	END,
};

class InterfaceUI
{
public:
	/// <summary>
	/// 移動する位置の始点と終点を指定
	/// </summary>
	/// <param name="start">始点</param>
	/// <param name="end">終点</param>
	void InitPos(const Vector2& start, const Vector2& end);

	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// UIを動かし始める
	/// </summary>
	/// <param name="move">入り挙動or戻り挙動を指定</param>
	void Move(UIMove move);

	/// <summary>
	/// 位置を取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetPos();

	/// <summary>
	/// 終了しているかを取得
	/// </summary>
	/// <param name="move">確認したい方の挙動を指定</param>
	/// <returns></returns>
	bool GetEnd(UIMove move);

protected:
	/// <summary>
	/// インターフェース内で指定している諸々を動かす
	/// Updateの最後で呼ぶのを想定
	/// </summary>
	void InterFaceUpdate();

	Vector2 pos = {-1000,-1000};
	
	Vector2 startPos{-1000,-1000};
	Vector2 endPos{ -1000,-1000 };

	Vector2 saveStartPos{ -1000,-1000 };
	Vector2 saveEndPos{ -1000,-1000 };

	TEasing::easeTimer moveTimer = 0.5f;

	UIMove flag = UIMove::START;
};

