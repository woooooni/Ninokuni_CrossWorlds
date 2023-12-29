
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix      g_matWorldInv, g_matViewInv, g_matProjInv;

texture2D   g_DepthTarget;

TextureCube	g_DiffuseTexture;
texture2D   g_Diffuse2DTexture, g_Alpha2DTexture;

float       g_fAlpha_Discard;
float3      g_fBlack_Discard;

struct VS_IN
{
	float3		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;	
	float3		vTexUV    : TEXCOORD0;	
    float4      vProjPos  : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV    = In.vTexUV;
    Out.vProjPos  = Out.vPosition;
	
	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV    : TEXCOORD0;
    float4      vProjPos  : TEXCOORD1;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//Out.vColor = (vector)0.f;

	return Out;
}

struct PS_OUT_EFFECT
{
    float4 vDiffuse_All    : SV_TARGET0;
    float4 vDiffuse_None   : SV_TARGET1;
    float4 vDiffuse_Low    : SV_TARGET2;
    float4 vDiffuse_Middle : SV_TARGET3;
    float4 vDiffuse_High   : SV_TARGET4;
    float4 vBloom          : SV_TARGET5;
};

PS_OUT_EFFECT PS_MAIN_DECAL(PS_IN In) // 캐릭터 픽셀에는 찍히면 안된다.
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;

	// Decal Box의 x,y 위치를 UV좌표로 변환.
    float2 vTexUV;
    vTexUV.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
	
	// Decal Box x,y 위치 기준으로 깊이 값을 가져옴.
    vector vDepthDesc = g_DepthTarget.Sample(PointSampler, vTexUV);
    if (vDepthDesc.z == 1.f)
        discard;
    
    float fViewZ      = vDepthDesc.y * 1000.f; // far

    vector vProjPos = (vector) 0.f;
    vProjPos.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
    vProjPos.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
    vProjPos.z = (vDepthDesc.x) * fViewZ;
    vProjPos.w = fViewZ;

    vector vViewPos  = mul(vProjPos, g_matProjInv);
    vector vWorldPos = mul(vViewPos, g_matViewInv);
    vector vLocalPos = mul(vWorldPos, g_matWorldInv);

	// 중점을 기준으로 0.5 이기때문에, 0.5를 뺀 음수값은 상자 밖이므로 자름.
    float3 fObjectAbsPos = abs(vLocalPos.xyz);
    clip(0.5f - fObjectAbsPos);

	// 데칼박스 버퍼가 -0.5 ~0.5 사이므로, 0.5를 더해줘서 UV좌표로 만들어줌.
    float2 fDecalUV      = vLocalPos.xz + 0.5f;
    vector vDiffuseColor = g_Diffuse2DTexture.Sample(LinearSampler, fDecalUV);
    if (vDiffuseColor.a == 0.0f)
        discard;
        
    Out.vDiffuse_All    = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_None   = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Low    = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_High   = float4(0.f, 0.f, 0.f, 0.f);
    Out.vBloom          = float4(0.f, 0.f, 0.f, 0.f);
    
    if (vDiffuseColor.a >= 0.9)
    {
        vDiffuseColor.rgb = float3(0.4f, 0.8f, 1.f);
        Out.vBloom        = vDiffuseColor;
    }
    else
        vDiffuseColor.rgb = float3(0.0f, 0.2f, 0.4f);
    
    Out.vDiffuse_All  = vDiffuseColor;
    Out.vDiffuse_None = vDiffuseColor;
    
    return Out;
}

BlendState DefaultBlendState
{
	BlendEnable[0] = true;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;	
};

technique11 DefaultTechnique
{
	pass SkyBox
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	/*	SetBlendState(DefaultBlendState);
		SetRasterizerState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}	

    pass Decal
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader   = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_DECAL();
    }
}