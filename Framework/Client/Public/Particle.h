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
	enum PARTICLEPROJTYPE { TYPE_PERSPECTIVE, TYPE_ORTHOGRAPHIC, TYPE_END };

public:
	typedef struct tagParticleDesc
	{	
		// 파티클 타입
		PARTICLEPROJTYPE eParticleType = TYPE_PERSPECTIVE;
		_bool bParticleSortZ           = false;

		// 반복 여부
		_bool bParticleLoop = true;

		// 파티클 개수
		_uint iNumEffectMaxCount = 1000; // 사용가능한 최대 파티클 정점 개수
		_uint iNumEffectCount    = 10;

		// 위치 (분포 범위)
		_float3 fRange         = _float3(0.f, 0.f, 0.f);
		_float2 fRangeDistance = _float2(0.f, 0.f);

#pragma region 크기
		_bool bScaleSameRate = true; // 정비율
		_float2 fScaleStart  = _float2(2.f, 4.f);

		_bool bScaleChange = false; 
		_float2 fScaleChangeStartDelay = _float2(0.f, 0.f);

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
		_float2 fVelocityChangeStartDelay = _float2(0.f, 0.f);

		_bool bVelocityChangeRandom = false;
		_float2 fVelocityChangeTime = _float2(1.f, 5.f);

		_bool bVelocityLoop = false;
		_uint iVelocityCountCur = 0;
		_uint iVelocityCountMax = 2;

		_uint iVelocityUse     = 10; // 사용하는 Velocity 포인터 변수 개수
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
		_float2 fRotationChangeStartDelay = _float2(0.f, 0.f);

		_float2 fRotationSpeed = _float2(30.f, 30.f);

		_bool bRotationChangeRandom = false;
		_float2 fRotationChangeTime = _float2(1.f, 1.f);

		_bool bRotationAdd = false;
#pragma endregion

		// 지속 시간
		_float2	fLifeTime = _float2(5.f, 10.f);

		// 박스 범위
		_bool   bUseBox = false;
		_float3 fBoxMin;
		_float3 fBoxMax;

#pragma region 텍스처
		wstring strDiffuseTetextureName = L"Prototype_Component_Texture_Effect_SubUV";
		wstring strDiffuseTetexturePath = L"";
		wstring strAlphaTexturName      = L"";
		wstring strAlphaTexturPath      = L"";

		_bool bRandomStartIndex = true;
		_float2 fUVIndex    = _float2(0.f, 0.f);
		_float2 fUVMaxCount = _float2(1.f, 1.f);

		_uint iTextureIndexDiffuse = 0;
		_uint iTextureIndexAlpha   = 0;
#pragma endregion

#pragma region 애니메이션
		_bool bAnimation        = false;
		_bool bAnimationLoop    = false;
		_float2 fAnimationSpeed = _float2(1.f, 1.f);
#pragma endregion

#pragma region 알파
		_float2 fStartAlpha = _float2(0.f, 0.f);

		_bool bFadeCreate   = false;
		_bool bFadeDelete   = false;
		_float2 fFadeSpeed  = _float2(1.f, 1.f);

		_bool   bFadeChange = false;
		_bool   bFadeIn     = false; // 점점 뚜렷해지다.
		_float2 fFadeChangeStartDelay = _float2(0.f, 0.f);
#pragma endregion

#pragma region 색상
		_bool bColorRandom = false;
		_float4	vColorS = _float4(1.f, 0.f, 0.f, 0.f);

		_bool bColorChange       = false;

		_bool bColorChangeRandom = false;
		_float2 fColorChangeRandomTime = _float2(4.f, 4.f);

		_bool bColorLoop = false;
		_float2 fColorChangeStartDelay = _float2(2.f, 2.f);

		_float2 fColorChangeStartM = _float2(4.f, 4.f);
		_float4 fColorM = _float4(0.f, 1.f, 0.f, 0.f);

		_float2 fColorChangeStartF = _float2(6.f, 6.f);
		_float4 fColorF = _float4(0.f, 0.f, 1.f, 0.f);

		// 보간 시간
		_float2 fColorDuration = _float2(2.f, 2.f);
#pragma endregion

#pragma region 블러
		_bool   bBloomPowerRandom = false;
		_float4 fBloomPower       = _float4(1.f, 0.f, 0.f, 1.f);
		_bool   bBlurPowerRandom  = false;
		_float  fBlurPower        = 0.5f;
#pragma endregion

#pragma region 기타 정보
		_uint iShaderPass      = 0;
		_float  fAlpha_Discard = 0.5f;
		_float3 fBlack_Discard = _float3(0.5f, 0.5f, 0.5f);
#pragma endregion

	} PARTICLE_DESC;

protected:
	CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;

public:
	virtual HRESULT Initialize_Prototype(const PARTICLE_DESC* pParticleDesc);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const PARTICLE_DESC& Get_ParticleDesc() { return m_tParticleDesc; }
	void Set_ParticleDesc(const PARTICLE_DESC& tDesc);
	void Set_Position_Particle(_float4x4 WorldMatrix);
	void Set_Position_Particle(_float3 fPosition);

	void Set_Owner(CGameObject* pGameObject) { m_pOwnerObject = pGameObject; }
	void Set_LoopParticle(_bool bLoop) { m_tParticleDesc.bParticleLoop = bLoop; }
	class CTransform* Get_TransformCom() { return m_pTransformCom; }

private:
	_bool m_isCloned = { false };
	PARTICLE_DESC m_tParticleDesc;
	_bool m_bParticleDie = false;

	_float4x4 m_ViewMatrix;
	_float4x4 m_ProjMatrix;

	class CGameObject* m_pOwnerObject = nullptr;

private:
	class CRenderer*  m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	class CVIBuffer_Particle* m_pVIBufferCom = nullptr;
	class CShader* m_pShaderCom = nullptr;

	class CTexture* m_pDiffuseTextureCom = nullptr;
	class CTexture* m_pAlphaTextureCom = nullptr;

private:
	HRESULT Bind_ShaderResource();

	void* Get_ParticleBufferInfo();
	void Set_Position_Perspective(_float3 fPosition);
	void Set_Position_Orthographic(_float2 fPosition);
	void Set_Texture_Diffuse();
	void Set_Texture_Alpha();

protected:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Components() override;

public:
	static CParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const wstring& strObjectTag, const PARTICLE_DESC* pParticleDesc);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END