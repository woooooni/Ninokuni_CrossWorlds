#include "stdafx.h"
#include "Vfx_Stellia_Skill_ClawDouble.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

CVfx_Stellia_Skill_ClawDouble::CVfx_Stellia_Skill_ClawDouble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_ClawDouble::CVfx_Stellia_Skill_ClawDouble(const CVfx_Stellia_Skill_ClawDouble& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_ClawDouble::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_ATTACK2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_TRAIL_01] = 22;
	m_pPositionOffset[TYPE_E_TRAIL_01] = _float3(0.7500f, 0.750f, 2.000f);
	m_pScaleOffset[TYPE_E_TRAIL_01] = _float3(2.25f, 2.25f, 2.25f);
	m_pRotationOffset[TYPE_E_TRAIL_01] = _float3(0.f, -20.000f, -10.000f);

	m_pFrameTriger[TYPE_P_CIRCLES_01] = 23;
	m_pPositionOffset[TYPE_P_CIRCLES_01] = _float3(0.f, 0.5f, 5.f);
	m_pScaleOffset[TYPE_P_CIRCLES_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLES_01] = _float3(0.f, 0.f, 0.f);


	// 1
	m_pFrameTriger[TYPE_E_TRAIL_02] = 57;
	m_pPositionOffset[TYPE_E_TRAIL_02] = _float3(0.f, 0.5f, 1.5f);
	m_pScaleOffset[TYPE_E_TRAIL_02] = _float3(2.5f, 2.5f, 2.5f);
	m_pRotationOffset[TYPE_E_TRAIL_02] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_CIRCLES_02] = 58;
	m_pPositionOffset[TYPE_P_CIRCLES_02] = _float3(0.f, 0.5f, 5.f);
	m_pScaleOffset[TYPE_P_CIRCLES_02]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLES_02] = _float3(0.f, 0.f, 0.f);


	return S_OK;
}

HRESULT CVfx_Stellia_Skill_ClawDouble::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_ClawDouble::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_E_TRAIL_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Stellia_Skill_LeftClaw"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_01], m_pScaleOffset[TYPE_E_TRAIL_01], m_pRotationOffset[TYPE_E_TRAIL_01]);
			//Safe_AddRef(m_pHand01Effect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLES_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLES_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Claw_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLES_01], m_pScaleOffset[TYPE_P_CIRCLES_01], m_pRotationOffset[TYPE_P_CIRCLES_01]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_02])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Stellia_Skill_RightClaw"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_02], m_pScaleOffset[TYPE_E_TRAIL_02], m_pRotationOffset[TYPE_E_TRAIL_02]);
			//Safe_AddRef(m_pHand02Effect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLES_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLES_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Claw_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLES_02], m_pScaleOffset[TYPE_P_CIRCLES_02], m_pRotationOffset[TYPE_P_CIRCLES_02]);
			m_iCount++;
		}
	}
}

void CVfx_Stellia_Skill_ClawDouble::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_ClawDouble::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_ClawDouble::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_ClawDouble* CVfx_Stellia_Skill_ClawDouble::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_ClawDouble* pInstance = new CVfx_Stellia_Skill_ClawDouble(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_ClawDouble");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_ClawDouble::Clone(void* pArg)
{
	CVfx_Stellia_Skill_ClawDouble* pInstance = new CVfx_Stellia_Skill_ClawDouble(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_ClawDouble");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_ClawDouble::Free()
{
	__super::Free();

	//if (nullptr != m_pHand01Effect)
	//{
	//	m_pHand01Effect->Set_Dead(true);
	//	Safe_Release(m_pHand01Effect);
	//}
	//
	//if (nullptr != m_pHand02Effect)
	//{
	//	m_pHand02Effect->Set_Dead(true);
	//	Safe_Release(m_pHand02Effect);
	//}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}