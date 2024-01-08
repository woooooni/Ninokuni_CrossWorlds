#include "Engine_Shader_Defines.hpp"

Texture2D SunOccluderTexture : register(t0);

cbuffer MatrixBuffer : register(b0)
{
    matrix world, view, projection;
}

cbuffer GodRayBuffer : register(b0)
{
    float4 vLightShaftValue = float4(1.0f, 0.9733f, 1.0f, 2.0f);
    float2 vScreenSunPosition;
}

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

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);

    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.vTexcoord = input.vTexcoord;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_LIGHTSHAFT(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
    
    #define NUM_SAMPLERS 192
    
    float4 vMaskColor = SunOccluderTexture.Sample(PointSampler, input.vTexcoord);
    
    float2 vTexcoord = input.vTexcoord;
    
    float2 vDeltaTexcoord = input.vTexcoord - vScreenSunPosition;

    vDeltaTexcoord *= 1.0f / (float) NUM_SAMPLERS * vLightShaftValue.x;
    
    float fIlluminationDecay = 1.0f;
    
    for (int i = 0; i < NUM_SAMPLERS; ++i)
    {
        vTexcoord -= vDeltaTexcoord;
        
        float4 vNewPixel = SunOccluderTexture.Sample(PointSampler, vTexcoord);
        vNewPixel *= fIlluminationDecay * vLightShaftValue.z;

        vMaskColor += vNewPixel;
        
        fIlluminationDecay *= vLightShaftValue.y;
    }
    
    output.vColor = saturate(vMaskColor * vLightShaftValue.w);
    
    return output;
}


technique11 DefaultTechnique
{
    pass GodRay
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LIGHTSHAFT();
        ComputeShader = NULL;
    }
}