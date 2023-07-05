#pragma once
#include "IScene.h"
#include "DebugCamera.h"
#include "Skydome.h"

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
    Obj3d testSphere;
    Skydome skydome;

    Vector3 lightdir = { 0.0f,-0.8f,0.0f };
};

