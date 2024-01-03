#include "stdafx.h"
#include "Vfx_Destroyer_Skill_LeafSlam.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_LeafSlam::CVfx_Destroyer_Skill_LeafSlam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_LeafSlam::CVfx_Destroyer_Skill_LeafSlam(const CVfx_Destroyer_Skill_LeafSlam& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_LeafSlam::Tick(_float fTimeDelta)
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

void CVfx_Destroyer_Skill_LeafSlam::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_LeafSlam* CVfx_Destroyer_Skill_LeafSlam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_LeafSlam* pInstance = new CVfx_Destroyer_Skill_LeafSlam(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_LeafSlam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_LeafSlam::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_LeafSlam* pInstance = new CVfx_Destroyer_Skill_LeafSlam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_LeafSlam");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_LeafSlam::Free()
{
	__super::Free();
}