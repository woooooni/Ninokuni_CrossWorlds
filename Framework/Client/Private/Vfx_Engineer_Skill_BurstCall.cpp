#include "stdafx.h"
#include "Vfx_Engineer_Skill_BurstCall.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_BurstCall::CVfx_Engineer_Skill_BurstCall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_BurstCall::CVfx_Engineer_Skill_BurstCall(const CVfx_Engineer_Skill_BurstCall& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_BurstCall::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_BurstCall::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_BurstCall::Tick(_float fTimeDelta)
{
	if (m_pOwnerObject != nullptr)
	{
		CStateMachine* pMachine = m_pOwnerObject->Get_Component<CStateMachine>(L"Com_StateMachine");
		if (pMachine != nullptr)
		{
			if (pMachine->Get_CurrState() != CCharacter::CLASS_SKILL_1)
			{
				Set_Dead(true);
				return;
			}
		}

		m_fTimeAcc += fTimeDelta;
		// 
		if (m_iCount == 0)
		{
			m_fTimeAcc = 0.f;
			m_iCount++;
		}
	}
}

void CVfx_Engineer_Skill_BurstCall::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_BurstCall::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_BurstCall::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_BurstCall* CVfx_Engineer_Skill_BurstCall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_BurstCall* pInstance = new CVfx_Engineer_Skill_BurstCall(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_BurstCall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_BurstCall::Clone(void* pArg)
{
	CVfx_Engineer_Skill_BurstCall* pInstance = new CVfx_Engineer_Skill_BurstCall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SpinningAssault");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_BurstCall::Free()
{
	__super::Free();
}