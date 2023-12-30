#include "Engine_Shader_Defines.hpp"

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer SkyBuffer
{
    float firstTranslationX;
    float firstTranslationZ;
    float secondTranslationX;
    float secondTranslationZ;
    float brightness;
    float3 padding;
};

Texture2D cloudTexture1 : register(t0);
Texture2D cloudTexture2 : register(t1);

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT SkyPlaneVertexShader(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);
    
    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.vTexcoord = input.vTexcoord;

    return output;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET;
};

PS_OUT SkyPlanePixelShader(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float2 sampleLocation;
    float4 textureColor1;
    float4 textureColor2;
    float4 finalColor;
    
    sampleLocation.x = input.vTexcoord.x + firstTranslationX;
    sampleLocation.y = input.vTexcoord.y + firstTranslationZ;
    
    textureColor1 = cloudTexture1.Sample(LinearSampler, sampleLocation);
    
    sampleLocation.x = input.vTexcoord.x + secondTranslationX;
    sampleLocation.y = input.vTexcoord.y + secondTranslationZ;
    
    textureColor2 = cloudTexture2.Sample(LinearSampler, sampleLocation);
    
    finalColor = lerp(textureColor1, textureColor2, 0.5f);
    
    output.vColor = finalColor * brightness;
    //output.vColor = vector(1.0f, 1.0f, 1.0f, 1.0f);
    
    
    return output;
}

RasterizerState RS_SkyPlane
{
    FillMode = Solid;

	/* 앞면을 컬링하겠다. == 후면을 보여주겠다. */
    CullMode = None;

	/* 앞면을 시계방향으로 쓰겠다. */
    FrontCounterClockwise = false;
};

technique11 CloudDefault
{
    pass SkyPlane
    {
        SetRasterizerState(RS_SkyPlane);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 SkyPlaneVertexShader();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 SkyPlanePixelShader();
    }
}