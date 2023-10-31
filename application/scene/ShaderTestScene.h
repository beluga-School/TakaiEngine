#pragma once
#include "IScene.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "Billboard.h"
#include "ImguiManager.h"

/*! ShaderTestScene
    シェーダーテストクラス
*/
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

    GUI gui = {"CG5_kadai"};

    Obj3d testObj;

    Texture* sub;
    Texture* blendMask;
    Texture* disolveMask;

    Texture* bayerMatrix;

    BillboardY billboard;

    enum class DrawMode
    {
        TextureBlend,
        Disolve,
        Noise,
        bayerMatrix,
    }mode = DrawMode::bayerMatrix;

    std::vector<std::string> handles;
    std::string output = "";
    std::string oldOutput = "";
};

