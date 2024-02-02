#include "Engine_Shader_Defines.hpp"

#define UNITY_PI            3.14159265359f
#define UNITY_TWO_PI        6.28318530718f
#define UNITY_FOUR_PI       12.56637061436f

Texture2D BladeTexture;
Texture2D WindTexture;


cbuffer MatrixBuffers
{
    Matrix world, view, projection;
};

cbuffer CB_GrassBuffer
{
    float fBladeWidthMin = 0.02f;
    float fBladeWidthMax = 0.05f;
    float fBladeHeightMin = 0.3f;
    float fBladeHeightMax = 0.5f;
    
    float fBladeBendDistance = 0.38f;
    float fBladeBendCurve = 2.0f; 
    float fBendDelta = 0.2f;
    float fTessellationGrassDistance = 0.1f;
    
    float fGrassThreshold = 0.5f;
    float fGrassFalloff = 0.05f;
    float fWindFrequency = 0.01f;
    float fFad = 0.0f;
    
    
    float4 vWindVelocity = float4(1.0f, 0.0f, 0.0f, 0.0f);
    float4 vWindMap_ST = float4(1.0f, 1.0f, 3.0f, 3.0f); // xy = scale zw = offset?
};

cbuffer CB_Time
{
    float fTime = 0.0f;
};

struct VertexInput
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL0;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VertexOutput
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL0;
    float4 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
    //float2 vWindSample : TEXCOORD1;
};

struct GeomData
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vWorldPos : TEXCOORD1;
};

float rand(float3 co)
{
    return frac(sin(dot(co.xyz, float3(12.9898f, 78.233f, 53.339f))) * 43758.5453f);
}

float3x3 AngleAxis3x3(float angle, float3 axis)
{
    float c, s;
    sincos(angle, s, c);
    
    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    
    return float3x3
    (
          t * x * x + c, t * x * y - s * z, t * x * z + s * y,
          t * x * y + s * z, t * y * y + c, t * y * z - s * x,
          t * x * z - s * y, t * y * z + s * x, t * z * z + c
    );
}

VertexOutput geomVert(VertexInput v)
{
    VertexOutput o;
    
    o.vPosition = float4(v.vPosition, 1.0f);
    o.vNormal = v.vNormal;
    o.vTangent = float4(v.vTangent, 0.0f);
    o.vTexcoord = v.vTexcoord;
    
    return o;
}

VertexOutput tessVert(VertexInput v)
{
    VertexOutput o;
    o.vPosition = float4(v.vPosition, 1.0f);
    o.vNormal = v.vNormal;
    o.vTangent = float4(v.vTangent, 1.0f);
    o.vTexcoord = v.vTexcoord;
    //o.vWindSample = WindTexture.SampleLevel(PointSampler, v.vTexcoord, 0.0f);
    
    return o;
}


struct TessellationFactors
{
    float edge[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

float tessellationEdgeFactor(VertexOutput vert0, VertexOutput vert1)
{
    float3 v0 = vert0.vPosition.xyz;
    float3 v1 = vert1.vPosition.xyz;
    float edgeLength = distance(v0, v1);
    return saturate(edgeLength / fTessellationGrassDistance);

}


TessellationFactors patchConstantFunc(InputPatch<VertexOutput, 3> patch)
{
    TessellationFactors f;
    
    f.edge[0] = tessellationEdgeFactor(patch[0], patch[1]);
    f.edge[1] = tessellationEdgeFactor(patch[1], patch[2]);
    f.edge[2] = tessellationEdgeFactor(patch[2], patch[0]);
    
    f.inside = (f.edge[0] + f.edge[1] + f.edge[2]) / 3.0f;
    
    return f;
}


[domain("tri")]
[outputcontrolpoints(3)]
[outputtopology("triangle_cw")]
[partitioning("integer")]
[patchconstantfunc("patchConstantFunc")]
VertexOutput hull(InputPatch<VertexOutput, 3> patch, uint id : SV_OutputControlPointID)
{
    return patch[id];
}



[domain("tri")]
VertexOutput domain(TessellationFactors factors, OutputPatch<VertexOutput, 3> patch, float3 barycentricCoordinates : SV_DomainLocation)
{
    VertexInput i;

#define INTERPOLATE(fieldname) i.fieldname = \
					patch[0].fieldname * barycentricCoordinates.x + \
					patch[1].fieldname * barycentricCoordinates.y + \
					patch[2].fieldname * barycentricCoordinates.z;

    
	INTERPOLATE(vPosition)
	INTERPOLATE(vNormal)
	INTERPOLATE(vTangent)
	INTERPOLATE(vTexcoord)


    return tessVert(i);
}

GeomData TransformGeomToClip(float3 position,
float3 offset, float3x3 transformationMatrix, float2 texcoord)
{
    GeomData o;
    
    matrix matWV, matWVP;
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);

    o.vPosition = float4(mul(position + mul(transformationMatrix, offset), (float3x3) matWVP), 1.0f);
    o.vTexcoord = texcoord;
    o.vWorldPos = mul(float4(position, 1.0f), world);
    
    return o;
}

GeomData ObjectToClipPos(float3 pos, float2 uv)
{
    GeomData o = (GeomData)0;
    
    matrix matWV, matWVP;
    
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);
    
    o.vPosition = mul(float4(pos, 1.0f), matWVP);
    o.vTexcoord = uv;
    return o;
}


GeomData GenerateGrassVertex(float3 vertexPosition, float width, float height, float forward, float2 uv, float3x3 transformMatrix)
{
    float3 tangentPoint = float3(width, forward, height);
    
    float3 localPosition = vertexPosition + mul(transformMatrix, tangentPoint);
    return ObjectToClipPos(localPosition, uv);
}

GeomData TempVertexOutput(float3 pos, float2 uv)
{
    GeomData o = (GeomData) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);
    
    o.vPosition = mul(float4(pos, 1.0f), matWVP);
    o.vTexcoord = uv;
    return o;
}

#define BLADE_SEGMENTS 4

[maxvertexcount(BLADE_SEGMENTS * 2 + 1)]
void geom(point VertexOutput input[1], inout TriangleStream<GeomData> triStream)
{
    GeomData o = (GeomData) 0;
    
    float3 pos = input[0].vPosition;
    float3 vNormal = input[0].vNormal;
    float3 vTangent = input[0].vTangent;
    float3 vBiNormal = cross(vNormal, vTangent);
    
    float3x3 tangentToLocal = float3x3(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
    
    float3x3 randRotMatrix = AngleAxis3x3(rand(pos) * UNITY_TWO_PI, float3(0.0f, 1.0f, 0.0f));
    float3x3 randBendMatrix = AngleAxis3x3(rand(pos.zzx) * fBendDelta * UNITY_PI * 0.5f, float3(-1.0f, 0.0f, 0.0f));

    float2 windUV = pos.xz * vWindMap_ST.xy + vWindMap_ST.zw + normalize(vWindVelocity.xzy) * fWindFrequency * fTime;
    float2 windSample = (WindTexture.SampleLevel(PointSampler, input[0].vTexcoord.y * fTime, 0.0f) * 2.0f - 1.0f) * length(vWindVelocity);
    float3 windAxis = normalize(float3(windSample.x, windSample.y, 0.0f));
    float3x3 windMatrix = AngleAxis3x3(UNITY_PI * windSample, windAxis);
     
    float3x3 baseTransformationMatrix = mul(tangentToLocal, randRotMatrix);
    float3x3 tipTransformationMatrix = mul(mul(mul(tangentToLocal, windMatrix), randBendMatrix), randRotMatrix);
    
    float width = lerp(fBladeWidthMin, fBladeWidthMax, rand(pos.xzy));
    float height = lerp(fBladeHeightMin, fBladeHeightMax, rand(pos.zyx));
    float foward = rand(pos.yyz) * fBladeBendDistance;
    
    for (int i = 0; i < BLADE_SEGMENTS; ++i)
    {
        float t = i / (float) BLADE_SEGMENTS;
        float3 offset = float3(width * (1 - t), pow(t, fBladeBendCurve) * foward, height * t);
        
        float3x3 transformationMatrix = (i == 0) ? baseTransformationMatrix : tipTransformationMatrix;
        
        triStream.Append(ObjectToClipPos(pos + mul(transformationMatrix, float3(offset.x, offset.y, offset.z)), float2(0, t)));
        triStream.Append(ObjectToClipPos(pos + mul(transformationMatrix, float3(-offset.x, offset.y, offset.z)), float2(1, t)));
    }
    triStream.Append(ObjectToClipPos(pos + mul(tipTransformationMatrix, float3(0, foward, height)), float2(0.5f, 1.0f)));
    triStream.RestartStrip();
}

float4 PS_Grass(GeomData i) : SV_Target
{
    float4 bottomColor = float4(0.29f, 0.431f, 0.294f, 1.0f);
    float4 TopColor = float4(1.0f, 0.639f, 0.447f, 1.0f);
   
    float4 vColor = lerp(bottomColor, TopColor, i.vTexcoord.y);
    
    return vColor;
}

RasterizerState RS_WireFrame
{
    FillMode = WireFrame;
    CullMode = Back;
    FrontCounterClockwise = false;
};

technique11 DefaultGrass
{
	// Grass
    pass RealTimeGrass
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 tessVert();
        HullShader = compile hs_5_0 hull();
        DomainShader = compile ds_5_0 domain();
        GeometryShader = compile gs_5_0 geom();
        PixelShader = compile ps_5_0 PS_Grass();
    }
}