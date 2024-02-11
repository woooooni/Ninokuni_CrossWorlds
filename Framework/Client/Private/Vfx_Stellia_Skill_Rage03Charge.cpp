#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage03Charge.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Particle.h"

CVfx_Stellia_Skill_Rage03Charge::CVfx_Stellia_Skill_Rage03Charge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage03Charge::CVfx_Stellia_Skill_Rage03Charge(const CVfx_Stellia_Skill_Rage03Charge& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage03Charge::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_RAGE3CHARGE;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_E_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_E_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLE] = _float3(-90.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_00] = 0;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_00] = _float3(-0.005f, 0.14f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_00]    = _float3(30.f, 30.f, 30.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_00] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_01] = 0;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_01] = _float3(-0.005f, 0.14f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_01]    = _float3(30.f, 30.f, 30.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 1.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_DECAL] = 0;
		m_pPositionOffset[TYPE_ET2_D_DECAL] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_DECAL]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_D_DECAL] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage03Charge::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage03Charge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_E_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLE], m_pScaleOffset[TYPE_ET1_E_CIRCLE], m_pRotationOffset[TYPE_ET1_E_CIRCLE], m_pOwnerObject, &m_fCircleEffect, false);
			Safe_AddRef(m_fCircleEffect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_00])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_FrontLine_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_00], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_00], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_00], m_pOwnerObject, &m_fCircleLine00Effect, false);
			Safe_AddRef(m_fCircleLine00Effect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_FrontLine_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_01], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_01], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_01], m_pOwnerObject, &m_fCircleLine01Effect, false);
			Safe_AddRef(m_fCircleLine01Effect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Charge_FrontCircles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES], m_pOwnerObject, &m_fCircleParticle, false);
			Safe_AddRef(m_fCircleParticle);
			m_iCount++;
		}

		else if (m_iCount > TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_DECAL])
		{
			m_fTimeAcc += fTimeDelta;
			if (m_fTimeAcc > 1.F)
			{
				m_fTimeAcc = 0.f;

				//GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT(""),
				//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_D_DECAL], m_pScaleOffset[TYPE_ET2_D_DECAL], m_pRotationOffset[TYPE_ET2_D_DECAL]);

				//GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT(""),
				//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES]);
			}

			if(m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_DECAL] + 20)
				m_iCount += 2;
		}
	}
}

void CVfx_Stellia_Skill_Rage03Charge::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage03Charge::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage03Charge::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage03Charge* CVfx_Stellia_Skill_Rage03Charge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage03Charge* pInstance = new CVfx_Stellia_Skill_Rage03Charge(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage03Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage03Charge::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage03Charge* pInstance = new CVfx_Stellia_Skill_Rage03Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage03Charge");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage03Charge::Free()
{
	__super::Free();

	if (nullptr != m_fCircleEffect)
	{
		m_fCircleEffect->Set_Dead(true);
		Safe_Release(m_fCircleEffect);
	}
	if (nullptr != m_fCircleLine00Effect)
	{
		m_fCircleLine00Effect->Set_Dead(true);
		Safe_Release(m_fCircleLine00Effect);
	}
	if (nullptr != m_fCircleLine01Effect)
	{
		m_fCircleLine01Effect->Set_Dead(true);
		Safe_Release(m_fCircleLine01Effect);
	}
	if (nullptr != m_fCircleParticle)
	{
		m_fCircleParticle->Set_Dead(true);
		Safe_Release(m_fCircleParticle);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}