#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_HealingTree.h"

CSkill_Engineer_HealingTree::CSkill_Engineer_HealingTree()
{
	
}



HRESULT CSkill_Engineer_HealingTree::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 30.f;

	return S_OK;
}

void CSkill_Engineer_HealingTree::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_HealingTree::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Engineer_HealingTree::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::SKILL_SPECIAL_2);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Engineer_HealingTree* CSkill_Engineer_HealingTree::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Engineer_HealingTree* pInstance = new CSkill_Engineer_HealingTree;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_HealingTree");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_HealingTree::Free()
{
	__super::Free();
}
