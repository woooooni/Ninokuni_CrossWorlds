#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Destroyer_FrengeCharge.h"

CSkill_Destroyer_FrengeCharge::CSkill_Destroyer_FrengeCharge()
{
	
}



HRESULT CSkill_Destroyer_FrengeCharge::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Destroyer_FrengeCharge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Destroyer_FrengeCharge::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Destroyer_FrengeCharge::Use_Skill()
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


CSkill_Destroyer_FrengeCharge* CSkill_Destroyer_FrengeCharge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Destroyer_FrengeCharge* pInstance = new CSkill_Destroyer_FrengeCharge;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Destroyer_FrengeCharge");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Destroyer_FrengeCharge::Free()
{
	__super::Free();
	
}
