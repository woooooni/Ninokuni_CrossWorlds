#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage01Shield_All.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

#include "Stellia.h"

CVfx_Stellia_Skill_Rage01Shield_All::CVfx_Stellia_Skill_Rage01Shield_All(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage01Shield_All::CVfx_Stellia_Skill_Rage01Shield_All(const CVfx_Stellia_Skill_Rage01Shield_All& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Shield_All::Initialize_Prototype()
{
	//m_bOwnerStateIndex = CStellia::STELLIA_RAGE1LOOP_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_V_E_SHIELD_ALL] = 0;
	m_pPositionOffset[TYPE_V_E_SHIELD_ALL] = _float3(0.f, 0.2f, 0.f);
	m_pScaleOffset[TYPE_V_E_SHIELD_ALL] = _float3(17.f, 17.f, 17.f);
	m_pRotationOffset[TYPE_V_E_SHIELD_ALL] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Shield_All::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage01Shield_All::Tick(_float fTimeDelta)
{
	if (!m_bIsStart)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Rage01_Shield_All"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_V_E_SHIELD_ALL], m_pScaleOffset[TYPE_V_E_SHIELD_ALL], m_pRotationOffset[TYPE_V_E_SHIELD_ALL]);

		m_bIsStart = true;
	}

	if (m_bFinish || m_pOwnerObject == nullptr || m_pOwnerObject->Is_ReserveDead() || m_pOwnerObject->Is_Dead())
	{
		Set_Dead(true);
		return;
	}

	CStateMachine* pMachine = m_pOwnerObject->Get_Component<CStateMachine>(L"Com_StateMachine");

	if (pMachine == nullptr || pMachine->Get_CurrState() == CStellia::STELLIA_RAGE1LOOP_EXPLOSION)
	{
		Set_Dead(true);
		return;
	}
}

void CVfx_Stellia_Skill_Rage01Shield_All::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Shield_All::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Shield_All::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage01Shield_All* CVfx_Stellia_Skill_Rage01Shield_All::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage01Shield_All* pInstance = new CVfx_Stellia_Skill_Rage01Shield_All(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage01Shield_All");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage01Shield_All::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage01Shield_All* pInstance = new CVfx_Stellia_Skill_Rage01Shield_All(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage01Shield_All");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage01Shield_All::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}