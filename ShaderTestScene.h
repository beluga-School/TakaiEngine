#pragma once
#include "IScene.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "LineObject.h"

class ShaderTestScene :
    public IScene
{
public:
    void LoadResource()override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void End()override;

private:
    DebugCamera debugCamera;
    Skydome skydome;

    Obj3d lineObject;
};

