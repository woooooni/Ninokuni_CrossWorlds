#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle final : public CVIBuffer
{
public:
	typedef struct tagParticleBufferDesc
	{
		// 반복 여부
		_bool bLoop;

		// 분포 범위
		_float3 fRange;

		// 크기
		_bool bSameRate;
		_float2 fScale;

		// 지속시간
		_float2	fLifeTime;

		// 속도
		_float2 fSpeed;

		// 움직임
		_float3 vVelocityMin;
		_float3 vVelocityMax;

		// 박스 범위
		_bool bUseBox;
		_float3 fBoxMin;
		_float3 fBoxMax;

		// 색상
		_bool bRandomColor;
		_float4	vDiffuseColor;

		// 텍스처
		_bool bAnimation;
		_bool bAnimationLoop;
		_bool bRandomStartIndex;
		_float fDiffuseTextureIndex;
		_float fDiffuseTextureIndexMax;
		_float2 fAnimationSpeed;

		


	} PARTICLE_BUFFER_DESC;

public:
	typedef struct tagParticleDesc
	{
		_float fTimeAccs;       //4
		_float fLifeTimes;      //4
		_float fSpeeds;         //4
		_float fAnimationSpeed; //4

		_float fTextureIndex;   //4
		_float fTemp1;          //4
		_float fTemp2;          //4
		_float fTemp3;          //4

		_float4 vVelocity;     //16
		_float4 vColor;        //16

	} PARTICLE_INFO_DESC;

private:
	CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle(const CVIBuffer_Particle& rhs);
	virtual ~CVIBuffer_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	void Tick(_float fTimeDelta);
	virtual HRESULT Render(_uint iCount);

public:
	void Set_ParticleBufferDesc(const PARTICLE_BUFFER_DESC& tDesc);
	vector<PARTICLE_INFO_DESC>& Get_ParticleInfo() { return m_vecParticleInfoDesc; }

	_bool Get_Finished() { return m_bFinished; }

protected:
	_uint m_iStrideInstance = { 0 };
	_uint m_iNumInstance    = { 0 }; // 파티클 개수

	ID3D11Buffer* m_pVBInstance = { nullptr };
	VTXINSTANCE* m_pVertices = { nullptr };

protected:
	PARTICLE_BUFFER_DESC m_tParticleDesc;

	_uint m_iMaxCount = 1000;
	vector<PARTICLE_INFO_DESC> m_vecParticleInfoDesc;

	_bool m_bFinished = { false };

public:
	static CVIBuffer_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END