#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTarget;

texture2D g_EffectUIDiffuseTarget;
texture2D g_EffectUIBrightnessTarget;
texture2D g_EffectUIBlurTarget;

// ¿É¼Ç
bool g_bBlurDraw;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float		fFogFactor : FOG;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_FINAL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	// Diffuse
	vector vDiffuseColor = g_DiffuseTarget.Sample(LinearSampler, In.vTexcoord);

	// Effect_UI
	vector vEffectUIDiffuseColor = g_EffectUIDiffuseTarget.Sample(PointSampler, In.vTexcoord);
	vector vEffectUIBrightnessColor = g_EffectUIBrightnessTarget.Sample(PointSampler, In.vTexcoord);
	vector vEffectUIBlurColor = g_EffectUIBlurTarget.Sample(PointSampler, In.vTexcoord);

	// Output
	if (vEffectUIDiffuseColor.a != 0.f)
		Out.vColor = vEffectUIDiffuseColor;
	else
		Out.vColor = vDiffuseColor;

	//if(g_bBlurDraw)
	//	Out.vColor = Out.vColor + vEffectBlurColor + vEffectUIBlurColor;

	return Out;

	/*vector vOriginEffectColor = g_OriginEffectTarget.Sample(PointSampler, In.vTexcoord);
	vector vOriginBloomColor = g_OriginBloomTarget.Sample(PointSampler, In.vTexcoord);
	vector vBlurBloomColor = g_BlurBloomTarget.Sample(PointSampler, In.vTexcoord);
	vector vBlurEffectColor = g_BlurEffectTarget.Sample(PointSampler, In.vTexcoord);
	Out.vColor += g_OriginEffectTarget.Sample(LinearSampler, In.vTexcoord);
	Out.vColor += vOriginBloomColor;
	Out.vColor += vBlurBloomColor;
	Out.vColor += vBlurEffectColor;*/
}

technique11 DefaultTechnique
{
	// 0
	pass Render_Final
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FINAL();
	}
}
