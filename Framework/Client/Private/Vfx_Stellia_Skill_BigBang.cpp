#include "stdafx.h"
#include "Vfx_Stellia_Skill_BigBang.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Skill_BigBang::CVfx_Stellia_Skill_BigBang(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_BigBang::CVfx_Stellia_Skill_BigBang(const CVfx_Stellia_Skill_BigBang& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_BigBang::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_BIGBANG;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	{
		m_pFrameTriger[TYPE_ET1_E_SMOKE_IN_01] = 4;
		m_pPositionOffset[TYPE_ET1_E_SMOKE_IN_01] = _float3(0.f, 1.1f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_SMOKE_IN_01]    = _float3(15.f, 1.f, 15.f);
		m_pRotationOffset[TYPE_ET1_E_SMOKE_IN_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES_IN] = 4;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES_IN] = _float3(0.f, 1.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES_IN]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES_IN] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_D_DECAL] = 6;
		m_pPositionOffset[TYPE_ET1_D_DECAL] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_DECAL]    = _float3(30.f, 2.5f, 30.f);
		m_pRotationOffset[TYPE_ET1_D_DECAL] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_SMOKE_OUT] = 118;
		m_pPositionOffset[TYPE_ET2_E_SMOKE_OUT] = _float3(0.f, 0.f, -1.f);
		m_pScaleOffset[TYPE_ET2_E_SMOKE_OUT]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_SMOKE_OUT] = _float3(90.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_SMOKE_UP] = 118;
		m_pPositionOffset[TYPE_ET2_E_SMOKE_UP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_SMOKE_UP]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_SMOKE_UP] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_CIRCLES_OUTUP] = 120;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES_OUTUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES_OUTUP]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES_OUTUP] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Stellia_Skill_BigBang::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_BigBang::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_E_SMOKE_IN_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_SMOKE_IN_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_BigBang_Smoke_In"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_SMOKE_IN_01], m_pScaleOffset[TYPE_ET1_E_SMOKE_IN_01], m_pRotationOffset[TYPE_ET1_E_SMOKE_IN_01], nullptr, &m_pSmokeIn_01, false);
			Safe_AddRef(m_pSmokeIn_01);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES_IN && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES_IN])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_BigBang_Circles_In"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES_IN], m_pScaleOffset[TYPE_ET1_P_CIRCLES_IN], m_pRotationOffset[TYPE_ET1_P_CIRCLES_IN], nullptr, &m_pCircleInParticle, false);
			Safe_AddRef(m_pCircleInParticle);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_D_DECAL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_DECAL])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Stellia_BigBang_Decal"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_DECAL], m_pScaleOffset[TYPE_ET1_D_DECAL], m_pRotationOffset[TYPE_ET1_D_DECAL], nullptr, &m_pDecal, false);
			Safe_AddRef(m_pDecal);
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_ET2_E_SMOKE_OUT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_SMOKE_OUT])
		{
			CEffect* Smoke_01 = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_BigBang_Smoke_Out"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_SMOKE_OUT], m_pScaleOffset[TYPE_ET2_E_SMOKE_OUT], m_pRotationOffset[TYPE_ET2_E_SMOKE_OUT], nullptr, &Smoke_01);
			if (nullptr != Smoke_01)
			{
				Smoke_01->Set_ScaleSpeed(10.f);
				Smoke_01->Set_Color(_float3(0.482f, 0.157f, 1.f));
			}

			CEffect* Smoke_02 = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_BigBang_Smoke_Out"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_SMOKE_OUT], m_pScaleOffset[TYPE_ET2_E_SMOKE_OUT], m_pRotationOffset[TYPE_ET2_E_SMOKE_OUT], nullptr, &Smoke_02);
			if (nullptr != Smoke_02)
			{
				Smoke_02->Set_ScaleSpeed(15.f);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_SMOKE_UP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_SMOKE_UP])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_JumpStamp_Shock01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_SMOKE_UP], m_pScaleOffset[TYPE_ET2_E_SMOKE_UP], m_pRotationOffset[TYPE_ET2_E_SMOKE_UP]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES_OUTUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES_OUTUP])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_BigBang_CircleCircle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES_OUTUP], m_pScaleOffset[TYPE_ET2_P_CIRCLES_OUTUP], m_pRotationOffset[TYPE_ET2_P_CIRCLES_OUTUP]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_BigBang_CircleCircle_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES_OUTUP], m_pScaleOffset[TYPE_ET2_P_CIRCLES_OUTUP], m_pRotationOffset[TYPE_ET2_P_CIRCLES_OUTUP]);

			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Stellia_Skill_BigBang::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_BigBang::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_BigBang::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_BigBang* CVfx_Stellia_Skill_BigBang::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_BigBang* pInstance = new CVfx_Stellia_Skill_BigBang(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_BigBang");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_BigBang::Clone(void* pArg)
{
	CVfx_Stellia_Skill_BigBang* pInstance = new CVfx_Stellia_Skill_BigBang(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_BigBang");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_BigBang::Free()
{
	__super::Free();

	if (nullptr != m_pSmokeIn_01)
	{
		m_pSmokeIn_01->Set_UVLoop(1);
		Safe_Release(m_pSmokeIn_01);
	}
	if (nullptr != m_pCircleInParticle)
	{
		m_pCircleInParticle->Set_LoopParticle(false);
		Safe_Release(m_pCircleInParticle);
	}
	if (nullptr != m_pDecal)
	{
		m_pDecal->Start_AlphaDeleate();
		Safe_Release(m_pDecal);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}