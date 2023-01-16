#include "Lambert.hlsli"

VSOutput main(float4 pos: POSITION,float3 normal : NORMAL,float2 uv : TEXCOORD)
{
    float3 lightdir = float3(1, -1, 1);
    lightdir = normalize(lightdir);
    //ライトの色(白色)
    float3 lightcolor = float3(1, 1, 1);
    
    VSOutput output;
    output.svpos = mul(mat, pos);

    output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
    output.color.a = m_alpha;
    output.uv = uv;
    
    return output;
}