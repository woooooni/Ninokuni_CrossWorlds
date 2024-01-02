#include "Engine_Shader_Defines.hpp"
#define PI 3.1415926f

struct VS_AuroraInfo
{
    float fWaveScale;
    float fWaveAmplitude;
    float fWaveSpeed;
    float fPadding;
    
    float3 vScalar;
    float fPadding2;
    float3 vPosition;
    float fPadding3;
};

struct PS_AuroraInfo
{
    float fGlowPhaseScale;
    float fGlowWaveSpeed;
    float fGlowPhaseSpeed;
    float fGlowPhaseBias;
  
    float2 vTextureTiling;
    float2 vTextureSpeed;
};

float fTime = 0.0f;

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer AuroraBuffer : register(b1)
{
    VS_AuroraInfo VS_AuroraDesc;
    PS_AuroraInfo PS_AuroraDesc;
}

struct VS_INPUT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
    float fPhase : TEXCOORD3;
};

VS_OUTPUT VS_AURORA(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float4 fvPos = float4(input.vPosition, 1.0f);
    float fTmp = fvPos.y;
    fvPos.y = fvPos.z;
    fvPos.z = fTmp;
    fvPos.z += VS_AuroraDesc.fWaveAmplitude *
    sin((fvPos.x + fTime * VS_AuroraDesc.fWaveSpeed) * 2.0f * PI * VS_AuroraDesc.fWaveScale);
    fvPos.xyz *= VS_AuroraDesc.vScalar;
    fvPos.xyz += VS_AuroraDesc.vPosition;
    
    matrix matWV, matWVP;
    
    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);
    
    output.vPosition = mul(fvPos, matWVP);
    output.vTexcoord = input.vPosition.xz + 0.5f;
    output.fPhase = input.vPosition.x + 0.5f;
    
    return output;
}

struct PS_INPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
    float fPhase : TEXCOORD3;
};

struct PS_OUTPUT
{
    float4 vColor : SV_TARGET0;
};

Texture2D AuroraTexture : register(t0);
SamplerState AuroraSampler : register(s0)
{
    AddressU = Mirror;
    AddressV = Mirror;
    Filter = MIN_MAG_MIP_LINEAR;
};

PS_OUTPUT PS_AURORA(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    float2 textureCoordNate = input.vTexcoord + PS_AuroraDesc.vTextureSpeed * fTime;
    float4 fvColor = AuroraTexture.Sample(AuroraSampler, textureCoordNate * PS_AuroraDesc.vTextureTiling);
    float fPhaseFactor = (sin(input.fPhase * 2.0f * PI * PS_AuroraDesc.fGlowPhaseScale)
    + cos((input.fPhase + PS_AuroraDesc.fGlowPhaseSpeed * fTime) * 2.0f * PI * PS_AuroraDesc.fGlowPhaseScale)
    + 2.0f) / 4.0f * PS_AuroraDesc.fGlowPhaseBias;

    
    output.vColor = lerp(fvColor, 0.0f, fPhaseFactor);
    
    return output;
}

BlendState BS_Blend
{
    BlendEnable[0] = true;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;
    BlendOp[0] = ADD;
    SrcBlendAlpha[0] = ONE;
    DestBlendAlpha[0] = Zero;
    BlendOpAlpha[0] = ADD;
    RenderTargetWriteMask[0] = 0x0f;
};

RasterizerState RS_AuroraCull
{
    AntialiasedLineEnable = false;
    CullMode = None;
    FillMode = solid;
    FrontCounterClockwise = false;
};

technique11 AuroraDefault
{
    pass Aurora
    {
        SetRasterizerState(RS_AuroraCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_AURORA();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_AURORA();
    }
}