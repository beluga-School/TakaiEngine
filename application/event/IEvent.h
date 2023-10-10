#pragma once
#include <string>
#include "EventCamera.h"
#include <list>

//イベントのインターフェース
class IEvent
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//開始時に実行されるもの
	virtual void Start() = 0;
	
	//終了時に実行されるもの
	virtual void End() = 0;

	//終了条件
	virtual bool EndFlag() = 0;

	virtual ~IEvent(){};

	//実行済みなら立つ
	bool isExecuted = false;

	std::string eventName;

	//カメラを使用するか
	bool isUseCamera = false;

	//プレイヤーを動けなくするか
	
	//黒線が出るか
	bool isUseEventLine = true;
};