#pragma once
#include <string>
#include "EventCamera.h"
#include <list>

//イベントのインターフェース
class IEvent
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//終了条件
	virtual bool End() = 0;

	//実行済みなら立つ
	bool isExecuted = false;

	std::string eventName;

	//カメラを使用するか
	bool isUseCamera = false;

	//プレイヤーを動けなくするか

};