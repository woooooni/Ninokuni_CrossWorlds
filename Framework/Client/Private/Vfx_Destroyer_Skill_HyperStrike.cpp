#include "stdafx.h"
#include "Vfx_Destroyer_Skill_HyperStrike.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_HyperStrike::CVfx_Destroyer_Skill_HyperStrike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_HyperStrike::CVfx_Destroyer_Skill_HyperStrike(const CVfx_Destroyer_Skill_HyperStrike& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_HyperStrike::Tick(_float fTimeDelta)
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

void CVfx_Destroyer_Skill_HyperStrike::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_HyperStrike* CVfx_Destroyer_Skill_HyperStrike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_HyperStrike* pInstance = new CVfx_Destroyer_Skill_HyperStrike(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_HyperStrike");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_HyperStrike::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_HyperStrike* pInstance = new CVfx_Destroyer_Skill_HyperStrike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_HyperStrike");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_HyperStrike::Free()
{
	__super::Free();
}