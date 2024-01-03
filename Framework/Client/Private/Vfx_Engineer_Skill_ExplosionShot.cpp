#include "stdafx.h"
#include "Vfx_Engineer_Skill_ExplosionShot.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_ExplosionShot::CVfx_Engineer_Skill_ExplosionShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_ExplosionShot::CVfx_Engineer_Skill_ExplosionShot(const CVfx_Engineer_Skill_ExplosionShot& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_ExplosionShot::Tick(_float fTimeDelta)
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

void CVfx_Engineer_Skill_ExplosionShot::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_ExplosionShot* CVfx_Engineer_Skill_ExplosionShot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_ExplosionShot* pInstance = new CVfx_Engineer_Skill_ExplosionShot(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_ExplosionShot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_ExplosionShot::Clone(void* pArg)
{
	CVfx_Engineer_Skill_ExplosionShot* pInstance = new CVfx_Engineer_Skill_ExplosionShot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_ExplosionShot");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_ExplosionShot::Free()
{
	__super::Free();
}