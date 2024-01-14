#include "Engine_Shader_Defines.hpp"

struct VertexInput
{
    float3 vPosition : POSITION;
    float4 vColor : COLOR;
};

struct HullInput
{
    float3 vPosition : POSITION;
    float4 vColor : COLOR;
};

// Vertex
HullInput ColorVertexShader(VertexInput input)
{
    HullInput output;
    
    // 정점 위치를 선체 쉐이더에 전달.
    output.vPosition = input.vPosition;
    
    // 픽셀 쉐이더가 사용할 입력 색상을 저장한다.
    output.vColor = input.vColor;
    
    return output;
}

cbuffer TessellationBuffer
{
    float fTessellationAmount;
};

struct ConstantOutput
{
    float fEdges[3] : SV_TessFactor;
    float fInside : SV_InsideTessFactor;
};

struct HullOutput
{
    float3 vPosition : POSITION;
    float4 vColor : COLOR;
};

// 훌 쉐이더에서 사용할 함수
ConstantOutput ConstantHullShader(InputPatch<HullInput, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutput output = (ConstantOutput) 0;
    
    // SV_TessFactor는 삼각형의 한 변을 몇 개로 나눌건지. -> fEdges
    output.fEdges[0] = fTessellationAmount;
    output.fEdges[1] = fTessellationAmount;
    output.fEdges[2] = fTessellationAmount;
    
    // 트라이앵글 내부를 터셀레이션 하기위한 터셀레이션 계수 설정
    // 내부 컨트롤 포인트를 결정한다.
    output.fInside = fTessellationAmount;

    
    return output;
}

////
//HullShader
////

[domain("tri")] // 패치의 종류, 삼각형은 tri( tri, quad , isoline )
[partitioning("integer")] // 테셀레이션 세분화 모드 (integer : 정수, fractional_even, fractional_odd)
[outputtopology("triangle_cw")] // 새로 만들어진 삼각형들의 감기는 순서 ( cw : 시계방향 , ccw : 반시계 line : 선 테셀레이션  )
[outputcontrolpoints(3)] // 입력된 컨트롤 포인트 개수
[patchconstantfunc("ConstantHullShader")] // constant Hull Shader 함수 이름
HullOutput ColorHullShader(InputPatch<HullInput, 3> patch, uint pointId : SV_OutputControlPointID,
uint patchId : SV_PrimitiveID)
{
    HullOutput output = (HullOutput) 0;

    // 제어점의 위치를 출력 위치로 설정
    output.vPosition = patch[pointId].vPosition;
    
    // 입력 색상을 출력 색상으로 설정
    output.vColor = patch[pointId].vColor;
    
    return output;
}

cbuffer MatrixBuffer
{
    matrix worldMatrix, viewMatrix, projectionMatrix;
};

struct PixelInput
{
    float4 vPosition : SV_POSITION;
    float4 vColor : COLOR;
};

// Domain Shader

// 모자이크 처리된 데이터를 받아서 버텍스 쉐이더에 이전에 사용한 것과 같은 
// 최종 꼭지점을 조작하고 변환하는데 사용

[domain("tri")]
PixelInput ColorDomainShader(ConstantOutput input, float3 uvwCoord : SV_DomainLocation,
const OutputPatch<HullOutput, 3> patch)
{
    float3 vertexPosition;
    PixelInput output = (PixelInput) 0;

    vertexPosition = uvwCoord.x * patch[0].vPosition + uvwCoord.y * patch[1].vPosition +
    uvwCoord.z * patch[2].vPosition;
    
    matrix matWV, matWVP;
    
    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);
    
    output.vPosition = mul(float4(vertexPosition, 1.0f), matWVP);
    
    output.vColor = patch[1].vColor;
    
    return output;
}

// PixelShader
struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vSunMask : SV_TARGET4;
};

PS_OUT ColorPixelShader(PixelInput input)
{
    PS_OUT output = (PS_OUT) 0;
    
    output.vColor = input.vColor;
    output.vSunMask = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return output;
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
        SetRasterizerState(RS_WireFrame);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 ColorVertexShader();
        GeometryShader = NULL;
        HullShader = compile hs_5_0 ColorHullShader();
        DomainShader = compile ds_5_0 ColorDomainShader();
        PixelShader = compile ps_5_0 ColorPixelShader();
    }
}