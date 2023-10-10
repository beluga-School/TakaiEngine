#include "NoEffectEvent.h"
#include "EventCameraManager.h"

void NoEffectEvent::Start()
{
	
}

void NoEffectEvent::End()
{
}

void NoEffectEvent::Initialize()
{
}

void NoEffectEvent::Update()
{

}

void NoEffectEvent::Draw()
{
}

bool NoEffectEvent::EndFlag()
{
	return EventCameraManager::Get()->GetEventEnd();
}
