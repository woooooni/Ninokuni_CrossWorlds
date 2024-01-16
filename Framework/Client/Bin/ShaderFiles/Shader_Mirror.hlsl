#include "Engine_Shader_Defines.hpp"

Texture2D ShaderTexture : register(t0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix, viewMatrix, projectionMatrix;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), worldMatrix);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), worldMatrix)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), worldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

	
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vBloom : SV_TARGET3;
    float4 vSunMask : SV_TARGET4;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = (vector) 1.f;


    Out.vDiffuse = ShaderTexture.Sample(PointSampler, In.vTexcoord);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vSunMask = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (0.3 >= Out.vDiffuse.a)
        discard;
	
	

    return Out;
}


technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}