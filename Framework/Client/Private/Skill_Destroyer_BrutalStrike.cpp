#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Destroyer_BrutalStrike.h"

CSkill_Destroyer_BrutalStrike::CSkill_Destroyer_BrutalStrike()
{
	
}



HRESULT CSkill_Destroyer_BrutalStrike::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Destroyer_BrutalStrike::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Destroyer_BrutalStrike::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Destroyer_BrutalStrike::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::CLASS_SKILL_1);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Destroyer_BrutalStrike* CSkill_Destroyer_BrutalStrike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Destroyer_BrutalStrike* pInstance = new CSkill_Destroyer_BrutalStrike;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Destroyer_BrutalStrike");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Destroyer_BrutalStrike::Free()
{
	__super::Free();
	
}
