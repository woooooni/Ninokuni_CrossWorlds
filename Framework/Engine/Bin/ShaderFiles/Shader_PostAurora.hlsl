#include "Engine_Shader_Defines.hpp"

cbuffer MatrixBuffer : register(b0)
{
    matrix world, view, projection;
}

Texture2D DiffuseTexture : register(t0);

struct VS_IN
{
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.vPosition = sign(float4(input.vPosition, 1.0f));
    output.vTexcoord = (output.vPosition + 1.0f) / 2.0f;
    output.vTexcoord.y = 1.0f - output.vTexcoord.y;

    return output;
}

cbuffer PostAurora : register(b0)
{
    float fBlurRange = 0.09f;
    int iBlurSamplers = 15;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

SamplerState PostAuroraSampler : register(s0)
{
    AddressU = Clamp;
    AddressV = Clamp;
    Filter = MIN_MAG_MIP_LINEAR;
};

PS_OUT PS_MAIN(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 fvColor = 0.0f;
    float fTotal = 0.0f;
    
    for (int x = 0; x < iBlurSamplers; ++x)
    {
        for (int y = 0; y < iBlurSamplers; ++y)
        {
            float2 fvCoord = input.vTexcoord;
            float2 fvDelta = float2(x, y);
            fvDelta -= fvDelta / 2.0f;
            fvDelta /= float2(iBlurSamplers, iBlurSamplers);
            fvColor += DiffuseTexture.Sample(PostAuroraSampler, fvCoord + fvDelta * fBlurRange);
        }

    }

    fvColor /= iBlurSamplers * iBlurSamplers;
    output.vColor = fvColor;
    
    return output;
}



technique11 AuroraDefault
{
    pass Aurora
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}