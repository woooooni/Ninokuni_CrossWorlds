#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Destroyer_IgnorePain.h"

CSkill_Destroyer_IgnorePain::CSkill_Destroyer_IgnorePain()
{
	
}



HRESULT CSkill_Destroyer_IgnorePain::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 15.f;

	return S_OK;
}

void CSkill_Destroyer_IgnorePain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Destroyer_IgnorePain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Destroyer_IgnorePain::Use_Skill()
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


CSkill_Destroyer_IgnorePain* CSkill_Destroyer_IgnorePain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Destroyer_IgnorePain* pInstance = new CSkill_Destroyer_IgnorePain;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Destroyer_IgnorePain");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Destroyer_IgnorePain::Free()
{
	__super::Free();
	
}
