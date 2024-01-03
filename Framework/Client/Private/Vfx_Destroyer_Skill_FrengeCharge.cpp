#include "stdafx.h"
#include "Vfx_Destroyer_Skill_FrengeCharge.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_FrengeCharge::CVfx_Destroyer_Skill_FrengeCharge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_FrengeCharge::CVfx_Destroyer_Skill_FrengeCharge(const CVfx_Destroyer_Skill_FrengeCharge& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_FrengeCharge::Tick(_float fTimeDelta)
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

void CVfx_Destroyer_Skill_FrengeCharge::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_FrengeCharge* CVfx_Destroyer_Skill_FrengeCharge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_FrengeCharge* pInstance = new CVfx_Destroyer_Skill_FrengeCharge(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_FrengeCharge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_FrengeCharge::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_FrengeCharge* pInstance = new CVfx_Destroyer_Skill_FrengeCharge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_FrengeCharge");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_FrengeCharge::Free()
{
	__super::Free();
}