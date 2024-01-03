#include "stdafx.h"
#include "Vfx_SwordMan_Skill_AcaneBarrier.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_AcaneBarrier::CVfx_SwordMan_Skill_AcaneBarrier(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_AcaneBarrier::CVfx_SwordMan_Skill_AcaneBarrier(const CVfx_SwordMan_Skill_AcaneBarrier& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_AcaneBarrier::Tick(_float fTimeDelta)
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

void CVfx_SwordMan_Skill_AcaneBarrier::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_AcaneBarrier* CVfx_SwordMan_Skill_AcaneBarrier::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_AcaneBarrier* pInstance = new CVfx_SwordMan_Skill_AcaneBarrier(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_AcaneBarrier");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_AcaneBarrier::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_AcaneBarrier* pInstance = new CVfx_SwordMan_Skill_AcaneBarrier(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_AcaneBarrier");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_AcaneBarrier::Free()
{
	__super::Free();
}