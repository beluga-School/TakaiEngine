#pragma once
#include <string>
#include "EventCamera.h"

//イベントのインターフェース
class IEvent
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//終了条件
	virtual bool End() = 0;

	std::string eventName;

	EventCamera eventCamera;
};