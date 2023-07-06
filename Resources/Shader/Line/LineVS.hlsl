#include "Line.hlsli"

VSOutput main(float4 pos : POSITION)
{
    //�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wpos = mul(world, pos);
    
    VSOutput output;
    output.svpos = mul(viewproj, wpos);
    output.worldPos = wpos;
    
    return output;
}