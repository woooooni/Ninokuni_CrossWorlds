#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage01Shield.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Skill_Rage01Shield::CVfx_Stellia_Skill_Rage01Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage01Shield::CVfx_Stellia_Skill_Rage01Shield(const CVfx_Stellia_Skill_Rage01Shield& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Shield::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pPositionOffset[TYPE_ET1_E_SHIELD] = _float3(0.f, 0.05f, 0.f);
	m_pScaleOffset[TYPE_ET1_E_SHIELD]    = _float3(38.f, 38.f, 38.f);
	m_pRotationOffset[TYPE_ET1_E_SHIELD] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Shield::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage01Shield::Tick(_float fTimeDelta)
{
	if (nullptr == m_pShield)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Shield"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_SHIELD], m_pScaleOffset[TYPE_ET1_E_SHIELD], m_pRotationOffset[TYPE_ET1_E_SHIELD], 
			m_pOwnerObject, &m_pShield, false);

		if (nullptr != m_pShield)
		{
			Safe_AddRef(m_pShield);
			m_pShield->Set_OwnerTransformOnly(true);
			m_pShield->Create_Dissolve(73,        // Index
				_float4(0.760f, 0.611f, 1.f, 1.f),// Color
				5.f,   // Speed
				10.f); // Total
		}
		return;
	}
	else
	{
		if (m_pOwnerObject == nullptr || m_pOwnerObject->Is_ReserveDead() || m_pOwnerObject->Is_Dead())
		{
			Set_Dead(true);
			return;
		}

		CStateMachine* pMachine = m_pOwnerObject->Get_Component<CStateMachine>(L"Com_StateMachine");
		if (pMachine == nullptr || 
			pMachine->Get_CurrState() == CStellia::STELLIA_RAGE2START_TURN_OC || 
			pMachine->Get_CurrState() == CStellia::STELLIA_RAGE2START_JUMP ||
			pMachine->Get_CurrState() == CStellia::STELLIA_RAGE2START_TURN_OL ||
			pMachine->Get_CurrState() == CStellia::STELLIA_RAGE2START)
		{
			Set_Dead(true);
			return;
		}

		if (pMachine->Get_CurrState() == CStellia::STELLIA_JUMPSTAMP ||
			pMachine->Get_CurrState() == CStellia::STELLIA_CHASEJUMPSTAMP ||
			pMachine->Get_CurrState() == CStellia::STELLIA_RAGE1START_JUMP ||
			pMachine->Get_CurrState() == CStellia::STELLIA_RAGE1LOOP_JUMPSTAMP ||
			pMachine->Get_CurrState() == CStellia::STELLIA_RAGE1LOOP_EXPLOSION)
		{
			if (true == m_pShield->Get_IsEffectShow())
			{
				m_pShield->Set_IsEffectShow(false);
				m_pShield->Start_Dissolve(73,         // Index
					_float4(0.760f, 0.611f, 1.f, 1.f),// Color
					5.f,   // Speed
					10.f,  // Total
					false); 
			}
		}
		else
		{
			if (false == m_pShield->Get_IsEffectShow())
			{
				m_pShield->Set_IsEffectShow(true);
				m_pShield->Create_Dissolve(73,        // Index
					_float4(0.760f, 0.611f, 1.f, 1.f),// Color
					5.f,   // Speed
					10.f); // Total
			}
		}
	}
}

void CVfx_Stellia_Skill_Rage01Shield::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Shield::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Shield::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage01Shield* CVfx_Stellia_Skill_Rage01Shield::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage01Shield* pInstance = new CVfx_Stellia_Skill_Rage01Shield(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage01Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage01Shield::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage01Shield* pInstance = new CVfx_Stellia_Skill_Rage01Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage01Shield");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage01Shield::Free()
{
	__super::Free();

	if (nullptr != m_pShield)
	{
		m_pShield->Start_Dissolve(73,         // Index
			_float4(0.760f, 0.611f, 1.f, 1.f),// Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pShield);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}