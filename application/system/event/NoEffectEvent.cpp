#include "NoEffectEvent.h"
#include "EventCameraManager.h"

void NoEffectEvent::Start()
{
	
}

void NoEffectEvent::Update()
{

}

void NoEffectEvent::Draw()
{
}

bool NoEffectEvent::End()
{
	return EventCameraManager::Get()->GetEventEnd();
}
