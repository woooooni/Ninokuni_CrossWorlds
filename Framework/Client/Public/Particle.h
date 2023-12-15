#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
class CRigidBody;
class CVIBuffer_Particle;
END

BEGIN(Client)

class CParticle final : public CGameObject
{
public:
	typedef struct tagParticleDesc
	{	
		// 반복 여부
		_bool bParticleLoop = true;

		// 파티클 개수
		_uint iNumEffectCount = 5; 

		// 위치 (분포 범위)
		_float3 fRange = _float3(0.f, 0.f, 0.f);

#pragma region 크기
		_bool bScaleSameRate = true; // 정비율
		_float2 fScaleStart  = _float2(5.f, 10.f);

		_bool bScaleChange       = false;
		_bool bScaleChangeRandom = false;
		_float2 fScaleChangeTime = _float2(1.f, 5.f);

		_bool bScaleAdd       = false;
		_bool bScaleLoop      = false;
		_bool bScaleLoopStart = false;

		_float2 fScaleMin   = _float2(1.f, 1.f);
		_float2 fScaleMax   = _float2(10.f, 10.f);
		_float2 fScaleSpeed = _float2(1.f, 5.f);
#pragma endregion

#pragma region 이동
		_float2 fVelocitySpeed = _float2(1.f, 1.f);

		_float3 vVelocityMinStart = _float3(-1.0f, -1.0f, -1.0f);
		_float3 vVelocityMaxStart = _float3(1.0f, 1.0f, 1.0f);

		_bool bVelocityChange = false;
		_bool bVelocityChangeRandom = false;
		_float2 fVelocityChangeTime = _float2(1.f, 5.f);

		_bool bVelocityLoop   = false;
		_uint iVelocityCountCur = 0;
		_uint iVelocityCountMax = 2;

		_uint iVelocityUse     = 10;
		_float3* pVelocityMin  = nullptr;
		_float3* pVelocityMax  = nullptr;
		_float2* pVelocityTime = nullptr;
#pragma endregion

#pragma region 회전
		_bool bBillboard = false;

		_bool bRandomAxis = false;
		_vector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_bool bRandomAngle = false;
		_float fAngle = 0.f;


		_bool bRotationChange  = false;

		_bool bRotationChangeRandom = false;
		_float2 fRotationChangeTime = _float2(1.f, 5.f);

		_bool bRotationAdd = false;

		_float2 fRotationSpeed = _float2(1.f, 5.f);
#pragma endregion

		// 지속 시간
		_float2	fLifeTime = _float2(5.f, 10.f);

		// 박스 범위
		_bool   bUseBox = false;
		_float3 fBoxMin;
		_float3 fBoxMax;

#pragma region 텍스처
		wstring strDiffuseTetextureName = L"";
		wstring strDiffuseTetexturePath = L"";
		wstring strAlphaTexturName      = L"";
		wstring strAlphaTexturPath      = L"";
		_bool bRandomStartIndex = true;
		_float2 fUVIndex    = _float2(0.f, 0.f);
		_float2 fUVMaxCount = _float2(1.f, 1.f);
#pragma endregion

#pragma region 애니메이션
		_bool bAnimation        = false;
		_bool bAnimationLoop    = false;
		_float2 fAnimationSpeed = _float2(1.f, 1.f);
#pragma endregion


#pragma region 색상
		_float4	vColor     = _float4(0.f, 0.f, 0.f, 0.f);
		_bool bColorRandom = false;

		_bool bColorChange = false;
		_bool bColorLoop = false;
		_uint iColorCountCur = 0;
		_uint iColorCountMax = 1;
		_float4* pColor     = nullptr;
		_float2* pColorTime = nullptr;
#pragma endregion

	} PARTICLE_DESC;

protected:
	CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;

public:
	virtual HRESULT Initialize_Prototype(const PARTICLE_DESC* pParticleDesc, const wstring& strParticleFilePath);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const PARTICLE_DESC& Get_ParticleDesc() { return m_tParticleDesc; }
	void Set_ParticleDesc(const PARTICLE_DESC& tDesc);
 
	class CTransform* Get_TransformCom() { return m_pTransformCom; }

private:
	_bool m_isCloned = { false };
	PARTICLE_DESC m_tParticleDesc;

private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	class CVIBuffer_Particle* m_pVIBufferCom = nullptr;
	class CShader* m_pShaderCom = nullptr;

	class CTexture* m_pDiffuseTextureCom = nullptr;
	class CTexture* m_pAlphaTextureCom = nullptr;

private:
	HRESULT Bind_ShaderResource();

protected:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Components() override;
	void Load_ParticleData(const wstring& strFileName);
	void* Get_ParticleBufferInfo();
	void Set_Texture_Diffuse();
	void Set_Texture_Alpha();

public:
	static CParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const wstring& strObjectTag, const PARTICLE_DESC* pParticleDesc = nullptr, const wstring& strParticleFilePath = {});
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END


