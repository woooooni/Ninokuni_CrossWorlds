#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTarget;

Texture2D g_ScreenEffectTexture;
Texture2D g_ScreenTarget;

float2 g_vUVWeight;

matrix g_UVRoationMatrix;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};


float3 Gamma_ForMonitor(float3 _rgb, float _f = 1.f / 2.2f)
{
    return pow(_rgb, _f); // 0.45454545 = 1/2.2 }
}


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

VS_OUT VS_MAIN_SCREEN_EFFECT(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    matrix matWV, matWVP;

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
	Out.vColor = vDiffuseColor;

	return Out;
}

PS_OUT PS_MAIN_SCREEN_SWORDMAN_SPLIT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vMaskColor = g_ScreenEffectTexture.Sample(LinearSampler, In.vTexcoord);
    float2 vNewTexCoord = In.vTexcoord;
    
    vNewTexCoord.x -= vMaskColor.r * g_vUVWeight.x;
    vNewTexCoord.x += vMaskColor.g * g_vUVWeight.y;
   
    
    if (0.f <= vNewTexCoord.x && 1.f >= vNewTexCoord.x)
        Out.vColor = g_ScreenTarget.Sample(LinearSampler, vNewTexCoord);
    else
        Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    Out.vColor.a = 1.f;
	
    return Out;
}

PS_OUT PS_MAIN_SCREEN_DESTROYER_BREAK(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vMaskColor = g_ScreenEffectTexture.Sample(LinearSampler, In.vTexcoord);
    float2 vNewTexCoord = In.vTexcoord + (vMaskColor.rg * 0.05f);
    
    if (0.f > vNewTexCoord.x || 1.f < vNewTexCoord.x || 0.f > vNewTexCoord.y || 1.f < vNewTexCoord.y)
        Out.vColor = float4(0.f, 0.f, 0.f, 1.f);
    else
        Out.vColor = g_ScreenTarget.Sample(LinearSampler, vNewTexCoord);
    
    
        
        
    
    Out.vColor.a = 1.f;
	
    return Out;
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
	
	// 1
    pass Render_SwordMan_Split
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_SCREEN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SCREEN_SWORDMAN_SPLIT();
    }

    // 2 
    pass Render_Destroyer_Break
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_SCREEN_EFFECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SCREEN_DESTROYER_BREAK();
    }

    //3
    pass Temp3
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

    // 4
    pass Temp4
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
