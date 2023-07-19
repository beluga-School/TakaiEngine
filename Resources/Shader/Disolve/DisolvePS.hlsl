#include "Disolve.hlsli"

Texture2D<float4> mainTex : register(t0); //0番スロットに設定されたテクスチャ
Texture2D<float4> maskTex : register(t1); //1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 main = mainTex.Sample(smp, input.uv);
    float4 mask = maskTex.Sample(smp, input.uv);
    
    float4 shadecolor = { 0, 0, 0, 1 };

    //このピクセルのマスクの赤成分を参照して、描画破棄を行う
    clip(mask.r - disolve);
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            //アンビエント = オブジェクトの元の色 * 暗くする率 * 光の色
            float3 ambient = main.rgb * m_ambient * dirLights[i].lightcolor;
            
            //ディフューズ
            //光との内積値
            float intensity = saturate(dot(normalize(input.normal), dirLights[i].lightv));
            //オブジェクトの色
            float3 difColor = main.rgb;
            
            //ディフューズ = オブジェクトの元の色 * 光との内積値 * 光の色
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;
            
            //スペキュラー
            //視線ベクトル
            float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
            //入射光
            float3 lightDir = normalize(dirLights[i].lightv);
            
            float3 normalizeVec = normalize(input.normal);
            //反射光
            float3 reflectDir = -lightDir + 2 * normalizeVec * dot(normalizeVec, lightDir);
            
            float3 specular = pow(saturate(dot(reflectDir, eyeDir)), 20) * dirLights[i].lightcolor;
            
            shadecolor.rgb += (ambient + diffuse + specular);
            shadecolor.a += 1;
        }
    }
    
    //どっちが正解？
    return shadecolor * color;
}