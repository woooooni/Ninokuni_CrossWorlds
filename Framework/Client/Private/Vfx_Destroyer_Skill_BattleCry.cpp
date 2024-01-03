#include "stdafx.h"
#include "Vfx_Destroyer_Skill_BattleCry.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_BattleCry::CVfx_Destroyer_Skill_BattleCry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_BattleCry::CVfx_Destroyer_Skill_BattleCry(const CVfx_Destroyer_Skill_BattleCry& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_BattleCry::Tick(_float fTimeDelta)
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

void CVfx_Destroyer_Skill_BattleCry::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_BattleCry* CVfx_Destroyer_Skill_BattleCry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_BattleCry* pInstance = new CVfx_Destroyer_Skill_BattleCry(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_BattleCry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_BattleCry::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_BattleCry* pInstance = new CVfx_Destroyer_Skill_BattleCry(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_BattleCry");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_BattleCry::Free()
{
	__super::Free();
}