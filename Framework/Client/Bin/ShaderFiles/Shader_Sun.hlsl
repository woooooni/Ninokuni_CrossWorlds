#include "Engine_Shader_Defines.hpp"

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix, viewMatrix, projectionMatrix;
}

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vNormal : NORMAL;
};

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);

    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.vNormal = normalize(mul(float4(input.vNormal, 0.f), worldMatrix)).xyz;
    output.vTexcoord = input.vTexcoord;
    
    return output;
}

struct PS_OUT
{
    float4 vNormal : SV_TARGET1;
    float4 vViewNormal : SV_TARGET4;
};

PS_OUT PS_MAIN(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
    
    output.vNormal = vector(input.vNormal.xyz * 0.5f + 0.5f, 1.f);
    output.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return output;
}

technique11 DefaultSun
{
    pass Sun
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}