#include "Phong.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float3 lightdir = float3(1, -1, 1);
    lightdir = normalize(lightdir);
    //ライトの色(白色)
    float3 lightcolor = float3(1, 1, 1);
    
    float3 ambient = m_ambient;
    float3 diffuse = dot(-lightdir,normal)*m_diffuse;
    
    //視点座標(視点座標を持ってくる必要がある？)
    const float3 eye = float3(0, 0, -20);
    
    const float shininess = 4.0f;
    
    float3 eyedir = normalize(eye - pos.xyz);
    
    float3 reflect = normalize(lightcolor + 2 * dot(-lightdir, normal) * normal);
    
    float3 specular = pow(saturate(dot(reflect, eye)), shininess) * m_specular;
    
    VSOutput output;
    output.svpos = mul(mat, pos);

    output.color.rgb = (ambient * diffuse + specular) * lightcolor;
    output.color.a = m_alpha;
    output.uv = uv;
    
    return output;
}