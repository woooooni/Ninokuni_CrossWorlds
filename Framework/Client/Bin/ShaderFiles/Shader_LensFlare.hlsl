#include "Engine_Shader_Defines.hpp"

#define FLT_MAX 3.402823466e+38F

sampler PointClampSampler = sampler_state
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
    MaxAnisotropy = 16;
    MaxLOD = FLT_MAX;
    ComparisonFunc = NEVER;
};

struct VSToGS
{
    float4 vPosition : SV_POSITION;
    nointerpolation uint vid : VERTEXID;
};

VSToGS LensFlareVS(uint vid : SV_VERTEXID)
{
    VSToGS o = (VSToGS) 0;
    
    o.vPosition = 0;
    o.vid = vid;
  
    return o;
}

struct Light
{
    int iCats_Shadows;
    int iUse_Cascades;
    float fVolumetric_Strength;
    int iScreen_space_shadow;
    
    float4 vScreen_space_position;
    float4 vPosition;
    float4 vDirection;
    float4 vColor;
};

cbuffer CBLightBuffer : register(b0)
{
    Light current_light;
};


struct GSToPS
{
    float4 vPosition : SV_POSITION;
    float3 vTexcoord : TEXCOORD0; // 텍스쳐 좌표(x,y) 광원으로부터의 거리 (z)
    nointerpolation uint iSel : TEXCOORD1;
    nointerpolation float4 vOpa : TEXCOORD2; // 투명기록
};

Texture2D lens0 : register(t0);
Texture2D lens1 : register(t1);
Texture2D lens2 : register(t2);
Texture2D lens3 : register(t3);
Texture2D lens4 : register(t4);
Texture2D lens5 : register(t5);
Texture2D lens6 : register(t6);
Texture2D depth_texture : register(t7);



inline void AppendToStream(inout TriangleStream<GSToPS> triStream, GSToPS p1, uint selector, float2 posMod, float2 size)
{
    // 뷰포트 광원 위치
    float2 pos = (current_light.vScreen_space_position.xy - 0.5f) * float2(2.0f, -2.0f);
    // posMod에 따른 후광 위치
    float2 moddedpos = pos * posMod;
    // 후광은 광원 위치를 기반.
    float dis = distance(pos, moddedpos);
    
    // 중심 오프셋 기준으로 사각형을 그린다.
    p1.vPosition.xy = moddedpos + float2(-size.x, -size.y);
    p1.vTexcoord.z = dis;
    p1.iSel = selector;
    p1.vTexcoord.xy = float2(0.0f, 0.0f);
    triStream.Append(p1);

    p1.vPosition.xy = moddedpos + float2(-size.x, size.y);
    p1.vTexcoord.xy = float2(0.0f, 1.0f);
    triStream.Append(p1);
    
    p1.vPosition.xy = moddedpos + float2(size.x, -size.y);
    p1.vTexcoord.xy = float2(1.0f, 0.0f);
    triStream.Append(p1);
    
    p1.vPosition.xy = moddedpos + float2(size.x, size.y);
    p1.vTexcoord.xy = float2(1.0f, 1.0f);
    triStream.Append(p1);
}

[maxvertexcount(4)]
void LensFlareGS(point VSToGS p[1], inout TriangleStream<GSToPS> triStream)
{
    GSToPS output = (GSToPS) 0;
    float2 flareSize = float2(256.0f, 256.0f);
    
    // 원래 텍스쳐의 크기
    // 후광맵의 사각형의 크기에 따라 후광 플레어의 크기를 결정한다.
    [branch]
    switch (p[0].vid)
    {
        case 0:
            lens0.GetDimensions(flareSize.x, flareSize.y);
            break;
        case 1:
            lens1.GetDimensions(flareSize.x, flareSize.y);
            break;
        case 2:
            lens2.GetDimensions(flareSize.x, flareSize.y);
            break;
        case 3:
            lens3.GetDimensions(flareSize.x, flareSize.y);
            break;
        case 4:
            lens4.GetDimensions(flareSize.x, flareSize.y);
            break;
        case 5:
            lens5.GetDimensions(flareSize.x, flareSize.y);
            break;
        case 6:
            lens6.GetDimensions(flareSize.x, flareSize.y);
            break;
        default:
            break;
    };
    
    uint width, height, levels;
    
    // 깊이 버퍼에서 원래 뷰포트의 크기를 가져온다.
    depth_texture.GetDimensions(0, width, height, levels);
    float2 ScreenResolution = float2(width, height);

    // 후광 크기 정규화
    flareSize /= ScreenResolution;

    // 적용 단계에서 처리된 빛의 깊이 값을 가져온다.
    float referenceDepth = saturate(current_light.vScreen_space_position.z);
    
    // 후광의 불 투명도 결정
    const float2 step = 1.0f / ScreenResolution;
    const float2 range = 10.5f * step; // 맞춤형 10.5f
    float samples = 0.0f;
    float accdepth = 0.0f;
    
    for (float y = -range.y; y <= range.y; y += step.y)
    {
        for (float x = -range.x; x <= range.x; x += step.x)
        {
            samples += 1.0f;
            accdepth += depth_texture.SampleLevel(PointClampSampler, current_light.vScreen_space_position.xy + float2(x, y), 0).r >= referenceDepth - 0.001f ? 1 : 0;
        }
    }

    accdepth /= samples;
    
    output.vPosition = float4(0.0f, 0.0f, 0.0f, 1.0f);
    output.vOpa = float4(accdepth, 0.0f, 0.0f, 0.0f);

    // 후광의 가시성에 따라서 해당 텍스쳐를 생성할 시기를 결정한다.
    [branch]
    if(accdepth > 0.0f)
    {
        // 광원 감쇄 방향(1차원 선형 표현)
        const float MODS[] = { 1, 0.55, 0.4, 0.1, -0.1, -0.3, -0.5 };
        AppendToStream(triStream, output, p[0].vid, MODS[p[0].vid], flareSize);
    }
}

float4 LensFlarePS(GSToPS input) : SV_TARGET
{
    float4 vColor = 0;
    
    [branch]
    switch (input.iSel)
    {
        case 0:
            vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 1:
            vColor = lens1.SampleLevel(PointClampSampler, input.vTexcoord.xy, 0);
            break;
        case 2:
            vColor = lens2.SampleLevel(PointClampSampler, input.vTexcoord.xy, 0);
            break;
        case 3:
            vColor = lens3.SampleLevel(PointClampSampler, input.vTexcoord.xy, 0);
            break;
        case 4:
            vColor = lens4.SampleLevel(PointClampSampler, input.vTexcoord.xy, 0);
            break;
        case 5:
            vColor = lens5.SampleLevel(PointClampSampler, input.vTexcoord.xy, 0);
            break;
        case 6:
            vColor = lens6.SampleLevel(PointClampSampler, input.vTexcoord.xy, 0);
            break;
        default:
            break;
    };
    
    // 후광 감소
    vColor *= 1.1f - saturate(input.vTexcoord.z);
    
    // 후광 투명도 오버레이
    vColor *= input.vOpa.x;
    
    return vColor;
}

BlendState AdditiveBlendState
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = true;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;
    BlendOp[0] = ADD;
    RenderTargetWriteMask[0] = 15;
};



technique11 DefaultTechique
{
    pass LensFlare
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(AdditiveBlendState, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 LensFlareVS();
        GeometryShader = compile gs_5_0 LensFlareGS();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 LensFlarePS();
    }
}