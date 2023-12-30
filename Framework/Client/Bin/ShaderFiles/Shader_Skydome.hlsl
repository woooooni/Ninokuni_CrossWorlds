#include "Engine_Shader_Defines.hpp"

Texture2D SkydomeTexture : register(t0);

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer GradientBuffer
{
    float4 apexColor;
    float4 centerColor;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vdomePosition : TEXCOORD1;
};

VS_OUT SkyDomeVertexShader(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    Matrix matWV, matWVP;
    
    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);
    
    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.vdomePosition = float4(input.vPosition, 1.0f);
    
    output.vTexcoord = input.vTexUV;
    
    return output;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vdomePosition : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT SkyDomePixelShader(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float height = input.vdomePosition.y;
    
    if(height < 0.0f)
    {
        height = 0.0f;
    }
    
    float4 SkyColor = SkydomeTexture.Sample(LinearSampler, input.vTexcoord);
    
   //output.vColor = lerp(centerColor, apexColor, height);
    output.vColor = SkyColor;
    
   return output;
}


RasterizerState RS_SkyDome
{
    AntialiasedLineEnable = false;
    CullMode = None;
    FillMode = solid;
    FrontCounterClockwise = false;
};

technique11 Sky
{
    pass SkyDome
    {
        SetRasterizerState(RS_SkyDome);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 SkyDomeVertexShader();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 SkyDomePixelShader();
    }
}