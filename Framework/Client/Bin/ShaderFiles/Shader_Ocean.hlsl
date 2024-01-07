#include "Engine_Shader_Defines.hpp"

float time = 0.f;
float damper = 0.0f;

cbuffer MatrixBuffer : register(b0)
{
    matrix World, View, Projection;
}

struct VS_GerstnerWave
{
    float2 A; // Amplitude - 진폭
    float2 F; // Frequency - 주기
    
    float2 S; // Steepness - 경사
    float2 padding;
    float4 D; // Direction - 방향 
};

struct PS_GerstnerWave
{
    float fresnelBias; // 매질
    float fresnelPower; // 파워
    float waterAmount;
    float reflectAmount;
    
    float3 lightDir;
    float bumpScale;
    
    float4 lightColor;
    float4 ShallowWaterColor;
    float4 deepWaterColor;
};

cbuffer GerstnerBuffer : register(b1)
{
    VS_GerstnerWave VS_Gerstner;
    PS_GerstnerWave PS_Gerstner;
}

cbuffer CameraBuffer : register(b2)
{
    float4 vCameraPosition;
}

struct VS_IN
{
    float3 vPosition : POSITION0;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3x3 vTangent : TANGENT;
    float2 wave0 : TEXCOORD0;
    float2 wave1 : TEXCOORD1;
    float2 wave2 : TEXCOORD2;
    float3 eyeVec : TEXCOORD3;
    float4 vProjPos : TEXCOORD4;
};

float3 CaculateWave(float2 parameterXY, float T)
{
    float H = 0;
    float deltaX = 0;
    float deltaY = 0;
    
    for (int i = 0; i < 2; ++i)
    {
        float forH = 0;
        float forN = 0;
        sincos((VS_Gerstner.D[2 * i] * parameterXY.x +
        VS_Gerstner.D[2 * i + 1] * parameterXY.y +
        T * VS_Gerstner.S[i]) * VS_Gerstner.F[i], forH, forN);
        
        H += forH * VS_Gerstner.A[i];
        deltaX += forN * VS_Gerstner.A[i] * VS_Gerstner.D[2 * i] * VS_Gerstner.F[i];
        deltaY += forN * VS_Gerstner.A[i] * VS_Gerstner.D[2 * i + 1] * VS_Gerstner.F[i];
    }
    
    return float3(H, deltaX, deltaY);
}

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    float3 wave = CaculateWave(input.vPosition.xy, time * 10.0f);
    input.vPosition.z += wave.x;
    
    float3x3 matObjToTangentSpace;
    matObjToTangentSpace[0] = float3(1.0f, 0.0f, wave.y);
    matObjToTangentSpace[1] = float3(0.0f, 1.0f, wave.z);
    matObjToTangentSpace[2] = float3(-wave.y, -wave.z, 1.0f);
    
    float3x3 matTangentToWorld = matObjToTangentSpace;
    matTangentToWorld = mul(matTangentToWorld, World);

    output.vTangent[0] = normalize(matTangentToWorld[0]);
    output.vTangent[1] = normalize(matTangentToWorld[1]);
    output.vTangent[2] = normalize(matTangentToWorld[2]);

    matrix WV, WVP;
    WV = mul(World, View);
    WVP = mul(WV, Projection);
    
    output.vPosition = mul(float4(input.vPosition, 1.0f), WVP);
    float2 translate = float2(time * damper, 0.0f);

    output.wave0 = input.vTexcoord + translate * 2.0f;
    output.wave1 = input.vTexcoord * 2 + translate * 4;
    output.wave2 = input.vTexcoord * 4 + translate;
    
    output.eyeVec = vCameraPosition - output.vPosition;
    output.vProjPos = output.vPosition;
    
    return output;
}

Texture2D NormalTexture : register(t0);
TextureCube Env : register(t1);

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float3x3 TBNtoWorld;
    float3 L = normalize(PS_Gerstner.lightDir);

    TBNtoWorld[0] = input.vTangent[0];
    TBNtoWorld[1] = input.vTangent[1];
    TBNtoWorld[2] = input.vTangent[2];
    
    float3 bump0 = NormalTexture.Sample(LinearSampler, input.wave0);
    float3 bump1 = NormalTexture.Sample(LinearSampler, input.wave1);
    float3 bump2 = NormalTexture.Sample(LinearSampler, input.wave2);
    
    float3 finalBump = 2 * (bump0 + bump1 + bump2) - 3.0f;
    finalBump.xy *= PS_Gerstner.bumpScale;
    finalBump = mul(TBNtoWorld, finalBump);
    finalBump = normalize(finalBump);
   
     
    float DiffuseFactor = saturate(dot(-L, finalBump));
    float4 DiffuseColor = PS_Gerstner.lightColor * DiffuseFactor;
    
    if (DiffuseColor.r < 0.1f)
        DiffuseColor = 0.2f;
    
    float3 V = normalize(input.eyeVec);
    float3 lookup = reflect(-V, finalBump);
    float4 reflectColor = Env.Sample(LinearSampler, lookup);
    
    float facing = 1 - saturate(dot(-V, finalBump));
    float fresnel = PS_Gerstner.fresnelBias + (1 - PS_Gerstner.fresnelBias) * pow(facing, PS_Gerstner.fresnelPower);
    float4 waterColor = lerp(PS_Gerstner.deepWaterColor, PS_Gerstner.ShallowWaterColor, fresnel);
    
    output.vColor = (waterColor * PS_Gerstner.waterAmount + PS_Gerstner.reflectAmount
     * reflectColor * fresnel) * DiffuseColor;
    if(output.vColor.a < 0.1f)
        output.vColor.a = 0.05f;
    
    output.vDepth = float4(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / 1000.f, 0.0f, 0.0f);
    output.vNormal = float4(finalBump.xyz * 0.5f + 0.5f, 0.0f);
    
    return output;
}

technique11 Water
{
    pass Evermore_Water
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}