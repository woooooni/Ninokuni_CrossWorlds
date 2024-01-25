#include "Engine_Shader_Defines.hpp"


matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_AlphaTexture;
Texture2D	g_DistortionTexture;

float4		g_vColor;
float		g_fAlpha;
float       g_fAlpha_Discard;
float3      g_fBlack_Discard;

float2		g_vDistortion;

float		g_fBlurPower;
float4		g_vBloomPower;


float4 g_vCamPosition;


// UVs
float2 g_vUVFlow;
float2 g_vUVIndex;
float2 g_vMaxUVCount;
int g_iUVLoop;






struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matrix matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexUV = float2(((g_vUVIndex.x + In.vTexUV.x) / g_vMaxUVCount.x) + g_vUVFlow.x,
						((g_vUVIndex.y + In.vTexUV.y) / g_vMaxUVCount.y) + g_vUVFlow.y);
	
	


	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vDiffuse_All : SV_TARGET0;
    float4 vDiffuse_None : SV_TARGET1;
    float4 vDiffuse_Low : SV_TARGET2;
    float4 vDiffuse_Middle : SV_TARGET3;
    float4 vDiffuse_High : SV_TARGET4;
    float4 vBloom : SV_TARGET5;
    float4 vDistortion : SV_TARGET6;
};

float4 Caculation_Brightness(float4 vColor)
{
    float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

    float fPixelBrightness = dot(vColor.rgb, g_vBloomPower.rgb);
    if (fPixelBrightness > 0.99f)
        vBrightnessColor = float4(vColor.rgb, 1.0f);

    return vBrightnessColor;
}


PS_OUT PS_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    if (0 > g_iUVLoop)
    {
        if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
            discard;
    }

    Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);
	
    vector vDiffuseColor = vector(g_vColor.rgb, g_fAlpha);
    if (vDiffuseColor.a < g_fAlpha_Discard)
        discard;

    Out.vDiffuse_All = vDiffuseColor;
    if (g_fBlurPower <= 0.0f)
        Out.vDiffuse_None = vDiffuseColor;
    else if (g_fBlurPower > 0.0f && g_fBlurPower <= 0.3f)
        Out.vDiffuse_Low = vDiffuseColor;
    else if (g_fBlurPower > 0.3f && g_fBlurPower <= 0.7f)
        Out.vDiffuse_Middle = vDiffuseColor;
    else
        Out.vDiffuse_High = vDiffuseColor;

	        // Bloom
    Out.vBloom = Caculation_Brightness(vDiffuseColor);
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;

	return Out;

};

PS_OUT PS_NO_ALPHA_WITH_DIFFUSE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    if (0 < g_iUVLoop)
    {
        if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < -1.f) || (In.vTexUV.y < -1.f))
            discard;
    }

    Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);
	
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    vDiffuseColor.a = g_fAlpha;
    
    if (vDiffuseColor.a < g_fAlpha_Discard || vDiffuseColor.r < g_fBlack_Discard.r && vDiffuseColor.g < g_fBlack_Discard.g && vDiffuseColor.b < g_fBlack_Discard.b)
        discard;

    vDiffuseColor.rgb = saturate((vDiffuseColor.rgb + g_vColor.rgb));
    
    
	
    Out.vDiffuse_All = vDiffuseColor;
    if (g_fBlurPower <= 0.0f)
        Out.vDiffuse_None = vDiffuseColor;
    else if (g_fBlurPower > 0.0f && g_fBlurPower <= 0.3f)
        Out.vDiffuse_Low = vDiffuseColor;
    else if (g_fBlurPower > 0.3f && g_fBlurPower <= 0.7f)
        Out.vDiffuse_Middle = vDiffuseColor;
    else
        Out.vDiffuse_High = vDiffuseColor;

	        // Bloom
    Out.vBloom = Caculation_Brightness(vDiffuseColor);
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;

	return Out;

};

PS_OUT PS_NO_DIFFUSE_WITH_ALPHA(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (0 < g_iUVLoop)
    {
        if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
            discard;
    }

    Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);
	
    
    vector vTextureAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);
    if ((vTextureAlpha.r <= g_fBlack_Discard.x) && (vTextureAlpha.g <= g_fBlack_Discard.y) && (vTextureAlpha.b <= g_fBlack_Discard.z))
        discard;
    
    if (vTextureAlpha.r <= g_fAlpha_Discard)
        discard;
    
    vector vDiffuseColor = vector(g_vColor.rgb, vTextureAlpha.r);
	
    Out.vDiffuse_All = vDiffuseColor;
    if (g_fBlurPower <= 0.0f)
        Out.vDiffuse_None = vDiffuseColor;
    else if (g_fBlurPower > 0.0f && g_fBlurPower <= 0.3f)
        Out.vDiffuse_Low = vDiffuseColor;
    else if (g_fBlurPower > 0.3f && g_fBlurPower <= 0.7f)
        Out.vDiffuse_Middle = vDiffuseColor;
    else
        Out.vDiffuse_High = vDiffuseColor;

	        // Bloom
    Out.vBloom = Caculation_Brightness(vDiffuseColor);
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;
    
    return Out;

};



PS_OUT PS_BOTH(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (0 < g_iUVLoop)
    {
        if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
            discard;
    }

    Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);
	
   
    
    vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    if ((vDiffuseColor.r <= g_fBlack_Discard.x) && (vDiffuseColor.g <= g_fBlack_Discard.y) && (vDiffuseColor.b <= g_fBlack_Discard.z))
        discard;
    
    vDiffuseColor.rgb = saturate((vDiffuseColor.rgb + g_vColor.rgb));

    if (vDiffuseColor.a <= g_fAlpha_Discard)
        discard;
    
    vector vTextureAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);
    if ((vTextureAlpha.r <= g_fBlack_Discard.x) && (vTextureAlpha.g <= g_fBlack_Discard.y) && (vTextureAlpha.b <= g_fBlack_Discard.z))
        discard;
    
    if(vTextureAlpha.r <= g_fAlpha_Discard)
        discard;
    
    vDiffuseColor.a = vTextureAlpha.r;
	
    Out.vDiffuse_All = vDiffuseColor;
    if (g_fBlurPower <= 0.0f)
        Out.vDiffuse_None = vDiffuseColor;
    else if (g_fBlurPower > 0.0f && g_fBlurPower <= 0.3f)
        Out.vDiffuse_Low = vDiffuseColor;
    else if (g_fBlurPower > 0.3f && g_fBlurPower <= 0.7f)
        Out.vDiffuse_Middle = vDiffuseColor;
    else
        Out.vDiffuse_High = vDiffuseColor;

	        // Bloom
    Out.vBloom = Caculation_Brightness(vDiffuseColor);
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;

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
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DEFAULT();
	}


	pass NoAlphaPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_NO_ALPHA_WITH_DIFFUSE();
	}

	pass NoDiffusePass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_NO_DIFFUSE_WITH_ALPHA();
	}


	pass BothPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BOTH();
	}

}