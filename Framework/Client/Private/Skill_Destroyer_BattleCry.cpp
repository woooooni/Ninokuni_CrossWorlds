#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Destroyer_BattleCry.h"

CSkill_Destroyer_BattleCry::CSkill_Destroyer_BattleCry()
{
	
}



HRESULT CSkill_Destroyer_BattleCry::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Destroyer_BattleCry::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Destroyer_BattleCry::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Destroyer_BattleCry::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::SKILL_SPECIAL_0);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Destroyer_BattleCry* CSkill_Destroyer_BattleCry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Destroyer_BattleCry* pInstance = new CSkill_Destroyer_BattleCry;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Destroyer_BattleCry");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Destroyer_BattleCry::Free()
{
	__super::Free();
	
}
