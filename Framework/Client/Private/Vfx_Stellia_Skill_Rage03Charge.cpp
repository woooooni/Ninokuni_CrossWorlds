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
		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_00] = 0;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_00] = _float3(-0.005f, 0.14f, -0.08f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_00]    = _float3(30.f, 30.f, 40.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_00] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_01] = 0;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_01] = _float3(-0.005f, 0.14f, -0.09f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_01]    = _float3(30.f, 30.f, 40.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 4.5f, 6.5f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET2_E_CIRCLE] = _float3(0.f, 4.f, -6.5f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLE] = _float3(0.f, 180.f, 0.f);
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
		if (m_iCount == TYPE_ET1_E_CIRCLELINE_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_00])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_FrontLine_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_00], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_00], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_00], m_pOwnerObject, &m_fCircleLine00Effect, false);
			if (nullptr != m_fCircleLine00Effect)
			{
				Safe_AddRef(m_fCircleLine00Effect);
				m_fCircleLine00Effect->Set_Color(_float3(0.45f, 0.1f, 1.0f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_FrontLine_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE_01], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE_01], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE_01], m_pOwnerObject, &m_fCircleLine01Effect, false);
			if (nullptr != m_fCircleLine01Effect)
			{
				Safe_AddRef(m_fCircleLine01Effect);
				m_fCircleLine01Effect->Set_Color(_float3(0.760f, 0.399f, 1.0f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Charge_FrontCircles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES], m_pOwnerObject, &m_fCircleParticle, false);
			Safe_AddRef(m_fCircleParticle);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLE])
		{
			m_fTimeAcc += fTimeDelta;
			if (m_fTimeAcc > 0.3F)
			{
				m_fTimeAcc = 0.f;

				CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr != pOwnerTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_TrailLine"),
						pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET2_E_CIRCLE], m_pScaleOffset[TYPE_ET2_E_CIRCLE], m_pRotationOffset[TYPE_ET2_E_CIRCLE]);
				}
			}
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