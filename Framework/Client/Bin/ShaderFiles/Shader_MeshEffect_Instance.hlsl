#include "Engine_Shader_Defines.hpp"


matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_AlphaTexture;

float       g_fAlpha_Discard;
float3      g_fBlack_Discard;

float       g_fBlurPower = 0.f;

Texture2D   g_DissolveTexture;
float4      g_vDissolveColor;
float       g_fDissolveTotalTime = 5.f;
float       g_fDissolveWeight    = 0.f;

Texture2D   g_DistortionTexture;

float4      g_vCamPosition;

float4      g_vDistortion;

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
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;

	uint	    iInstanceID : SV_INSTANCEID;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
    float3      vTangent : TANGENT;
    float3      vBinormal : BINORMAL;
	float4		vProjPos : TEXCOORD1;
    float4      vWorldPosition : TEXCOORD2;
    
	uint		iInstanceID : SV_INSTANCEID;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	float4x4 InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), InstanceWorld);
	Out.vPosition = mul(Out.vPosition, matVP);
    Out.vWorldPosition = mul(Out.vPosition, InstanceWorld);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), InstanceWorld));
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), InstanceWorld)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    
	EffectDesc Desc = g_EffectDesc[In.iInstanceID];
	Out.vTexUV = float2(
		((Desc.g_fUVIndex.x + In.vTexUV.x) / Desc.g_fMaxCount.x) + Desc.g_fUVFlow.x,
		((Desc.g_fUVIndex.y + In.vTexUV.y) / Desc.g_fMaxCount.y) + Desc.g_fUVFlow.y);

	Out.vProjPos = Out.vPosition;
	Out.iInstanceID = In.iInstanceID;

	return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vProjPos : TEXCOORD1;
    float4 vWorldPosition : TEXCOORD2;
    
    uint iInstanceID : SV_INSTANCEID;
};

struct PS_OUT
{
	float4 vDiffuse_All    : SV_TARGET0;
	float4 vDiffuse_None   : SV_TARGET1;
	float4 vDiffuse_Low    : SV_TARGET2;
	float4 vDiffuse_Middle : SV_TARGET3;
	float4 vDiffuse_High   : SV_TARGET4;
	float4 vBloom          : SV_TARGET5;
    float4 vDistortion     : SV_TARGET6;
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
	
    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha   = saturate(1.0 - g_fDissolveWeight / g_fDissolveTotalTime + vDissoveTexture.r);
	
	// ÇÈ¼¿ »ý·«
    if (fDissolveAlpha < 0.3)
        discard;
    else
    {
        vector vDiffuseColor = vector(g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor.rgb, g_EffectDesc[In.iInstanceID].g_fAlpha);
        if (vDiffuseColor.a < g_fAlpha_Discard)
            discard;
        
        Out.vDiffuse_None   = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Low    = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_High   = float4(0.f, 0.f, 0.f, 0.f);
        
		// ÇÈ¼¿ »ö»ó ÁöÁ¤ : ¸í¾Ï ¿¬»ê X
        if (fDissolveAlpha < 0.5)
        {
            Out.vDiffuse_All = g_vDissolveColor;
            if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
                Out.vDiffuse_None = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
                Out.vDiffuse_Low = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
                Out.vDiffuse_Middle = g_vDissolveColor;
            else
                Out.vDiffuse_High = g_vDissolveColor;
			
			// Bloom
            Out.vBloom = float4(g_vDissolveColor.r, g_vDissolveColor.g, g_vDissolveColor.b, 0.5f);
        }
	
	    // ±âº» ÇÈ¼¿ ·»´õ¸µ
        else
        {
            Out.vDiffuse_All = vDiffuseColor;
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
        }
    }
    
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;
    

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

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha   = saturate(1.0 - g_fDissolveWeight / g_fDissolveTotalTime + vDissoveTexture.r);
	
    // ÇÈ¼¿ »ý·«
    if (fDissolveAlpha < 0.3)
        discard;
	
    else
    {
        vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
        if (vDiffuseColor.a < g_fAlpha_Discard ||
				vDiffuseColor.r < g_fBlack_Discard.r && vDiffuseColor.g < g_fBlack_Discard.g && vDiffuseColor.b < g_fBlack_Discard.b)
            discard;

        vDiffuseColor.rgb = saturate((vDiffuseColor.rgb + g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor.rgb));
        vDiffuseColor.a = g_EffectDesc[In.iInstanceID].g_fAlpha;
        if (Out.vDiffuse_All.a <= g_fAlpha_Discard)
            discard;
        
        Out.vDiffuse_None   = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Low    = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_High   = float4(0.f, 0.f, 0.f, 0.f);
		
		// ÇÈ¼¿ »ö»ó ÁöÁ¤ : ¸í¾Ï ¿¬»ê X
        if (fDissolveAlpha < 0.5)
        {
            Out.vDiffuse_All = g_vDissolveColor;
            if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
                Out.vDiffuse_None = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
                Out.vDiffuse_Low = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
                Out.vDiffuse_Middle = g_vDissolveColor;
            else
                Out.vDiffuse_High = g_vDissolveColor;
			
			// Bloom
            Out.vBloom = float4(g_vDissolveColor.r, g_vDissolveColor.g, g_vDissolveColor.b, 0.5f);
        }
		
	    // ±âº» ÇÈ¼¿ ·»´õ¸µ
        else
        {
            Out.vDiffuse_All = vDiffuseColor;

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
        }
    }
    
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;

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

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = saturate(1.0 - g_fDissolveWeight / g_fDissolveTotalTime + vDissoveTexture.r);
	
	// ÇÈ¼¿ »ý·«
    if (fDissolveAlpha < 0.3)
        discard;
    
	else
    {
        vector vTextureAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);
        if ((vTextureAlpha.r <= g_fBlack_Discard.x) && (vTextureAlpha.g <= g_fBlack_Discard.y) && (vTextureAlpha.b <= g_fBlack_Discard.z))
            discard;

        vector vDiffuseColor = vector(g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor.rgb, g_EffectDesc[In.iInstanceID].g_fAlpha);
        if (vDiffuseColor.a <= g_fAlpha_Discard)
            discard;
        
        Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);
		
	    // ÇÈ¼¿ »ö»ó ÁöÁ¤ : ¸í¾Ï ¿¬»ê X
        if (fDissolveAlpha < 0.5)
        {
            Out.vDiffuse_All = g_vDissolveColor;
            if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
                Out.vDiffuse_None = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
                Out.vDiffuse_Low = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
                Out.vDiffuse_Middle = g_vDissolveColor;
            else
                Out.vDiffuse_High = g_vDissolveColor;
			
			// Bloom
            Out.vBloom = float4(g_vDissolveColor.r, g_vDissolveColor.g, g_vDissolveColor.b, 0.5f);
        }
		
	    // ±âº» ÇÈ¼¿ ·»´õ¸µ
        else
        {
            Out.vDiffuse_All = vDiffuseColor;
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
        }
    }
    
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;
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

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = saturate(1.0 - g_fDissolveWeight / g_fDissolveTotalTime + vDissoveTexture.r);
	
    // ÇÈ¼¿ »ý·«
    if (fDissolveAlpha < 0.3)
        discard;
    else
    {
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
        
        Out.vDiffuse_None   = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Low    = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_High   = float4(0.f, 0.f, 0.f, 0.f);
		
		// ÇÈ¼¿ »ö»ó ÁöÁ¤ : ¸í¾Ï ¿¬»ê X
        if (fDissolveAlpha < 0.5)
        {
            Out.vDiffuse_All = g_vDissolveColor;
            if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
                Out.vDiffuse_None = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
                Out.vDiffuse_Low = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
                Out.vDiffuse_Middle = g_vDissolveColor;
            else
                Out.vDiffuse_High = g_vDissolveColor;
			
			// Bloom
            Out.vBloom = float4(g_vDissolveColor.r, g_vDissolveColor.g, g_vDissolveColor.b, 0.5f);
        }
		
		// ±âº» ÇÈ¼¿ ·»´õ¸µ
        else
        {
            Out.vDiffuse_All = vDiffuseColor;
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
        }
    }
    
    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;
	return Out;

};

PS_OUT PS_RIM(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    if (0 < g_EffectDesc[In.iInstanceID].g_iUVLoop)
    {
        if ((In.vTexUV.x > 1.f) || (In.vTexUV.y > 1.f) || (In.vTexUV.x < 0.f) || (In.vTexUV.y < 0.f))
            discard;
    }

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = saturate(1.0 - g_fDissolveWeight / g_fDissolveTotalTime + vDissoveTexture.r);
	
    // ÇÈ¼¿ »ý·«
    if (fDissolveAlpha < 0.3)
        discard;
    else
    {
        Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
        Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);
		
		// ÇÈ¼¿ »ö»ó ÁöÁ¤ : ¸í¾Ï ¿¬»ê X
        if (fDissolveAlpha < 0.5)
        {
            Out.vDiffuse_All = g_vDissolveColor;
            if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
                Out.vDiffuse_None = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
                Out.vDiffuse_Low = g_vDissolveColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
                Out.vDiffuse_Middle = g_vDissolveColor;
            else
                Out.vDiffuse_High = g_vDissolveColor;
			
			// Bloom
            Out.vBloom = float4(g_vDissolveColor.r, g_vDissolveColor.g, g_vDissolveColor.b, 0.5f);
        }
		
		// ±âº» ÇÈ¼¿ ·»´õ¸µ
        else
        {
            float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
            fRimPower = pow(fRimPower, 1.f);
            
            vector vRimColor = g_EffectDesc[In.iInstanceID].g_fAdditiveDiffuseColor * fRimPower;

            Out.vDiffuse_All = vRimColor;
            Out.vDiffuse_None   = float4(0.f, 0.f, 0.f, 0.f);
            Out.vDiffuse_Low    = float4(0.f, 0.f, 0.f, 0.f);
            Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
            Out.vDiffuse_High   = float4(0.f, 0.f, 0.f, 0.f);
            
            if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.0f)
                Out.vDiffuse_None = vRimColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.0f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.3f)
                Out.vDiffuse_Low = vRimColor;
            else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.3f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.7f)
                Out.vDiffuse_Middle = vRimColor;
            else
                Out.vDiffuse_High = vRimColor;

	        // Bloom
            Out.vBloom = Caculation_Brightness(vRimColor, In.iInstanceID);
        }
    }

    Out.vDistortion.xy = g_DistortionTexture.Sample(LinearSampler, In.vTexUV).xy * g_vDistortion.xy;
    
    return Out;
};

technique11 DefaultTechnique
{
// 0
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

//1
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

//2
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

//3
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

//4
    pass RimPass
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RIM();
    }

//5
    pass DefaultPass_NoDepth
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DEFAULT();
    }

//6
    pass NoAlphaPass_NoDepth
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NO_ALPHA_WITH_DIFFUSE();
    }

//7
    pass NoDiffusePass_NoDepth
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NO_DIFFUSE_WITH_ALPHA();
    }

//8
    pass BothPass_NoDepth
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BOTH();
    }
}