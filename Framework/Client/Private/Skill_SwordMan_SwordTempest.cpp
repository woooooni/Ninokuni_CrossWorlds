#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_SwordTempest.h"

CSkill_SwordMan_SwordTempest::CSkill_SwordMan_SwordTempest()
{
	
}



HRESULT CSkill_SwordMan_SwordTempest::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SwordMan_SwordTempest::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_SwordMan_SwordTempest::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_SwordMan_SwordTempest::Use_Skill()
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


CSkill_SwordMan_SwordTempest* CSkill_SwordMan_SwordTempest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_SwordMan_SwordTempest* pInstance = new CSkill_SwordMan_SwordTempest;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_SwordMan_SwordTempest");
		return nullptr;
	}

	return pInstance;
}

void CSkill_SwordMan_SwordTempest::Free()
{
	__super::Free();
	
}
