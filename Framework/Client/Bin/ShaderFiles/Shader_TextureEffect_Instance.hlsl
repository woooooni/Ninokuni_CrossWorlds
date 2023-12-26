#include "Engine_Shader_Defines.hpp"


matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_AlphaTexture;

float       g_fAlpha_Discard;
float3      g_fBlack_Discard;

float       g_fBlurPower = 0.f;

struct EffectDesc
{
	float2 g_fUVIndex;
	float2 g_fMaxCount;

	float2 g_fUVFlow;
	int	   g_iUVLoop;
	float  g_fAlpha;

	float4 g_fAdditiveDiffuseColor;

	float3 g_fBloomPower;
	float  g_fBlurPower;
};
EffectDesc g_EffectDesc[1000];




struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;

	uint		iInstanceID : SV_INSTANCEID;

};




struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

	uint	iInstanceID : SV_INSTANCEID;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float4x4	InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	Out.vPosition = mul(float4(In.vPosition, 1.f), InstanceWorld);
	Out.vPosition = mul(Out.vPosition, matVP);
	
	Out.vTexUV = float2(
		((g_EffectDesc[In.iInstanceID].g_fUVIndex.x + In.vTexUV.x) / g_EffectDesc[In.iInstanceID].g_fMaxCount.x) + g_EffectDesc[In.iInstanceID].g_fUVFlow.x,
		((g_EffectDesc[In.iInstanceID].g_fUVIndex.y + In.vTexUV.y) / g_EffectDesc[In.iInstanceID].g_fMaxCount.y) + g_EffectDesc[In.iInstanceID].g_fUVFlow.y);

	Out.iInstanceID = In.iInstanceID;

	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

	uint	iInstanceID : SV_INSTANCEID;
};

struct PS_OUT
{
	float4 vDiffuse_All    : SV_TARGET0;
	float4 vDiffuse_None   : SV_TARGET1;
	float4 vDiffuse_Low    : SV_TARGET2;
	float4 vDiffuse_Middle : SV_TARGET3;
	float4 vDiffuse_High   : SV_TARGET4;
	float4 vBloom          : SV_TARGET5;
};



float4 Caculation_Brightness(float4 vColor, uint iInstanceID)
{
	float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

	float fPixelBrightness = dot(vColor.rgb, g_EffectDesc[iInstanceID].g_fBloomPower.rgb);
	if (fPixelBrightness > 0.99f)
		vBrightnessColor = float4(vColor.rgb, 1.0f);

	return vBrightnessColor;
}



PS_OUT PS_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (0 < g_EffectDesc[In.iInstanceID].g_iUVLoop)
	{
		if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
			discard;
	}

	vector vDiffuseColor = vector(g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor.rgb, g_EffectDesc[In.iInstanceID].g_fAlpha);
	if (vDiffuseColor.a < g_fAlpha_Discard)
		discard;

	Out.vDiffuse_All = vDiffuseColor;
	Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);

	if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
		Out.vDiffuse_None = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
		Out.vDiffuse_Low = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
		Out.vDiffuse_Middle = vDiffuseColor;
	else
		Out.vDiffuse_High = vDiffuseColor;

	// Bloom
	Out.vBloom = Caculation_Brightness(vDiffuseColor, In.iInstanceID);

	return Out;

};

PS_OUT PS_NO_DIFFUSE_WITH_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (0 < g_EffectDesc[In.iInstanceID].g_iUVLoop)
	{
		if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
			discard;
	}

	vector vTextureAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);
	if ((vTextureAlpha.r <= g_fBlack_Discard.x) && (vTextureAlpha.g <= g_fBlack_Discard.y) && (vTextureAlpha.b <= g_fBlack_Discard.z))
		discard;

	vector vDiffuseColor = vector(g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor.rgb, g_EffectDesc[In.iInstanceID].g_fAlpha);
	if (vDiffuseColor.a <= g_fAlpha_Discard)
		discard;

	Out.vDiffuse_All = vDiffuseColor;
	Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);

	if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
		Out.vDiffuse_None = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
		Out.vDiffuse_Low = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
		Out.vDiffuse_Middle = vDiffuseColor;
	else
		Out.vDiffuse_High = vDiffuseColor;

	// Bloom
	Out.vBloom = Caculation_Brightness(vDiffuseColor, In.iInstanceID);

	return Out;
};

PS_OUT PS_NO_ALPHA_WITH_DIFFUSE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (0 < g_EffectDesc[In.iInstanceID].g_iUVLoop)
	{
		if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
			discard;
	}

	vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (vDiffuseColor.a < g_fAlpha_Discard ||
		vDiffuseColor.r < g_fBlack_Discard.r && vDiffuseColor.g < g_fBlack_Discard.g && vDiffuseColor.b < g_fBlack_Discard.b)
		discard;

	vDiffuseColor.rgb = saturate((vDiffuseColor.rgb + g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor.rgb));
	vDiffuseColor.a = g_EffectDesc[In.iInstanceID].g_fAlpha;
	if (Out.vDiffuse_All.a <= g_fAlpha_Discard)
		discard;

	Out.vDiffuse_All = vDiffuseColor;
	Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);

	if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
		Out.vDiffuse_None = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
		Out.vDiffuse_Low = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
		Out.vDiffuse_Middle = vDiffuseColor;
	else
		Out.vDiffuse_High = vDiffuseColor;

	// Bloom
	Out.vBloom = Caculation_Brightness(vDiffuseColor, In.iInstanceID);

	return Out;

};

PS_OUT PS_BOTH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (0 < g_EffectDesc[In.iInstanceID].g_iUVLoop)
	{
		if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
			discard;
	}

	vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if ((vDiffuseColor.r <= g_fBlack_Discard.x) && (vDiffuseColor.g <= g_fBlack_Discard.y) && (vDiffuseColor.b <= g_fBlack_Discard.z))
		discard;

	vDiffuseColor.rgb = saturate((vDiffuseColor.rgb + g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor.rgb));
	vDiffuseColor.a = g_EffectDesc[In.iInstanceID].g_fAlpha;
	if (vDiffuseColor.a <= g_fAlpha_Discard)
		discard;

	vector vTextureAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);
	if ((vTextureAlpha.r <= g_fBlack_Discard.x) && (vTextureAlpha.g <= g_fBlack_Discard.y) && (vTextureAlpha.b <= g_fBlack_Discard.z))
		discard;

	Out.vDiffuse_All = vDiffuseColor;
	Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);

	if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
		Out.vDiffuse_None = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
		Out.vDiffuse_Low = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
		Out.vDiffuse_Middle = vDiffuseColor;
	else
		Out.vDiffuse_High = vDiffuseColor;

	// Bloom
	Out.vBloom = Caculation_Brightness(vDiffuseColor, In.iInstanceID);

	return Out;

};

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEFAULT();
	}


	pass NoAlphaPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NO_ALPHA_WITH_DIFFUSE();
	}

	pass NoDiffusePass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NO_DIFFUSE_WITH_ALPHA();
	}


	pass BothPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BOTH();
	}

}