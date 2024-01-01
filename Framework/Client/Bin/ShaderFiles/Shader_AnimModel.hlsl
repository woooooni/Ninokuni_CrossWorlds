#include "Engine_Shader_Defines.hpp"

#include "Matrix.hlsl"
#include "Quaternion.hlsl"
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;

float4          g_vRimColor = { 0.f, 0.f, 0.f, 0.f };

Texture2D       g_DissolveTexture;
float           g_DissolveDuration;
float           g_fDissolveWeight;
float4          g_vDissolveColor = { 0.6f, 0.039f, 0.039f, 1.f };

float3          g_vBloomPower;

float4			g_vCamPosition;


struct KeyframeDesc
{
	int iAnimIndex;
	uint iCurFrame;
	uint iNextFrame;
	float fRatio;
	float fFrameAcc;

	uint	iFinish;
	uint	iFix;
	uint	iStop;
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

TweenFrameDesc  g_TweenFrames;
Texture2DArray  g_TransformMap;


struct VS_IN
{
	float3		vPosition		: POSITION;
	float3		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	float3		vTangent		: TANGENT;
	uint4		vBlendIndex		: BLENDINDEX;
	float4		vBlendWeight	: BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition		: SV_POSITION0;
	float4		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	float3		vTangent		: TANGENT;
	float3		vBinormal		: BINORMAL;
	float4		vProjPos		: TEXCOORD1;
	float4		vWorldPosition	: TEXCOORD2;
};


matrix GetAnimationMatrix(VS_IN input)
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
	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;
	
	for (int i = 0; i < 4; i++)
	{
		/* cur */
		c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
		c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
		c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
		c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
		curr = matrix(c0, c1, c2, c3);


		n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
		n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
		n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
		n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
		next = matrix(n0, n1, n2, n3);

        matrix result = lerp(curr, next, ratio[0]);
		
		///* Tweening */
        if (animIndex[1] >= 0)
        {
            c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
            c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
            c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
            c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
            curr = matrix(c0, c1, c2, c3);
            

            n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);
		
            matrix nextResult = lerp(curr, next, ratio[1]);
            result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
        }

		transform += mul(result, weights[i]);
	}

	return transform;
}

matrix GetAnimationMatrixEdited(VS_IN input)
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
	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;

	for (int i = 0; i < 4; i++)
	{
		float3 fSourceScale, fDestScale, fLerpScale;
		float4 fSourceRot, fDestRot, fLerpRot;
		float3 fSourcePos, fDestPos, fLerpPos;

		/* cur */
		{
			c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0)); /* Scale */
			c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0)); /* Quat */
			c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0)); /* Pos */
			c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0)); /* None */

			fSourceScale.x = c0.x;
			fSourceScale.y = c0.y;
			fSourceScale.z = c0.z;

			fSourceRot.x = c1.x;
			fSourceRot.y = c1.y;
			fSourceRot.z = c1.z;
			fSourceRot.w = c1.w;

			fSourcePos.x = c2.x;
			fSourcePos.y = c2.y;
			fSourcePos.z = c2.z;
		}

		/* next */
		{
			n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
			n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
			n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
			n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
			
			fDestScale.x = n0.x;
			fDestScale.y = n0.y;
			fDestScale.z = n0.z;

			fDestRot.x = n1.x;
			fDestRot.y = n1.y;
			fDestRot.z = n1.z;
			fDestRot.w = n1.w;

			fDestPos.x = n2.x;
			fDestPos.y = n2.y;
			fDestPos.z = n2.z;
		}

		/* Lerp */
		{
			fLerpScale = lerp(fSourceScale, fDestScale, ratio[0]);
            fLerpRot = q_slerp(fSourceRot, fDestRot, ratio[0]);
			fLerpPos = lerp(fSourcePos, fDestPos, ratio[0]);
		}

		/* AffineTransformation */
		{
			matrix scaleMatrix = 0;
			{
				scaleMatrix[0][0] = fLerpScale.x;
				scaleMatrix[1][1] = fLerpScale.y;
				scaleMatrix[2][2] = fLerpScale.z;
				scaleMatrix[3][3] = 1.f;
			}
			matrix translationToOrigin = 0;
			{			
				translationToOrigin[0][0] = 1.f;
				translationToOrigin[1][1] = 1.f;
				translationToOrigin[2][2] = 1.f;
				translationToOrigin[3][3] = 1.f;
			}
			matrix translationBack = translationToOrigin;
			{
				//translationBack[3][0] = 1.f;
				//translationBack[3][1] = 1.f;
				//translationBack[3][2] = 1.f;
				//translationBack[3][3] = 1.f;
			}
			matrix rotationMatrix = 0;
			{
				float x = fLerpRot.x;
				float y = fLerpRot.y;
				float z = fLerpRot.z;
				float w = fLerpRot.w;

				float xx = x * x;
				float yy = y * y;
				float zz = z * z;
				float xy = x * y;
				float xz = x * z;
				float yz = y * z;
				float wx = w * x;
				float wy = w * y;
				float wz = w * z;

				rotationMatrix[0][0] = 1.0f - 2.0f * (yy + zz);
				rotationMatrix[1][0] = 2.0f * (xy + wz);
				rotationMatrix[2][0] = 2.0f * (xz - wy);
				rotationMatrix[3][0] = 0.f;

				rotationMatrix[0][1] = 2.0f * (xy - wz);
				rotationMatrix[1][1] = 1.0f - 2.0f * (xx + zz);
				rotationMatrix[2][1] = 2.0f * (yz + wx);
				rotationMatrix[3][1] = 0.f;

				rotationMatrix[0][2] = 2.0f * (xz + wy);
				rotationMatrix[1][2] = 2.0f * (yz - wx);
				rotationMatrix[2][2] = 1.0f - 2.0f * (xx + yy);
				rotationMatrix[3][2] = 0.f;

				rotationMatrix[0][3] = 0.f;
				rotationMatrix[1][3] = 0.f;
				rotationMatrix[2][3] = 0.f;
				rotationMatrix[3][3] = 1.f;
			}
			matrix translationMatrix = 0;
			{
				translationMatrix[0][0] = 1.f;
				translationMatrix[1][1] = 1.f;
				translationMatrix[2][2] = 1.f;
				
				translationMatrix[0][3] = fLerpPos.x;
				translationMatrix[1][3] = fLerpPos.y;
				translationMatrix[2][3] = fLerpPos.z;
				translationMatrix[3][3] = 1.f;
			}

			curr = scaleMatrix * translationBack * rotationMatrix * translationToOrigin * translationMatrix;
		}



		matrix result = curr;
		//matrix result = lerp(curr, next, ratio[0]);

		/* if next */
		/*if (animIndex[1] >= 0)
		{
			c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
			c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
			c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
			c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
			curr = matrix(c0, c1, c2, c3);

			n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
			n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
			n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
			n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
			next = matrix(n0, n1, n2, n3);

			matrix nextResult = lerp(curr, next, ratio[1]);
			result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
		}*/

		transform += mul(result, weights[i]);
	}

	return transform;
}


float3 PositionSlerp(float3 start, float3 end, float t)
{
    start = normalize(start);
    end = normalize(end);
    float dotProduct = dot(start, end); // Ensure the dot product doesn't exceed 1 to prevent NaN in acos 
	dotProduct = saturate(dotProduct); 
	float theta = acos(dotProduct); 
	float sinTheta = sin(theta); // Perform spherical linear interpolation 
	return (sin((1.0 - t) * theta) * start + sin(t * theta) * end) / sinTheta;
}

matrix GetAnimationMatrix_2(VS_IN input, bool bNext)
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
    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;
	
    int iChoice = 0;
	if(true == bNext)
        iChoice = 1;
	
    for (int i = 0; i < 4; i++)
    {
		/* cur */
        c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[iChoice], animIndex[iChoice], 0));
        c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[iChoice], animIndex[iChoice], 0));
        c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[iChoice], animIndex[iChoice], 0));
        c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[iChoice], animIndex[iChoice], 0));
        curr = matrix(c0, c1, c2, c3);


        n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[iChoice], animIndex[iChoice], 0));
        n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[iChoice], animIndex[iChoice], 0));
        n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[iChoice], animIndex[iChoice], 0));
        n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[iChoice], animIndex[iChoice], 0));
        next = matrix(n0, n1, n2, n3);

        matrix result = lerp(curr, next, ratio[iChoice]);
		
		///* Tweening */
  //      if (animIndex[1] >= 0)
  //      {
  //          c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
  //          c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
  //          c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
  //          c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
  //          curr = matrix(c0, c1, c2, c3);
            

  //          n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
  //          n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
  //          n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
  //          n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
  //          next = matrix(n0, n1, n2, n3);
		
  //          matrix nextResult = lerp(curr, next, ratio[1]);
  //          result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
  //      }

        transform += mul(result, weights[i]);
    }
	
    return transform;
}

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;


	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	//float4x4	BoneMatrix = GetAnimationMatrix_2(In);
    vector vPosition = 0;
    vector vNormal = 0;
    vector vTangent = 0;
	
    //bool bTween = g_TweenFrames.next.iAnimIndex > 0;
	
    //if (true == bTween)
    //{
    //    float4x4 CurrBoneMatrix = GetAnimationMatrix_2(In, false);
    //    float4x4 NextBoneMatrix = GetAnimationMatrix_2(In, true);
		
    //    vPosition = vector(PositionSlerp(mul(vector(In.vPosition, 1.f), CurrBoneMatrix).xyz, mul(vector(In.vPosition, 1.f), NextBoneMatrix).xyz, g_TweenFrames.fTweenRatio), 1.f);
    //    vNormal = lerp(mul(vector(In.vNormal, 0.f), CurrBoneMatrix), mul(vector(In.vNormal, 0.f), NextBoneMatrix), g_TweenFrames.fTweenRatio);
    //    vTangent = lerp(mul(vector(In.vTangent, 0.f), CurrBoneMatrix), mul(vector(In.vTangent, 0.f), NextBoneMatrix), g_TweenFrames.fTweenRatio);
    //}
    //else
    //{
    //    float4x4 BoneMatrix = GetAnimationMatrix_2(In, false);
    //    vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    //    vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    //    vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);
    //}
	
    float4x4 BoneMatrix = GetAnimationMatrix(In);
    vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vWorldPosition = mul(vPosition, g_WorldMatrix);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTangent = normalize(mul(vTangent, g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vProjPos : TEXCOORD1;
	float4		vWorldPosition : TEXCOORD2;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
    float4      vBloom : SV_TARGET3;
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

    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;

	if (0.f == Out.vDiffuse.a)
		discard;

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

	if (0.f == Out.vDiffuse.a)
		discard;

	return Out;
}



PS_OUT PS_DISSOLVE_DEAD(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float  fDissolveAlpha  = saturate(1.0 - g_fDissolveWeight / g_DissolveDuration + vDissoveTexture.r);
	
	// 픽셀 생략
    if (fDissolveAlpha < 0.3)
        discard;
	
	// 픽셀 색상 지정 : 명암 연산 X
    else if (fDissolveAlpha < 0.5)
    {
        Out.vDiffuse = g_vDissolveColor;
        Out.vNormal  = float4(1.f, 1.f, 1.f, 0.f);
        Out.vBloom   = float4(Out.vDiffuse.r, Out.vDiffuse.g, Out.vDiffuse.b, 0.5f);
    }
	// 기본 픽셀 렌더링
    else
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
        Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
        Out.vBloom   = Caculation_Brightness(Out.vDiffuse);
    }
    
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
	
    if (0.f == Out.vDiffuse.a)
        discard;
	
	return Out;
}


// 그림자 픽셀 쉐이더.
struct PS_OUT_SHADOW_DEPTH
{
	float4		vDepth : SV_TARGET0;
};


PS_OUT_SHADOW_DEPTH PS_SHADOW_DEPTH(PS_IN In)
{
	PS_OUT_SHADOW_DEPTH Out = (PS_OUT_SHADOW_DEPTH)0;

	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if(vColor.a <= 0.01f)
		discard;


	Out.vDepth = vector(In.vProjPos.w / 1000.0f, 0.f, 0.f, 1.f);

	return Out;
}


PS_OUT PS_MAIN_UIDUMMY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);


	float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
	fRimPower = pow(fRimPower, 2.f);

	vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;

	if (0.f == Out.vDiffuse.a)
		discard;

	// UI의 깊이 값을 설정
	float uiDepth = 1.f;

	// 메쉬의 깊이 값과 UI의 깊이 값을 비교하여 UI를 가려내기
	float meshDepth = In.vProjPos.z / In.vProjPos.w;
	float adjustedDepth = saturate(meshDepth - uiDepth);

	// 메쉬의 원래 깊이 값은 그대로 사용
	Out.vDepth = vector(meshDepth, 1.0f - meshDepth, 0.0f, 0.0f);

	// 메쉬의 깊이 값이 UI의 깊이 값보다 작은 경우에만 UI를 가리도록 함
	if (adjustedDepth >= 0.0f)
		discard;

	return Out;
}

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
		PixelShader = compile ps_5_0 PS_DISSOLVE_DEAD();
	}

	pass Temp0
	{
		// 3
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp1
	{
		// 4
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp2
	{
		// 5
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp3
	{
		// 6
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp4
	{
		// 6
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp5
	{
		// 7
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp6
	{
		// 8
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Shadow_Depth
	{
		// 10
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Shadow, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW_DEPTH();
	}

	pass UI_Dummy_NonDepth
	{
		// 11
		SetRasterizerState(RS_CostumeDummy);
		SetDepthStencilState(DSS_CostumeDummy, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
};