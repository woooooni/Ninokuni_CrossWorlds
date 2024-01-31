
#include "Engine_Shader_Defines.hpp"
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, ReflectionMatrix;
matrix g_WorldInv;

Texture2D	g_DiffuseTexture;
Texture2D	g_NormalTexture;
Texture2D	g_MaskMap;
Texture2D	g_MaskMap2;
Texture2D   g_DissolveTexture;

float4		g_vCamPosition;
float		g_fDissolveWeight;

float4 g_vLightDir = float4(1.0f, -1.0f, 1.0f, 0.0f);
float4 g_vLightDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 g_vLightAmbient = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 g_vLightSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);

float4 g_vMtrlAmbient = float4(0.4f, 0.4f, 0.4f, 0.4f);
float4 g_vMtrlSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);

// ´«µ¢ÀÌ ±¼¸®±â..
matrix g_SnowRotationMatrix;

float3 g_vBloomPower;
float4 g_vRimColor = { 0.f, 0.f, 0.f, 0.f };

// ÇÇÅ·
float4 g_vObjectHash;

cbuffer InversTransposeMatBuffer
{
    matrix WorldInvTransposeView;
};

float4 Caculation_Brightness(float4 vColor)
{
    float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

    float fPixelBrightness = dot(vColor.rgb, g_vBloomPower.rgb);
    if (fPixelBrightness > 0.99f)
        vBrightnessColor = float4(vColor.rgb, 1.0f);

    return vBrightnessColor;
}

cbuffer WaterOption
{
    float fWaterTranslationSpeed;
    float fWaterTime;
    float fReflectRefractScale;
	
    float fTiling = 5.0f;
    float fStrength = 2.0f;
    float2 flowDirection;
	
    bool bFresnel = false;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorldPosition : TEXCOORD2;
    
    float3      vViewNormal : NORMAL1;
    float3      vPositionView : POSITION;
};

struct WaterVertexToPixel
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
    
    float4 vRefractionPos : TEXCOORD3;
    float4 vReflectionPos : TEXCOORD4;
    //float4 vReflectionMapSamplingPos : TEXCOORD1;
    //float2 vBumpMapSamplingPos : TEXCOORD2;
    //float4 vRefractionMapSamplingPos : TEXCOORD3;
    //float4 vWorldPos : TEXCOORD4;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

	// SSAO
    Out.vViewNormal = mul(Out.vNormal.xyz, (float3x3) g_ViewMatrix);
    Out.vPositionView = mul(float4(In.vPosition, 1.0f), matWV);
    
    return Out;
}

VS_OUT VS_REFLECT(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldInv)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

		// SSAO
    Out.vViewNormal = mul(Out.vNormal.xyz, (float3x3) g_ViewMatrix);
    Out.vPositionView = mul(float4(In.vPosition, 1.0f), matWV);
    
    return Out;
}

VS_OUT VS_SNOWBALL(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
	
    	// SSAO
    Out.vViewNormal = mul(Out.vNormal.xyz, (float3x3) g_ViewMatrix);
    Out.vPositionView = mul(float4(In.vPosition, 1.0f), matWV);
    
    return Out;
}

WaterVertexToPixel WaterVertexShader(VS_IN input)
{
    WaterVertexToPixel output = (WaterVertexToPixel) 0;
    
    matrix matWV, matWVP;
    matrix matReflectPW;
    matrix matVPWorld;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    matReflectPW = mul(ReflectionMatrix, g_ProjMatrix);
    matReflectPW = mul(g_WorldMatrix, matReflectPW);
    output.vReflectionPos = mul(float4(input.vPosition, 1.0f), matReflectPW);
    
    matVPWorld = mul(g_ViewMatrix, g_ProjMatrix);
    matVPWorld = mul(g_WorldMatrix, matVPWorld);
    output.vRefractionPos = mul(float4(input.vPosition, 1.0f), matVPWorld);
    
	// 
    output.vPosition = mul(float4(input.vPosition, 1.f), matWVP);
    output.vWorldPosition = mul(float4(input.vPosition, 1.f), g_WorldMatrix);
    output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_WorldMatrix)).xyz;
    output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_WorldMatrix)).xyz;
    output.vBinormal = normalize(cross(output.vNormal, output.vTangent));
    output.vTexUV = input.vTexUV;
    output.vProjPos = output.vPosition;
    
    return output;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorldPosition : TEXCOORD2;
    
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
    float4      vBloom : SV_TARGET3;
    float4      vViewNormal : SV_TARGET4;
};

struct PS_OUT_SHADOW_DEPTH
{
	float4		vDepth : SV_TARGET0;
};

struct WaterPixelToFrame
{
    float4 vColor : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vBloom : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = (vector)1.f;


	Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    
  
    // SSAO
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
    
	if (0.3 >= Out.vDiffuse.a)
		discard;
	
	

	return Out;	
}

PS_OUT PS_SKY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(1.f, 1.f, 0.f, 0.f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (0.f == Out.vDiffuse.a)
		discard;

	return Out;

}

PS_OUT PS_MAIN_NORMAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	vector		vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;
	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);

	if (0 == Out.vDiffuse.a)
		discard;

	return Out;
}

PS_OUT PS_WEAPON(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = (vector) 1.f;


    Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (0.3 >= Out.vDiffuse.a)
        discard;
	
    return Out;
}


PS_OUT PS_SNOWBALL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = (vector) 1.f;


    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (0.3 >= Out.vDiffuse.a)
        discard;
	
	

    return Out;
}


WaterPixelToFrame WaterPS(WaterVertexToPixel input)
{
    WaterPixelToFrame output = (WaterPixelToFrame) 0;

    float2 vReflectTexCoord;
    float2 vRefractTexCoord;
    float4 vNormalMap;
    float4 vNormalMap2;
    float3 vNormal;
    float4 vReflectionColor;
    float4 vRefractionColor;
	
	// TODO 
    float2 vFlowDir = normalize(flowDirection);
    float2 flow = vFlowDir * fWaterTime * fWaterTranslationSpeed;
	
    vNormalMap = g_NormalTexture.Sample(LinearSampler, input.vTexUV * fTiling + flow) * fStrength;
    vNormalMap2 = g_NormalTexture.Sample(LinearSampler, input.vTexUV * fTiling - flow * 0.3f) * fStrength * 0.5f;
    vNormal = (vNormalMap + vNormalMap2) * 0.5f;
	
    float4 vMtrlDiffuse = float4(0.0f, 144.0f / 255.0f, 233.0f / 255.0f, 0.85f);
    vNormal = (vNormalMap.xyz * 2.0f) - 1.0f;
	
    float4 vMaskMap = g_MaskMap.Sample(LinearSampler, input.vTexUV * fTiling + flow) * fStrength;
    float4 vMaskMap2 = g_MaskMap.Sample(LinearSampler, input.vTexUV * fTiling - flow * 0.3f) * fStrength * 0.5f;
	
    float4 vFinalMask = (vMaskMap + vMaskMap2) * 0.5f;
	
    if (vFinalMask.r >= 0.5f && vFinalMask.g >= 0.5f && vFinalMask.b >= 0.5f)
        vMtrlDiffuse.rgb += 0.3f;

    float4 vMaskMap3 = g_MaskMap2.Sample(LinearSampler, input.vTexUV * 12.0f + flow) * 1.05f;
	
    if (vMaskMap3.r >= 0.99999999f && vMaskMap3.g >= 0.99999999f && vMaskMap3.b >= 0.99999999f)
        output.vBloom = float4(1.0f, 1.0f, 1.0f,0.5f);
	else
        output.vBloom = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    float3x3 WorldMatrix = float3x3(input.vTangent, input.vBinormal, input.vNormal.xyz);
	
    vNormal = normalize(mul(vNormal, WorldMatrix));
	
	

    //vReflectTexCoord.x = input.vReflectionPos.x / input.vReflectionPos.w / 2.0f + 0.5f;
    //vReflectTexCoord.y = -input.vReflectionPos.y / input.vReflectionPos.w / 2.0f + 0.5f;

    //vRefractTexCoord.x = input.vRefractionPos.x / input.vRefractionPos.w / 2.0f + 0.5f;
    //vRefractTexCoord.y = -input.vRefractionPos.y / input.vRefractionPos.w / 2.0f + 0.5f;
    
    //vReflectTexCoord = vReflectTexCoord + (vNormal.xy * fReflectRefractScale);
    //vRefractTexCoord = vRefractTexCoord + (vNormal.xy * fReflectRefractScale);

    //vReflectionColor = ReflectionMap.Sample(LinearSampler, vReflectTexCoord);
    //vRefractionColor = RefractionMap.Sample(LinearSampler, vRefractTexCoord);
    
    //float fFresnelTerm = 0.0f;
    //float4 vLook = input.vProjPos - vCameraPosition;
    //vLook = normalize(vLook);
    vMtrlDiffuse.a = 0.7f;
    output.vColor = vMtrlDiffuse;

    //if(true == bFresnel)
    //{
    //    fFresnelTerm = 0.02f + 0.97f * pow((1 - dot(vLook, float4(vNormal, 1.0f))), 5.0f);
    //    float4 vCombinedColor = vRefractionColor * (1 - fFresnelTerm) *
    //        vRefractionColor.a * vReflectionColor.a + vReflectionColor * fFresnelTerm * vReflectionColor.a * vRefractionColor.a;
    //    output.vColor = vCombinedColor;
    //}
    //else
    //    output.vColor = lerp(vReflectionColor, vRefractionColor, 0.5f);
    
    output.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    output.vDepth = float4(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / 1000.f, 1.0f, 0.0f);
	
    return output;
}

PS_OUT PS_DISSOVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
	
    vector vDissoveSample = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fInterval = saturate(vDissoveSample.r - g_fDissolveWeight);

    Out.vDiffuse = (vector) 1.f;
	
	if(0.f == fInterval)
        discard;
    else if (fInterval >= 0.1f && fInterval >= 0.15f)
        Out.vDiffuse = vector(1.f, 0.98f, 0.812f, 1.f);
    else if (fInterval >= 0.2f && fInterval >= 0.25f)
        Out.vDiffuse = vector(1.f, 0.957f, 0.569f, 1.f);
	else
        Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
	
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
    return Out;
}




// ±×¸²ÀÚ ÇÈ¼¿ ½¦ÀÌ´õ
PS_OUT_SHADOW_DEPTH PS_SHADOW_DEPTH(PS_IN In)
{
	PS_OUT_SHADOW_DEPTH		Out = (PS_OUT_SHADOW_DEPTH)0;


	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);;
	if (vColor.a <= 0.f)
		discard;


	Out.vDepth = vector(In.vProjPos.w / 1000.0f, 0.f, 0.f, 1.f);

	return Out;
}

PS_OUT PS_MAIN_REFLECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    // ºû ¿¬»ê 
    
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
    
    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    vector vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f) +
		g_vLightAmbient * g_vMtrlAmbient;

    vector vReflect = reflect(normalize(g_vLightDir), normalize(float4(In.vNormal, 0.f)));
    vector vLook = In.vPosition - g_vCamPosition;

   // float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);
    float fSpecular = 0.0f;
    
    vShade = (ceil(vShade * 2.f) / 2.f);

    Out.vDiffuse = (g_vLightDiffuse * vMtrlDiffuse) * saturate(vShade) +
		(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

    //Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);

    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_vRimColor * fRimPower;
    Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
    //if (0.f == Out.vDiffuse.a)
    //    discard;

    return Out;
}

struct PS_OUT_MINIMAP
{
    float4      vMinimap : SV_TARGET5;
};

PS_OUT_MINIMAP PS_MAIN_MINIMAP(PS_IN In)
{
    PS_OUT_MINIMAP		Out = (PS_OUT_MINIMAP)0;

    Out.vMinimap= g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

 //   if (0.3 >= Out.vDiffuse.a)
 //       discard;

    return Out;
}

struct PS_OUT_PICKING
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_PICKING PS_PICKING(PS_IN In)
{
    PS_OUT_PICKING Out = (PS_OUT_PICKING) 0;
    Out.vColor = g_vObjectHash;
    
    return Out;
}



RasterizerState CullClockWiseRS
{
    AntialiasedLineEnable = false;
    CullMode = BACK;
    DepthBias = 0;
    DepthBiasClamp = 0.0f;
    DepthClipEnable = true;
    FillMode = SOLID;
    FrontCounterClockwise = true;
    MultisampleEnable = false;
    ScissorEnable = false;
    SlopeScaledDepthBias = 0.0f;
};

DepthStencilState DrawReflectionDSS
{
    DepthEnable = true;
    DepthWriteMask = ALL; // Zero or ALL
    DepthFunc = LESS; // LESS or ALWAYS
    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;

    // Front
    FrontFaceStencilFail = KEEP;
    FrontFaceStencilDepthFail = KEEP;
    FrontFaceStencilPass = KEEP;
    FrontFaceStencilFunc = EQUAL;

    //Back
    FrontFaceStencilFail = KEEP;
    FrontFaceStencilDepthFail = KEEP;
    FrontFaceStencilPass = KEEP;
    FrontFaceStencilFunc = EQUAL;
};

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}


	pass Sky
	{
		// 2
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_Sky, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SKY();
	}

	pass Water
	{
		// 3
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 WaterVertexShader();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 WaterPS();
    }

	pass Weapon
	{
		// 4
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_WEAPON();
    }

    pass Weapon_Dissove
    {
		// 5
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DISSOVE();
    }

	pass SnowBall
	{
		// 6
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_SNOWBALL();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SNOWBALL();
	}

	pass Reflect
	{
		// 7
        SetRasterizerState(CullClockWiseRS);
        SetDepthStencilState(DrawReflectionDSS, 1);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_REFLECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_REFLECT();
    }

	pass Temp8
	{
		// 8
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UI_Minimap
	{
		// 9
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MINIMAP();
	}

	pass Shadow_Depth
	{
		// 10
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Shadow, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW_DEPTH();
	}
    
    pass Temp11
    {
		// 11
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Temp12
    {
		// 12
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass ObjectPicking
    {
		// 13
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PICKING();
    }



	
}