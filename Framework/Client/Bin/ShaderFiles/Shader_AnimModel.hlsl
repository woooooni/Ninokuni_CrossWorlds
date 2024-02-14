#include "Engine_Shader_Defines.hpp"

#include "Matrix.hlsl"
#include "Quaternion.hlsl"
matrix          g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix          g_WorldInv;

Texture2D       g_DiffuseTexture;
Texture2D       g_NormalTexture;

float4          g_vRimColor = { 0.f, 0.f, 0.f, 0.f };

Texture2D       g_DissolveTexture;
float           g_DissolveDuration;
float           g_fDissolveWeight;
float4          g_vDissolveColor = { 0.6f, 0.039f, 0.039f, 1.f };

float3          g_vBloomPower;
float           g_fMotionTrailAlpha;

float4			g_vCamPosition;
float           g_fBlurPower;

float4 g_vLightDir = float4(1.0f, -1.0f, 1.0f, 0.0f);
float4 g_vLightDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 g_vLightAmbient = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 g_vLightSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);

float4 g_vMtrlAmbient = float4(0.4f, 0.4f, 0.4f, 0.4f);
float4 g_vMtrlSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);

float4 g_vObjectHash;

cbuffer InversTransposeMatBuffer
{
    matrix WorldInvTransposeView;
};
struct KeyframeDesc
{
    int iAnimIndex;
    uint iCurFrame;
    uint iNextFrame;
    float fRatio;
    float fFrameAcc;

    uint iFinish;
    uint iFix;
    uint iStop;
};

struct TweenFrameDesc
{
    KeyframeDesc cur;
    KeyframeDesc next;

    float fTweenDuration;
    float fTweenRatio;
    float fTweenAcc;
    float fPadding;
};

TweenFrameDesc g_TweenFrames;
Texture2DArray g_TransformMap;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION0;
    float4 vNormal : NORMAL0;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vProjPos : TEXCOORD1;
    float4 vWorldPosition : TEXCOORD2;
    
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

float4x4 Create_AffineTransform(float3 trans, float4 quat, float3 scale)
{
    float4x4 translationMatrix =
    {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { trans.x, trans.y, trans.z, 1.0f }
    };

    float4x4 rotationMatrix =
    {
        { 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z), 2.0f * (quat.x * quat.y - quat.w * quat.z), 2.0f * (quat.x * quat.z + quat.w * quat.y), 0.0f },
        { 2.0f * (quat.x * quat.y + quat.w * quat.z), 1.0f - 2.0f * (quat.x * quat.x + quat.z * quat.z), 2.0f * (quat.y * quat.z - quat.w * quat.x), 0.0f },
        { 2.0f * (quat.x * quat.z - quat.w * quat.y), 2.0f * (quat.y * quat.z + quat.w * quat.x), 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y), 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    float4x4 scaleMatrix =
    {
        { scale.x, 0.0f, 0.0f, 0.0f },
        { 0.0f, scale.y, 0.0f, 0.0f },
        { 0.0f, 0.0f, scale.z, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    return mul(scaleMatrix, mul(rotationMatrix, translationMatrix));
}

matrix Create_BoneMatrix_By_Lerp(VS_IN input)
{
    float indices[4] = { input.vBlendIndex.x, input.vBlendIndex.y, input.vBlendIndex.z, input.vBlendIndex.w };
    float weights[4] = { input.vBlendWeight.x, input.vBlendWeight.y, input.vBlendWeight.z, input.vBlendWeight.w };

    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];

	/* cur */
    animIndex[0] = g_TweenFrames.cur.iAnimIndex;
    currFrame[0] = g_TweenFrames.cur.iCurFrame;
    nextFrame[0] = g_TweenFrames.cur.iNextFrame;
    ratio[0] = g_TweenFrames.cur.fRatio;

	/* next */
    animIndex[1] = g_TweenFrames.next.iAnimIndex;
    currFrame[1] = g_TweenFrames.next.iCurFrame;
    nextFrame[1] = g_TweenFrames.next.iNextFrame;
    ratio[1] = g_TweenFrames.next.fRatio;

    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    float4x4 curr = 0;
    float4x4 next = 0;
    
    float4x4 transform = 0;

    for (int i = 0; i < 4; i++)
    {
		/* cur */
        c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0)) * weights[i];
        c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0)) * weights[i];
        c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0)) * weights[i];
        c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0)) * weights[i];
        curr = float4x4(c0, c1, c2, c3);

        n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0)) * weights[i];
        n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0)) * weights[i];
        n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0)) * weights[i];
        n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0)) * weights[i];
        next = float4x4(n0, n1, n2, n3);

        float4x4 result = lerp(curr, next, ratio[0]);
		
		/* if next */
        if (0 <= animIndex[1])
        {
            c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0)) * weights[i];
            c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0)) * weights[i];
            c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0)) * weights[i];
            c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0)) * weights[i];
            curr = float4x4(c0, c1, c2, c3);

            n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0)) * weights[i];
            n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0)) * weights[i];
            n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0)) * weights[i];
            n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0)) * weights[i];
            next = float4x4(n0, n1, n2, n3);
            
            // Test         
            {
                //matrix nextResult = lerp(curr, next, ratio[1]);
                //
                //float3 fCurAnimScale, fNextAnimScale, fBlendingScale;
                //float4 fCurAnimQuat, fNextAnimQuat, fBlendingQuat;
                //float3 fCurAnimTrans, fNextAnimTrans, fBlendingTrans;
                //
                //
                //decompose(result, fCurAnimTrans, fCurAnimQuat, fCurAnimScale);
                //decompose(nextResult, fNextAnimTrans, fNextAnimQuat, fNextAnimScale);
                //
                //fBlendingTrans = lerp(fCurAnimTrans, fNextAnimTrans, g_TweenFrames.fTweenRatio);
                //fBlendingQuat = q_slerp(fCurAnimQuat, fNextAnimQuat, g_TweenFrames.fTweenRatio);
                //fBlendingScale = lerp(fCurAnimScale, fNextAnimScale, g_TweenFrames.fTweenRatio);
                //
                //result = Create_AffineTransform(fBlendingTrans, fBlendingQuat, fBlendingScale);
            }
            
            // Origin 
            {
               matrix nextResult = lerp(curr, next, ratio[1]);
                         
               result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
            }
            
        }
        transform += result;
    }

    return transform;
}

matrix Create_BoneMatrix_By_Affine(VS_IN input)
{
    float indices[4] = { input.vBlendIndex.x, input.vBlendIndex.y, input.vBlendIndex.z, input.vBlendIndex.w };
    float weights[4] = { input.vBlendWeight.x, input.vBlendWeight.y, input.vBlendWeight.z, input.vBlendWeight.w };

    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];

	/* cur */
    animIndex[0] = g_TweenFrames.cur.iAnimIndex;
    currFrame[0] = g_TweenFrames.cur.iCurFrame;
    nextFrame[0] = g_TweenFrames.cur.iNextFrame;
    ratio[0] = g_TweenFrames.cur.fRatio;

	/* next */
    animIndex[1] = g_TweenFrames.next.iAnimIndex;
    currFrame[1] = g_TweenFrames.next.iCurFrame;
    nextFrame[1] = g_TweenFrames.next.iNextFrame;
    ratio[1] = g_TweenFrames.next.fRatio;

    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix transform = 0;

    for (int i = 0; i < 4; i++)
    {
        float3 fCurAnimSourceScale, fCurAnimDestScale, fCurAnimLerpScale;
        float4 fCurAnimSourceRot, fCurAnimDestRot, fCurAnimLerpRot;
        float3 fCurAnimSourcePos, fCurAnimDestPos, fCurAnimLerpPos;
		
        float4x4 fCurAnimCurFrame = 0;
        float4x4 fCurAnimNextFrame = 0;
		
        matrix result;
		
		/* curr Frame */
		{
            c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
            c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
            c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
            c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
		
            fCurAnimCurFrame = float4x4(c0, c1, c2, c3);
            decompose(fCurAnimCurFrame, fCurAnimSourcePos, fCurAnimSourceRot, fCurAnimSourceScale);
        }

		/* next Frame */ 
		{
            n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
            n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
            n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
            n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
		
            fCurAnimNextFrame = float4x4(n0, n1, n2, n3);
            decompose(fCurAnimNextFrame, fCurAnimDestPos, fCurAnimDestRot, fCurAnimDestScale);
        }
		
		/* Frame Lerp */
		{
            fCurAnimLerpScale = lerp(fCurAnimSourceScale, fCurAnimDestScale, ratio[0]);
            fCurAnimLerpRot = q_slerp(fCurAnimSourceRot, fCurAnimDestRot, ratio[0]);
            fCurAnimLerpPos = lerp(fCurAnimSourcePos, fCurAnimDestPos, ratio[0]);
        }
        
		/* if next */
        if (0 <= animIndex[1])
        {
            float3 fNextAnimSourceScale, fNextAnimDestScale, fNextAnimLerpScale;
            float4 fNextAnimSourceRot, fNextAnimDestRot, fNextAnimLerpRot;
            float3 fNextAnimSourcePos, fNextAnimDestPos, fNextAnimLerpPos;
			
            float4x4 fNextAnimCurFrame = 0;
            float4x4 fNextAnimNextFrame = 0;
			
			/* curr Frame */
			{
                c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
                c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
                c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
                c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
			
                fNextAnimCurFrame = float4x4(c0, c1, c2, c3);
                decompose(fNextAnimCurFrame, fNextAnimSourcePos, fNextAnimSourceRot, fNextAnimSourceScale);
            }
			
			/* next Frame */ 
			{
                n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
                n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
                n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
                n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));

                fNextAnimNextFrame = float4x4(n0, n1, n2, n3);
                decompose(fNextAnimNextFrame, fNextAnimDestPos, fNextAnimDestRot, fNextAnimDestScale);
            }
			
			/* Frame Lerp */			
			{
                fNextAnimLerpScale = lerp(fNextAnimSourceScale, fNextAnimDestScale, ratio[1]);
                fNextAnimLerpRot = q_slerp(fNextAnimSourceRot, fNextAnimDestRot, ratio[1]);
                fNextAnimLerpPos = lerp(fNextAnimSourcePos, fNextAnimDestPos, ratio[1]);
            }
			
			/* Tween Lerp */			
			{
                float3 fTweenScale = lerp(fCurAnimLerpScale, fNextAnimLerpScale, g_TweenFrames.fTweenRatio);
                float4 fTweenRot = q_slerp(fCurAnimLerpRot, fNextAnimLerpRot, g_TweenFrames.fTweenRatio);
                float3 fTweenPos = lerp(fCurAnimLerpPos, fNextAnimLerpPos, g_TweenFrames.fTweenRatio);
            
                //result = compose(fTweenPos, fTweenRot, fTweenScale);
                result = Create_AffineTransform(fTweenPos, fTweenRot, fTweenScale);
            }
        }
        else
        {
            //result = compose(fCurAnimLerpPos, fCurAnimLerpRot, fCurAnimLerpScale);
            result = Create_AffineTransform(fCurAnimLerpPos, fCurAnimLerpRot, fCurAnimLerpScale);
        }

        transform += mul(result, weights[i]);
    }

    return transform;
}

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    float4x4 BoneMatrix = Create_BoneMatrix_By_Lerp(In);
    //float4x4 BoneMatrix = Create_BoneMatrix_By_Affine(In);

    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    vector vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vWorldPosition = mul(vPosition, g_WorldMatrix);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    Out.vTangent = normalize(mul(vTangent, g_WorldMatrix)).xyz;
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

    float4x4 BoneMatrix = Create_BoneMatrix_By_Lerp(In);
    //float4x4 BoneMatrix = Create_BoneMatrix_By_Affine(In);

    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    vector vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vWorldPosition = mul(vPosition, g_WorldMatrix);
    Out.vNormal = normalize(mul(vNormal, g_WorldInv));
    Out.vTangent = normalize(mul(vTangent, g_WorldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent));
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL0;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vProjPos : TEXCOORD1;
    float4 vWorldPosition : TEXCOORD2;
    
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

struct PS_OUT_UI
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vBloom : SV_TARGET3;
};

struct PS_OUT_MINIMAP
{
    float4		vMinimap : SV_TARGET5;
};

float4 Caculation_Brightness(float4 vColor)
{
    float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

    float fPixelBrightness = dot(vColor.rgb, g_vBloomPower.rgb);
    if (fPixelBrightness > 0.99f)
        vBrightnessColor = float4(vColor.rgb, 1.0f);

    return vBrightnessColor;
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
    


    Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);

    float fRimPower = 1.f - saturate(dot(In.vNormal, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (0.f == Out.vDiffuse.a)
        discard;

    return Out;
}

PS_OUT PS_PREVIEW_POSSIBLE_TOWER(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    


    Out.vDiffuse = vector(0.f, 1.f, 0.f, 1.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);

    float fRimPower = 1.f - saturate(dot(In.vNormal, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_vRimColor * fRimPower;
    Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (0.f == Out.vDiffuse.a)
        discard;

    return Out;
}

PS_OUT PS_PREVIEW_IMPOSSIBLE_TOWER(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vDiffuse = vector(1.f, 0.f, 0.f, 1.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);

    float fRimPower = 1.f - saturate(dot(In.vNormal, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_vRimColor * fRimPower;
    Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (0.f == Out.vDiffuse.a)
        discard;

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

    vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    vector vLook = In. vPosition - g_vCamPosition;

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


PS_OUT_UI PS_MAIN_UI(PS_IN In)
{
    PS_OUT_UI Out = (PS_OUT_UI) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);

    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_vRimColor * fRimPower;
    Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;

    if (0.f == Out.vDiffuse.a)
        discard;

    return Out;
}

PS_OUT_MINIMAP PS_MAIN_MINIMAP(PS_IN In)
{
    PS_OUT_MINIMAP Out = (PS_OUT_MINIMAP)0;

    vector vColor = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
    if (vColor.a <= 0.01f)
        discard;

    Out.vMinimap = vColor;

    return Out;
}

PS_OUT PS_MAIN_NORMAL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);

	vector vTextureNormal = g_NormalTexture.Sample(ModelSampler, In.vTexUV);

	float3 vNormal = vTextureNormal.xyz * 2.f - 1.f;
	float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);

    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
    
    
    if (0.f == Out.vDiffuse.a)
        discard;

    return Out;
}



PS_OUT PS_DISSOLVE_DEAD(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float  fDissolveAlpha  = saturate(1.0 - g_fDissolveWeight / g_DissolveDuration + vDissoveTexture.r);
	
	// ÇÈ¼¿ »ý·«
    if (fDissolveAlpha < 0.3)
        discard;
	
	// ÇÈ¼¿ »ö»ó ÁöÁ¤ : ¸í¾Ï ¿¬»ê X
    else if (fDissolveAlpha < 0.5)
    {
        Out.vDiffuse = g_vDissolveColor;
        Out.vNormal  = float4(1.f, 1.f, 1.f, 0.f);
        Out.vBloom   = float4(Out.vDiffuse.r, Out.vDiffuse.g, Out.vDiffuse.b, 0.5f);
    }
	// ±âº» ÇÈ¼¿ ·»´õ¸µ
    else
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
        Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
        Out.vBloom   = Caculation_Brightness(Out.vDiffuse);
    }
    
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
    
    
    if (0.f == Out.vDiffuse.a)
        discard;
	
	return Out;
}

struct PS_OUT_MOTIONTRAIL
{
    float4 vDiffuse_All : SV_TARGET0;
    float4 vDiffuse_None : SV_TARGET1;
    float4 vDiffuse_Low : SV_TARGET2;
    float4 vDiffuse_Middle : SV_TARGET3;
    float4 vDiffuse_High : SV_TARGET4;
    float4 vBloom : SV_TARGET5;
};

PS_OUT_MOTIONTRAIL PS_MOTION_TRAIL(PS_IN In)
{
    PS_OUT_MOTIONTRAIL Out = (PS_OUT_MOTIONTRAIL) 0;
    
    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 1.f);
	
    vector vRimColor = g_vRimColor * fRimPower;
    vRimColor.a = g_fMotionTrailAlpha;
    
    Out.vDiffuse_All = vRimColor;
    Out.vDiffuse_None = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Low = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_High = float4(0.f, 0.f, 0.f, 0.f);
    
    if (g_fBlurPower <= 0.0f)
        Out.vDiffuse_None = vRimColor;
    else if (g_fBlurPower > 0.0f && g_fBlurPower <= 0.3f)
        Out.vDiffuse_Low = vRimColor;
    else if (g_fBlurPower > 0.3f && g_fBlurPower <= 0.7f)
        Out.vDiffuse_Middle = vRimColor;
    else
        Out.vDiffuse_High = vRimColor;
    
    // Bloom
    Out.vBloom = Caculation_Brightness(vRimColor);
    
    
	
    return Out;
}

// ±×¸²ÀÚ ÇÈ¼¿ ½¦ÀÌ´õ.
struct PS_OUT_SHADOW_DEPTH
{
    float4 vDepth : SV_TARGET0;
};


PS_OUT_SHADOW_DEPTH PS_SHADOW_DEPTH(PS_IN In)
{
    PS_OUT_SHADOW_DEPTH Out = (PS_OUT_SHADOW_DEPTH) 0;

    vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    if (vColor.a <= 0.01f)
        discard;


    Out.vDepth = vector(In.vProjPos.w / 1000.0f, 0.f, 0.f, 1.f);

    return Out;
}


struct VS_CASCADE_OUT
{
    float4 vPosition : SV_POSITION;
};

struct PS_OUT_PICKING
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_PICKING PS_PICKING(PS_IN In)
{
    PS_OUT_PICKING Out = (PS_OUT_PICKING)0;
    Out.vColor = g_vObjectHash;
    
    return Out;
}

// Geom
cbuffer LightTransform
{
    matrix CascadeViewProj[3];
    matrix WorldViewProjMatrix;
};

struct GS_IN
{
    float4 vPosition : SV_POSITION;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

VS_CASCADE_OUT VS_CASCADE(VS_IN input)
{
    VS_CASCADE_OUT Out = (VS_CASCADE_OUT) 0;
    float4x4 BoneMatrix = Create_BoneMatrix_By_Lerp(input);

    vector vPosition = mul(vector(input.vPosition, 1.f), BoneMatrix);
    
    //Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.vPosition = vPosition;
    
    return Out;
}

[maxvertexcount(9)]
void GS_CASCADE(triangle GS_IN input[3], inout TriangleStream<GS_OUT> output)
{
    for (int face = 0; face < 3; ++face)
    {
        GS_OUT element = (GS_OUT) 0;
        element.RTIndex = face;
        for (int i = 0; i < 3; ++i)
        {
            element.vPosition = mul(input[i].vPosition, CascadeViewProj[face]);
            output.Append(element);
        }
        output.RestartStrip();
    }
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

RasterizerState RS_WireFrame
{
    FillMode = WireFrame;
    CullMode = Back;
    FrontCounterClockwise = false;
};

RasterizerState RS_CascadeGen
{
    FillMode = SOLID;
    CullMode = FRONT;
    FrontCounterClockwise = false;
    DepthBias = 0;
    DepthBiasClamp = 0.0f;
    SlopeScaledDepthBias = 0.0f;
    DepthClipEnable = true;
    ScissorEnable = false;
    MultisampleEnable = false;
    AntialiasedLineEnable = false;
};

DepthStencilState DSS_ShadowMapGen
{
    DepthEnable = true;
    DepthWriteMask = ALL;
    DepthFunc = LESS;
    StencilEnable = false;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;
    FrontFaceStencilFail = KEEP;
    FrontFaceStencilDepthFail = KEEP;
    FrontFaceStencilPass = KEEP;
    FrontFaceStencilFunc = EQUAL;

    BackFaceStencilFail = KEEP;
    BackFaceStencilDepthFail = KEEP;
    BackFaceStencilPass = KEEP;
    BackFaceStencilFunc = EQUAL;
};

technique11 DefaultTechnique
{
    pass DefaultPass
    {
		// 0
        SetRasterizerState(RS_Default);
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
		// 1
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
    }

    pass DissolveDead
    {
		// 2
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DISSOLVE_DEAD();
    }


    pass MotionTrail
    {
		// 3
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MOTION_TRAIL();
    }

    pass ReflectPass // ¹Ì·¯ÇÒ ¶§ ÀÌ ·»´õ »ç¿ë.
    {
		// 4
        SetRasterizerState(CullClockWiseRS);
        SetDepthStencilState(DrawReflectionDSS, 1);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_REFLECT();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_REFLECT();
    }

    pass Preview_Tower_Possible
    {
		// 5
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PREVIEW_POSSIBLE_TOWER();
    }

    pass Preview_Tower_Impossible
    {
		// 6
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PREVIEW_IMPOSSIBLE_TOWER();
    }

    pass Temp4
    {
		// 7
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Temp5
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

    pass UI_Dummy_NonDepth
    {
        // 11
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UI();
    }

    pass CascadeShadow_Depth
    {
        // 12
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Shadow, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_CASCADE();
        GeometryShader = compile gs_5_0 GS_CASCADE();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = NULL;
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
};