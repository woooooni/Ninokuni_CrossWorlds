#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_FlashHeal.h"

CSkill_Engineer_FlashHeal::CSkill_Engineer_FlashHeal()
{
	
}



HRESULT CSkill_Engineer_FlashHeal::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 15.f;

	return S_OK;
}

void CSkill_Engineer_FlashHeal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_FlashHeal::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Engineer_FlashHeal::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacter->Increase_HP(100);
		m_pCharacterStateMachine->Change_State(CCharacter::SKILL_SPECIAL_0);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Engineer_FlashHeal* CSkill_Engineer_FlashHeal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Engineer_FlashHeal* pInstance = new CSkill_Engineer_FlashHeal;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_FlashHeal");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_FlashHeal::Free()
{
	__super::Free();
}
