#include "Engine_Shader_Defines.hpp"

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer SkyBuffer
{
    float fTranslation;
    float fScale;
    float fbrightness;
    float padding;
};

//Texture2D cloudTexture1 : register(t0);
//Texture2D cloudTexture2 : register(t1);

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

Texture2D cloudTexture : register(t0);
Texture2D perturbTexture : register(t1);



PS_OUT SkyPlanePixelShader(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 perturbValue;
    float4 cloudColor;
    
    input.vTexcoord.x = input.vTexcoord.x + fTranslation;
    
    perturbValue = perturbTexture.Sample(LinearSampler, input.vTexcoord);
    
    perturbValue = perturbValue * fScale;
    
    perturbValue.xy = perturbValue.xy + input.vTexcoord.xy + fTranslation;
    
    cloudColor = cloudTexture.Sample(LinearSampler, perturbValue.xy);
    
    cloudColor = cloudColor * fbrightness;
    
    output.vColor = cloudColor;
    
    return output;
    
    //sampleLocation.x = input.vTexcoord.x + firstTranslationX;
    //sampleLocation.y = input.vTexcoord.y + firstTranslationZ;
    
    //textureColor1 = cloudTexture1.Sample(PointSampler, sampleLocation);
    
    //sampleLocation.x = input.vTexcoord.x + secondTranslationX;
    //sampleLocation.y = input.vTexcoord.y + secondTranslationZ;
    
    //textureColor2 = cloudTexture2.Sample(PointSampler, sampleLocation);
    
    //finalColor = lerp(textureColor1, textureColor2, 0.5f);
    
    //finalColor = finalColor * brightness;
    
    //if(finalColor.a <= 0.3f)
    //    discard;
    
    //output.vColor = finalColor;
    //output.vColor = vector(1.0f, 1.0f, 1.0f, 1.0f);

}

RasterizerState RS_SkyPlane
{
    FillMode = Solid;

	/* 앞면을 컬링하겠다. == 후면을 보여주겠다. */
    CullMode = back;

	/* 앞면을 시계방향으로 쓰겠다. */
    FrontCounterClockwise = false;
};

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

technique11 CloudDefault
{
    pass SkyPlane
    {
        SetRasterizerState(RS_SkyPlane);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 SkyPlaneVertexShader();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 SkyPlanePixelShader();
    }
}